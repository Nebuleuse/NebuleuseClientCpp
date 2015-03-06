#include "Nebuleuse.h"

namespace Neb{
	int Nebuleuse::GetUserStats(std::string name){
		return _UserStats[name].Value;
	}
	void Nebuleuse::SetUserStats(std::string name, int value){
		if (_UserStats[name].Value != value){
			_UserStats[name].Value = value;
			_UserStats[name].Changed = true;
			SendStats();
		}
	}
	void Nebuleuse::SendStats(){
		if (IsUnavailable())
			return;

		std::string msg = Parse_CreateChangedStatsJson();
		Talk_SendStatsUpdate(msg);
	}

	void Nebuleuse::AddComplexStat(ComplexStat stat){
		if (VerifyComplexStat(stat))
			_CStats.push_back(stat);
		else
			ThrowError(NEBULEUSE_ERROR, "Could not add complex stats, verification failed!");
	}
	bool Nebuleuse::VerifyComplexStat(ComplexStat stat){
		if (_CStatsTableInfos.count(stat.Name) == 0)
			return false;
		ComplexStatsTableInfos infos = _CStatsTableInfos[stat.Name];
		//Iterate the fields we were given and check if they are present in the ComplexStatsTableInfo.
		for (map<string, string>::iterator it = stat.Values.begin(); it != stat.Values.end(); ++it){
			if (find(infos.Fields.begin(), infos.Fields.end(), it->first) == infos.Fields.end())
				return false;
		}
		return true;
	}

	void Nebuleuse::SendComplexStats(){
		if (IsUnavailable())
			return;

		std::string Msg;
		Msg = Parse_CreateComplexStatJson();
		Talk_SendComplexStats(Msg);
	}
}