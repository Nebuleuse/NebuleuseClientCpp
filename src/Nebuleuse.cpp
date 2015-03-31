#include <iostream>
#include "CurlWrap.h"
#include "Nebuleuse.h"
#include "Macros.h"

namespace Neb{
	Nebuleuse::Nebuleuse(std::string addr, unsigned int version){
		CurlWrap::init();
		_HostName = addr;
		_Self = User();
		SetState(NEBULEUSE_NOTCONNECTED);
		_LastError = NEBULEUSE_ERROR_NONE;

		_Version = version;

		_Self.Rank = NEBULEUSE_USER_RANK_NORMAL;
		_NebuleuseError_Callback = NULL;
		_NebuleuseLog_Callback = NULL;
		_AchievementEarned_CallBack = NULL;
	}

	Nebuleuse::~Nebuleuse() {
		_Self.Achievements.clear();
		_Self.Stats.clear();
		_CStats.clear();
	}

	bool Nebuleuse::Init(){
		STARTCOMTHREAD(GetServiceStatus)
		return _LastError == NEBULEUSE_ERROR_NONE;
	}

	void Nebuleuse::Connect(std::string username, std::string password){
		if (_Username != username){ //New user, wipe older infos
			_Self.Achievements.clear();
			_Self.Stats.clear();
			_CStats.clear();
		}

		_Username = username;
		_Password = password;
		Talk_Connect(username, password);
	}
	//Called once we are connected to start longpoll and user data get
	void Nebuleuse::ProceedConnection(bool success){
		SetState(success ? NEBULEUSE_CONNECTED : NEBULEUSE_NOTCONNECTED); //We got a valid SessionId

		if (success){
			//If we got disconnected before, changed data may need to be sent
			SendAchievements();
			SendStats();
			SendComplexStats();

			//Subscribe to messages
			STARTCOMTHREAD(GetLongPoll)
		}

		// We have finished
		if (_Connect_Callback)
			_Connect_Callback(success);
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

		_LastError = e;
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