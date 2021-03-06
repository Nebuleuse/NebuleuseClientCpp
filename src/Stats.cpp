#include <algorithm>
#include "Nebuleuse.h"
#include "Macros.h"

namespace Neb{
	int Nebuleuse::GetUserStats(std::string name){
		lock_guard<mutex> lock(_DataLock);
		return _Self.Stats[name].Value;
	}
	void Nebuleuse::SetUserStats(std::string name, int value){
		_DataLock.lock();
		if (_Self.Stats[name].Value != value){
			_Self.Stats[name].Value = value;
			_Self.Stats[name].Changed = true;
			_DataLock.unlock();
			SendStats();
		}
	}
	void Nebuleuse::SendStats(){
		if (IsUnavailable() || CountChangedStats() == 0)
			return;

		STARTCOMTHREAD(SendStatsUpdate);
	}
	int Nebuleuse::CountChangedStats(){
		lock_guard<mutex> lock(_DataLock);
		int count = 0;
		for (map<string, UserStat>::iterator it = _Self.Stats.begin(); it != _Self.Stats.end(); ++it){
			if (!it->second.Changed)
				continue;
			count++;
		}
		return count;
	}
	void Nebuleuse::AddComplexStat(ComplexStat stat){
		lock_guard<mutex> lock(_DataLock);
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
		if (IsUnavailable() || _CStats.size() == 0)
			return;

		STARTCOMTHREAD(SendComplexStats);
	}
}