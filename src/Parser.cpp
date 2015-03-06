#include "Nebuleuse.h"
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"

#define PARSEANDCHECK(x)	if (doc.Parse(x.c_str()).HasParseError()){\
								return ThrowError(NEBULEUSE_ERROR_PARSEFAILED);\
							}\
							if (!doc.IsObject()){\
								return ThrowError(NEBULEUSE_ERROR_PARSEFAILED);\
							}\
							if (doc.HasMember("Code") && doc.HasMember("Message")){\
								if (doc["Code"].IsInt()){\
									return ThrowError(doc["Code"].GetInt(), doc["Message"].GetString());\
								}\
							}
#define STDTOJSONVAL(x) StringRef(x.c_str())

namespace Neb{
	using namespace rapidjson;

	void Nebuleuse::Parse_Errors(string data){
		Document doc;
		PARSEANDCHECK(data)
	}

	void Nebuleuse::Parse_Status(string data){
		Document doc;
		PARSEANDCHECK(data)

		if (doc.HasMember("Maintenance") && doc["Maintenance"].IsBool()){
			if (doc["Maintenance"].GetBool())
				return ThrowError(NEBULEUSE_ERROR_MAINTENANCE);
		}
		if (doc.HasMember("NebuleuseVersion") && doc["NebuleuseVersion"].IsInt()){
			if (doc["NebuleuseVersion"].GetInt() > NEBULEUSEVERSION)
				return ThrowError(NEBULEUSE_ERROR_OUTDATED);
		}
		if (doc.HasMember("GameVersion") && doc["GameVersion"].IsUint()){
			if (doc["GameVersion"].GetUint() > _Version){
				_ServerVersion = doc["GameVersion"].GetUint();
				return ThrowError(NEBULEUSE_ERROR_OUTDATED);
			}
		}
		if (doc.HasMember("ComplexStatsTable") && doc["ComplexStatsTable"].IsArray()){
			const Value &CSTable = doc["ComplexStatsTable"];
			for (rapidjson::SizeType i = 0; i < CSTable.Size(); i++) {
				const Value& CST = CSTable[i];
				if (CST.HasMember("Name") && CST.HasMember("Fields") && CST.HasMember("AutoCount")){
					ComplexStatsTableInfos infos;
					infos.Name = CST["Name"].GetString();
					infos.AutoCount = CST["AutoCount"].GetBool();
					for (rapidjson::SizeType i = 0; i < CST["Fields"].Size(); i++)	{
						infos.Fields.push_back(CST["Fields"][i].GetString());
					}
					_CStatsTableInfos[CST["Name"].GetString()] = infos;
				}
			}
		}
		
		return;
	}
	void Nebuleuse::Parse_Connect(string data){
		Document doc;
		PARSEANDCHECK(data)

		if (!doc.HasMember("SessionId") && doc["SessionId"].IsString()){
			return ThrowError(NEBULEUSE_ERROR_PARSEFAILED);
		}
		_SessionID = doc["SessionId"].GetString();

		ProceedConnection();
	}
	void Nebuleuse::Parse_UserInfos(string data){
		Document doc;
		PARSEANDCHECK(data)

			if (doc.HasMember("Rank") && doc["Rank"].IsInt()){
			_UserRank = static_cast<NebuleuseUserRank>(doc["Rank"].GetInt());
		}

		if (doc.HasMember("Avatar") && doc["Avatar"].IsString()){
			_AvatarUrl = doc["Avatar"].GetString();
		}

		if (doc.HasMember("Achievements") && doc["Achievements"].IsArray())
		{
			_Achievements.clear();
			const Value& achievements = doc["Achievements"];
			int AchNbr = 0;
			for (rapidjson::SizeType i = 0; i < achievements.Size(); i++)
			{
				const Value& Ach = achievements[i];
				if (Ach.HasMember("Name") && Ach.HasMember("Progress") && Ach.HasMember("Value"))
				{
					Achievement newAchievement;
					newAchievement.Name = Ach["Name"].GetString();
					newAchievement.Progress = Ach["Progress"].GetUint();
					newAchievement.ProgressMax = Ach["Value"].GetUint();
					newAchievement.Id = Ach["Id"].GetUint();
					newAchievement.Changed = false;
					_Achievements[newAchievement.Name] = newAchievement;
					AchNbr++;
				}
			}
		}
		if (doc.HasMember("Stats") && doc["Stats"].IsArray())
		{
			_UserStats.clear();
			const Value& stats = doc["Stats"];
			for (rapidjson::SizeType i = 0; i < stats.Size(); i++)
			{
				const Value& Stat = stats[i];
				if (Stat.HasMember("Name") && Stat.HasMember("Value"))
				{
					UserStat stt;
					stt.Name = Stat["Name"].GetString();
					stt.Value = Stat["Value"].GetInt();
					stt.Changed = false;
					_UserStats[stt.Name] = stt;
				}
			}
		}
		FinishConnect();
	}
	string Nebuleuse::Parse_CreateComplexStatJson(){
		Document doc;
		doc.SetObject();
		Document::AllocatorType& allocator = doc.GetAllocator();
		Value AllStats(kArrayType);
		for (unsigned int i = 0; i < _CStats.size(); i++) // Insert all the different stats we have
		{
			Value Vals(kArrayType), ComplexStat(kObjectType);
			ComplexStat.AddMember("Name", STDTOJSONVAL(_CStats[i].Name), allocator);
			for (map<string, string>::iterator it = _CStats[i].Values.begin(); it != _CStats[i].Values.end(); ++it){
				Value St(kObjectType);
				St.AddMember("Name", STDTOJSONVAL(it->first), allocator);
				St.AddMember("Value", STDTOJSONVAL(it->second), allocator);
				Vals.PushBack(St, allocator);
			}
			ComplexStat.AddMember("Values", Vals, allocator);
			AllStats.PushBack(ComplexStat, allocator);
		}
		doc.AddMember("Stats", AllStats, allocator);
		StringBuffer buffer;
		PrettyWriter<StringBuffer> writer(buffer);
		doc.Accept(writer);

		_CStats.clear();

		return buffer.GetString();
	}
	string Nebuleuse::Parse_CreateChangedStatsJson(){
		Document doc;
		doc.SetObject();
		Document::AllocatorType& allocator = doc.GetAllocator();
		Value AllStats(kArrayType);

		for (map<string, UserStat>::iterator it = _UserStats.begin(); it != _UserStats.end(); ++it){
			if (!it->second.Changed)
				continue;

			Value Stat(kObjectType);

			Stat.AddMember("Name", STDTOJSONVAL(it->first), allocator);
			Stat.AddMember("Value", it->second.Value, allocator);
			
			AllStats.PushBack(Stat, allocator);
		}

		doc.AddMember("Stats", AllStats, allocator);
		StringBuffer buffer;
		PrettyWriter<StringBuffer> writer(buffer);
		doc.Accept(writer);

		return buffer.GetString();
	}

	string Nebuleuse::Parse_CreateChangedAchievementsJson(){
		Document doc;
		doc.SetObject();
		Document::AllocatorType& allocator = doc.GetAllocator();
		Value AllAch(kArrayType);
		
		for (map<string, Achievement>::iterator it = _Achievements.begin(); it != _Achievements.end(); ++it){
			if (!it->second.Changed)
				continue;

			Value Ach(kObjectType);

			Ach.AddMember("Id", it->second.Id, allocator);
			Ach.AddMember("Value", it->second.Progress, allocator);

			AllAch.PushBack(Ach, allocator);

			it->second.Changed = false;
		}
		
		doc.AddMember("Achievements", AllAch, allocator);
		StringBuffer buffer;
		PrettyWriter<StringBuffer> writer(buffer);
		doc.Accept(writer);

		return buffer.GetString();
	}
}