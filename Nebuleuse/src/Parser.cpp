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

	void Nebuleuse::Parse_Errors(std::string data){
		Document doc;
		PARSEANDCHECK(data)
	}

	void Nebuleuse::Parse_Status(std::string data){
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
			if (doc["GameVersion"].GetUint() > _Version)
				return ThrowError(NEBULEUSE_ERROR_OUTDATED);
		}
		if (doc.HasMember("Motd") && doc["Motd"].IsString()){
			_Motd = doc["Motd"].GetString();
		}
		
		return;
	}
	void Nebuleuse::Parse_Connect(std::string data){
		Document doc;
		PARSEANDCHECK(data)

		if (!doc.HasMember("SessionId") || !doc["SessionId"].IsString()){
			return ThrowError(NEBULEUSE_ERROR_PARSEFAILED);
		}
		_SessionID = doc["SessionId"].GetString();

		return;
	}
	void Nebuleuse::Parse_UserInfos(std::string data){
		Document doc;
		PARSEANDCHECK(data)

		if (doc.HasMember("Rank")){
			_PlayerRank = static_cast<NebuleusePlayerRank>(doc["Rank"].GetInt());
		}

		if (doc.HasMember("Avatar")){
			_AvatarUrl = doc["Avatar"].GetString();
		}

		if (doc.HasMember("Achievements"))
		{
			const Value& achievements = doc["Achievements"];
			int AchNbr = 0;
			for (rapidjson::SizeType i = 0; i < achievements.Size(); i++)
			{
				const Value& Ach = achievements[i];
				if (Ach.HasMember("Name") && Ach.HasMember("Progress") && Ach.HasMember("Value"))
				{
					AchievementData newAchievement;
					newAchievement.Name = Ach["Name"].GetString();
					newAchievement.Progress = Ach["Progress"].GetUint();
					newAchievement.ProgressMax = Ach["Value"].GetUint();
					newAchievement.Id = AchNbr;
					_Achievements.push_back(newAchievement);
					AchNbr++;
				}
			}
		}
		if (doc.HasMember("Stats"))
		{
			const Value& stats = doc["Stats"];
			for (rapidjson::SizeType i = 0; i < stats.Size(); i++)
			{
				const Value& Stat = stats[i];
				if (Stat.HasMember("Name") && Stat.HasMember("Value"))
				{
					PlayerStat stt;
					stt.Name = Stat["Name"].GetString();
					stt.Value = Stat["Value"].GetInt();
					stt.Changed = false;
					_PlayerStats.push_back(stt);
				}
			}
		}
		FinishConnect();
	}
	std::string Nebuleuse::Parse_CreateComplexStatJson(){
		Document doc;
		doc.SetObject();
		Document::AllocatorType& allocator = doc.GetAllocator();
		Value AllStats(kArrayType);
		for (int i = 0; i < _CStats.size(); i++) // Insert all the different stats we have
		{
			Value Vals(kArrayType), ComplexStat(kObjectType);
			ComplexStat.AddMember("Name", STDTOJSONVAL(_CStats[i].Name), allocator);
			for (int j = 0; j < _CStats[i].Values.size(); j++)
			{
				Value St(kObjectType);
				St.AddMember("Name", STDTOJSONVAL(_CStats[i].Values[j].Name), allocator);
				St.AddMember("Value", STDTOJSONVAL(_CStats[i].Values[j].Value), allocator);
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
}