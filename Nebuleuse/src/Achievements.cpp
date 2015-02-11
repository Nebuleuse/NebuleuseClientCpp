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
				SendAchievement(_Achievements[i]);
			}
		}
	}
	void Nebuleuse::UpdateAchievementProgress(std::string name, int progress){
		for (size_t i = 0; i < _Achievements.size(); i++) {
			if (_Achievements[i].Name == name){
				_Achievements[i].Progress = progress;
				SendAchievement(_Achievements[i]);
				break;
			}
		}
	}
	void Nebuleuse::UpdateAchievementProgress(int id, int progress){
		for (size_t i = 0; i < _Achievements.size(); i++) {
			if (_Achievements[i].Id == id){
				_Achievements[i].Progress = progress;
				SendAchievement(_Achievements[i]);
				break;
			}
		}
	}
	void Nebuleuse::UnlockAchievement(std::string name){
		for (size_t i = 0; i < _Achievements.size(); i++) {
			if (_Achievements[i].Name == name){
				_Achievements[i].Complete();
				SendAchievement(_Achievements[i]);
				break;
			}
		}
	}
	void Nebuleuse::UnlockAchievement(int id){
		for (size_t i = 0; i < _Achievements.size(); i++) {
			if (_Achievements[i].Id == id){
				_Achievements[i].Complete();
				SendAchievement(_Achievements[i]);
				break;
			}
		}
	}
	void Nebuleuse::SendAchievement(Achievement ach){
		if (IsUnavailable()){
			for (size_t i = 0; i < _Achievements.size(); i++) {
				if (_Achievements[i].Id == ach.Id){
					_Achievements[i].Changed = true;
				}
			}
			return;
		}
		std::string msg = Parse_CreateAchievementUpdateJson(ach);
		Talk_SendAchievementProgress(msg);
	}
}