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
				_UserStats[i].Value = value;
				SendStat(_UserStats[i]);
			}
		}
	}
	void Nebuleuse::SendStat(UserStat stat){
		std::string msg = Parse_CreateStatUpdateJson(stat);
		Talk_SendStatsUpdate(msg);
	}
}