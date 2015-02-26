#include "Nebuleuse.h"

namespace Neb{
	int Nebuleuse::GetUserStats(std::string name){
		for (size_t i = 0; i < _UserStats.size(); i++) {
			if (_UserStats[i].Name == name)
				return _UserStats[i].Value;
		}
		return 0;
	}
	void Nebuleuse::SetUserStats(std::string name, int value){
		for (size_t i = 0; i < _UserStats.size(); i++) {
			if (_UserStats[i].Name == name){
				if(_UserStats[i].Value != value){
					_UserStats[i].Value = value;
					SendStats();
				}
				return;
			}
		}
	}
	void Nebuleuse::SendStats(){
		if (IsUnavailable())
			return;

		std::string msg = Parse_CreateChangedStatsJson();
		Talk_SendStatsUpdate(msg);
	}

	void Nebuleuse::AddComplexStat(ComplexStat stat){
		_CStats.push_back(stat);
	}

	void Nebuleuse::SendComplexStats(){
		if (IsUnavailable())
			return;

		std::string Msg;
		Msg = Parse_CreateComplexStatJson();
		Talk_SendComplexStats(Msg);
	}
}