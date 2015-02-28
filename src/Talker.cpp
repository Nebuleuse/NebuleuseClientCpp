#include "CurlWrap.h"
#include "Nebuleuse.h"
#include <thread>

using namespace std;

namespace Neb{
	void Nebuleuse::Talk_GetServiceStatus(){
		_Curl->Lock();
		string res = _Curl->fetchPage(CreateUrl("/status"));
		_Curl->Unlock();
		Log(res);
		Parse_Status(res);
	}

	void Nebuleuse::Thread_GetServiceStatus(){
		_Curl->Lock();
		_Curl->addPost("sessionid", GetSessionID());
		string res = _Curl->fetchPage(CreateUrl("/getUserInfos"), true);
		_Curl->Unlock();

		Parse_UserInfos(res);
	}
	void Nebuleuse::Talk_GetUserInfos(){
		thread thre(&Nebuleuse::Thread_GetServiceStatus, this);
		thre.detach();
	}

	void Nebuleuse::Thread_Connect(string username, string password){
		_Curl->Lock();
		_Curl->addPost("username", username);
		_Curl->addPost("password", password);
		string res = _Curl->fetchPage(CreateUrl("/connect"), true);
		_Curl->Unlock();

		Parse_Connect(res);
	}
	void Nebuleuse::Talk_Connect(string username, string password){
		thread thre(&Nebuleuse::Thread_Connect, this, username, password);
		thre.detach();
	}

	void Nebuleuse::Thread_SendComplexStats(string data){
		_Curl->Lock();
		_Curl->addPost("sessionid", GetSessionID());
		_Curl->addPost("data", data);
		string res = _Curl->fetchPage(CreateUrl("/addComplexStats"), true);
		_Curl->Unlock();

		Parse_Errors(res);
	}
	void Nebuleuse::Talk_SendComplexStats(string data){
		thread thre(&Nebuleuse::Thread_SendComplexStats, this, data);
		thre.detach();
	}
	void Nebuleuse::Thread_SendStatsUpdate(string data){
		_Curl->Lock();
		_Curl->addPost("sessionid", GetSessionID());
		_Curl->addPost("data", data);
		string res = _Curl->fetchPage(CreateUrl("/updateStats"), true);
		_Curl->Unlock();

		Parse_Errors(res);
	}
	void Nebuleuse::Talk_SendStatsUpdate(string data){
		thread thre(&Nebuleuse::Thread_SendStatsUpdate, this, data);
		thre.detach();
	}

	void Nebuleuse::Thread_SendAchievementProgress(string data){
		_Curl->Lock();
		_Curl->addPost("sessionid", GetSessionID());
		_Curl->addPost("data", data);
		string res = _Curl->fetchPage(CreateUrl("/updateAchievements"), true);
		_Curl->Unlock();

		Parse_Errors(res);
	}
	void Nebuleuse::Talk_SendAchievementProgress(string data){
		thread thre(&Nebuleuse::Thread_SendAchievementProgress, this, data);
		thre.detach();
	}
}