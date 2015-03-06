#include "Nebuleuse.h"
#include <algorithm>

namespace Neb{
	unsigned int Nebuleuse::GetAchievementCount(){
		return _Achievements.size();
	}
	Achievement Nebuleuse::GetAchievement(string name){
		return _Achievements[name];
	}
	Achievement Nebuleuse::GetAchievement(int id){
		for(map<string, Achievement>::iterator it = _Achievements.begin(); it != _Achievements.end(); ++it){
			if (it->second.Id == id)
				return it->second;
		}
		return Achievement();
	}
	void Nebuleuse::SetAchievement(string name, Achievement ach){
		_Achievements[name] = ach;
		UpdateAchievement(name, ach.Progress);
	}
	void Nebuleuse::UpdateAchievementProgress(string name, unsigned int progress){
		UpdateAchievement(name, progress);
	}
	void Nebuleuse::UpdateAchievementProgress(int id, unsigned int progress){
		for (map<string, Achievement>::iterator it = _Achievements.begin(); it != _Achievements.end(); ++it){
			if (it->second.Id == id){
				UpdateAchievement(it->second.Name, progress);
				break;
			}
		}
	}
	void Nebuleuse::UnlockAchievement(string name){
		UpdateAchievement(name, _Achievements[name].ProgressMax);
	}
	void Nebuleuse::UnlockAchievement(int id){
		for (map<string, Achievement>::iterator it = _Achievements.begin(); it != _Achievements.end(); ++it){
			if (it->second.Id == id){
				UpdateAchievement(it->second.Name, it->second.ProgressMax);
				break;
			}
		}
	}
	void Nebuleuse::UpdateAchievement(string name, unsigned int progress){
		if(_Achievements[name].IsCompleted() || _Achievements[name].Progress == progress)
			return;
					
		if(progress >= _Achievements[name].ProgressMax)
			if (_AchievementEarned_CallBack)
				_AchievementEarned_CallBack(_Achievements[name].Name);
				
		_Achievements[name].Progress = progress > _Achievements[name].ProgressMax ? _Achievements[name].ProgressMax : progress;
		_Achievements[name].Changed = true;
		SendAchievements();
	}
	void Nebuleuse::SendAchievements(){
		if (IsUnavailable())
			return;
		
		string msg = Parse_CreateChangedAchievementsJson();
		Talk_SendAchievementProgress(msg);
	}
}