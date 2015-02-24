#include "CurlWrap.h"
#include "Nebuleuse.h"
#include <thread>

using namespace std;

namespace Neb{
	void Nebuleuse::Talk_GetServiceStatus(){
		_Curl->Lock();
		string res = _Curl->fetchPage(CreateUrl("/status"));
		_Curl->Unlock();

		Parse_Status(res);
	}

	void getUserInfos(Nebuleuse *neb){
		neb->_Curl->Lock();
		neb->_Curl->addPost("sessionid", neb->GetSessionID());
		string res = neb->_Curl->fetchPage(neb->CreateUrl("/getUserInfos"), true);
		neb->_Curl->Unlock();

		neb->Parse_UserInfos(res);
	}
	void Nebuleuse::Talk_GetUserInfos(){
		thread thre(getUserInfos, this);
		thre.detach();
	}
	
	void connect(Nebuleuse *neb, string username, string password){
		neb->_Curl->Lock();
		neb->_Curl->addPost("username", username);
		neb->_Curl->addPost("password", password);
		string res = neb->_Curl->fetchPage(neb->CreateUrl("/connect"), true);
		neb->_Curl->Unlock();

		if(neb->Parse_Connect(res))
			neb->ProceedConnection();
	}
	void Nebuleuse::Talk_Connect(string username, string password){
		thread thre(connect, this, username, password);
		thre.detach();
	}

	void sendComplexStats(Nebuleuse * neb, string data){
		neb->_Curl->Lock();
		neb->_Curl->addPost("sessionid", neb->GetSessionID());
		neb->_Curl->addPost("data", data);
		string res = neb->_Curl->fetchPage(neb->CreateUrl("/addComplexStats"), true);
		neb->_Curl->Unlock();

		neb->Parse_Errors(res);
	}
	void Nebuleuse::Talk_SendComplexStats(string data){
		thread thre(sendComplexStats, this, data);
		thre.detach();
	}
	void sendStats(Nebuleuse *neb, string data){
		neb->_Curl->Lock();
		neb->_Curl->addPost("sessionid", neb->GetSessionID());
		neb->_Curl->addPost("data", data);
		string res = neb->_Curl->fetchPage(neb->CreateUrl("/updateStats"), true);
		neb->_Curl->Unlock();

		neb->Parse_Errors(res);
	}
	void Nebuleuse::Talk_SendStatsUpdate(string data){
		thread thre(sendStats, this, data);
		thre.detach();
	}

	void sendAchievements(Nebuleuse *neb, string data){
		neb->_Curl->Lock();
		neb->_Curl->addPost("sessionid", neb->GetSessionID());
		neb->_Curl->addPost("data", data);
		string res = neb->_Curl->fetchPage(neb->CreateUrl("/updateAchievements"), true);
		neb->_Curl->Unlock();

		neb->Parse_Errors(res);
	}
	void Nebuleuse::Talk_SendAchievementProgress(string data){
		thread thre(sendAchievements, this, data);
		thre.detach();
	}
}