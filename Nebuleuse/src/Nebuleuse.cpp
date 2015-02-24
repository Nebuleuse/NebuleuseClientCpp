#include <iostream>
#include "CurlWrap.h"
#include "Nebuleuse.h"

namespace Neb{
	Nebuleuse::Nebuleuse(std::string addr, unsigned int version){
		_HostName = addr;
		_Curl = new CurlWrap();
		SetState(NEBULEUSE_NOTCONNECTED);
		LastError = NEBULEUSE_ERROR_NONE;

		_Version = version;

		_UserRank = NEBULEUSE_USER_RANK_NORMAL;
		_NebuleuseError_Callback = NULL;
		_NebuleuseLog_Callback = NULL;
		_AchievementEarned_CallBack = NULL;
	}

	Nebuleuse::~Nebuleuse() {}

	bool Nebuleuse::Init(){
		Talk_GetServiceStatus();
		return LastError == NEBULEUSE_ERROR_NONE;
	}

	void Nebuleuse::Connect(std::string username, std::string password){
		if (_Username != username){ //New user, wipe older infos
			_Achievements.clear();
			_UserStats.clear();
			_CStats.clear();
		}

		_Username = username;
		_Password = password;
		Talk_Connect(username, password);
	}
	void Nebuleuse::ProceedConnection(){
		SetState(NEBULEUSE_CONNECTED); //We got a valid SessionId

		//If we got disconnected before, changed data may need to be sent
		SendAchievements();
		SendStats();
		SendComplexStats();

		//Get fresh user Infos
		Talk_GetUserInfos();
	}
	void Nebuleuse::FinishConnect(){
		if (_Connect_Callback)
			_Connect_Callback(_State == NEBULEUSE_CONNECTED);
	}

	void Nebuleuse::Disconnect(bool fireCallback){
		SetState(NEBULEUSE_NOTCONNECTED);
		if (_Disconnect_Callback)
			_Disconnect_Callback();
	}

	void Nebuleuse::ThrowError(int e, std::string Msg){
		ThrowError(static_cast<NebuleuseError>(e), Msg);
	}
	void Nebuleuse::ThrowError(NebuleuseError e, std::string Msg){
		if (e == NEBULEUSE_ERROR_NONE)
			return;
		else if (e == NEBULEUSE_ERROR_DISCONNECTED)
			Disconnect();
		else if(e == NEBULEUSE_ERROR_MAINTENANCE || e == NEBULEUSE_ERROR_OUTDATED)
			SetState(NEBULEUSE_DISABLED);

		LastError = e;
		if (_NebuleuseError_Callback)
			_NebuleuseError_Callback(e, Msg);
	}

	void Nebuleuse::Log(std::string msg){
		if (_NebuleuseLog_Callback)
			_NebuleuseLog_Callback(msg);
	}

	void Nebuleuse::SetLogCallBack(void(*Callback)(std::string)){
		_NebuleuseLog_Callback = Callback;
	}
	void Nebuleuse::SetErrorCallBack(void(*Callback)(NebuleuseError, std::string Msg)){
		_NebuleuseError_Callback = Callback;
	}
	void Nebuleuse::SetAchievementCallBack(void(*Callback)(std::string)){
		_AchievementEarned_CallBack = Callback;
	}
	void Nebuleuse::SetConnectCallback(void(*Callback)(bool)){
		_Connect_Callback = Callback;
	}
	void Nebuleuse::SetDisconnectCallback(void(*Callback)()){
		_Disconnect_Callback = Callback;
	}

	std::string Nebuleuse::CreateUrl(std::string path){
		std::string url;
		url.append(getHost());
		url.append(path);
		return url;
	}
}