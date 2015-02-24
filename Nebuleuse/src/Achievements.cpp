#include "Nebuleuse.h"

namespace Neb{
	unsigned int Nebuleuse::GetAchievementCount(){
		return _Achievements.size();
	}
	Achievement Nebuleuse::GetAchievement(std::string name){
		for (size_t i = 0; i < _Achievements.size(); i++) {
			if (_Achievements[i].Name == name)
				return _Achievements[i];
		}
	}
	Achievement Nebuleuse::GetAchievement(int id){
		for (size_t i = 0; i < _Achievements.size(); i++) {
			if (_Achievements[i].Id == id)
				return _Achievements[i];
		}
	}
	void Nebuleuse::SetAchievement(Achievement ach){
		for (size_t i = 0; i < _Achievements.size(); i++) {
			if (_Achievements[i].Id == ach.Id){
				_Achievements[i] = ach;
				UpdateAchievement(i, ach.ProgressMax)
			}
		}
	}
	void Nebuleuse::UpdateAchievementProgress(std::string name, int progress){
		for (size_t i = 0; i < _Achievements.size(); i++) {
			if (_Achievements[i].Name == name){
				UpdateAchievement(i, progress);
				break;
			}
		}
	}
	void Nebuleuse::UpdateAchievementProgress(int id, int progress){
		for (size_t i = 0; i < _Achievements.size(); i++) {
			if (_Achievements[i].Id == id){
				UpdateAchievement(i, progress);
				break;
			}
		}
	}
	void Nebuleuse::UnlockAchievement(std::string name){
		for (size_t i = 0; i < _Achievements.size(); i++) {
			if (_Achievements[i].Name == name){
				UpdateAchievement(i, _Achievements[i].ProgressMax);
				break;
			}
		}
	}
	void Nebuleuse::UnlockAchievement(int id){
		for (size_t i = 0; i < _Achievements.size(); i++) {
			if (_Achievements[i].Id == id){
				UpdateAchievement(i, _Achievements[i].ProgressMax);
				break;
			}
		}
	}
	void Nebuleuse::UpdateAchievement(int i, int progress){
		if(_Achievements[i].IsCompleted() || _Achievements[i].Progress == progress)
			return;
					
		if(progress >= _Achievements[i].ProgressMax)
			if (_AchievementEarned_CallBack)
				_AchievementEarned_CallBack(_Achievements[i].Name);
				
		_Achievements[i].Progress = min(progress, _Achievements[i].ProgressMax);
		_Achievements[i].Changed = true;
		SendAchievements();
	}
	void Nebuleuse::SendAchievements(){
		if (IsUnavailable())
			return;
		
		std::string msg = Parse_CreateChangedAchievementsJson();
		Talk_SendAchievementProgress(msg);
	}
}