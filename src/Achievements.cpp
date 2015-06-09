#include "Nebuleuse.h"
#include "Macros.h"
#include <algorithm>

namespace Neb{
	unsigned int Nebuleuse::GetAchievementCount(){
		lock_guard<mutex> lock(_DataLock);
		return _Self.Achievements.size();
	}
	Achievement Nebuleuse::GetAchievement(string name){
		lock_guard<mutex> lock(_DataLock);
		return _Self.Achievements[name];
	}
	Achievement Nebuleuse::GetAchievement(int id){
		lock_guard<mutex> lock(_DataLock);
		for (map<string, Achievement>::iterator it = _Self.Achievements.begin(); it != _Self.Achievements.end(); ++it){
			if (it->second.Id == id)
				return it->second;
		}
		return Achievement();
	}
	void Nebuleuse::SetAchievement(string name, Achievement ach){
		UpdateAchievement(name, ach.Progress);
	}
	void Nebuleuse::UpdateAchievementProgress(string name, unsigned int progress){
		UpdateAchievement(name, progress);
	}
	void Nebuleuse::UpdateAchievementProgress(int id, unsigned int progress){
		for (map<string, Achievement>::iterator it = _Self.Achievements.begin(); it != _Self.Achievements.end(); ++it){
			if (it->second.Id == id){
				UpdateAchievement(it->second.Name, progress);
				break;
			}
		}
	}
	void Nebuleuse::UnlockAchievement(string name){
		UpdateAchievement(name, _Self.Achievements[name].ProgressMax);
	}
	void Nebuleuse::UnlockAchievement(int id){
		for (map<string, Achievement>::iterator it = _Self.Achievements.begin(); it != _Self.Achievements.end(); ++it){
			if (it->second.Id == id){
				UpdateAchievement(it->second.Name, it->second.ProgressMax);
				break;
			}
		}
	}
	void Nebuleuse::UpdateAchievement(string name, unsigned int progress){
		_DataLock.lock();
		if (_Self.Achievements[name].IsCompleted() || _Self.Achievements[name].Progress == progress)
			return;
					
		if (progress >= _Self.Achievements[name].ProgressMax)
			if (_AchievementEarned_CallBack)
				_AchievementEarned_CallBack(_Self.Achievements[name].Name);
				
		_Self.Achievements[name].Progress = progress > _Self.Achievements[name].ProgressMax ? _Self.Achievements[name].ProgressMax : progress;
		_Self.Achievements[name].Changed = true;
		_DataLock.unlock();
		SendAchievements();
	}
	int Nebuleuse::CountChangedAchievements(){
		lock_guard<mutex> lock(_DataLock);
		int count = 0;
		for (map<string, Achievement>::iterator it = _Self.Achievements.begin(); it != _Self.Achievements.end(); ++it){
			if (!it->second.Changed)
				continue;
			count++;
		}
		return count;
	}
	void Nebuleuse::SendAchievements(){
		if (IsUnavailable() || CountChangedAchievements() == 0)
			return;
		
		STARTCOMTHREAD(SendAchievementProgress)
	}
}