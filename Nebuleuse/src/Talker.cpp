#include "Nebuleuse.h"
#include <thread>

using namespace std;

namespace Neb{
	void getStatus(Nebuleuse *neb){
		neb->_Curl->Lock();
		string res = neb->_Curl->fetchPage(neb->CreateUrl("/status"));
		neb->_Curl->Unlock();

		neb->Parse_Status(res);
	}
	void Nebuleuse::Talk_GetServiceStatus(){
		thread thre(getStatus, this);
		thre.join();
	}

	void getUserInfos(Nebuleuse *neb){
		neb->_Curl->Lock();
		neb->_Curl->addPost("sessionid", neb->GetSessionID());
		string res = neb->_Curl->fetchPage(neb->CreateUrl("/getUserInfos"), true);
		neb->_Curl->Unlock();

		neb->Parse_UserInfos(res);
	}
	void connect(Nebuleuse *neb, string username, string password){
		neb->_Curl->Lock();
		neb->_Curl->addPost("username", username);
		neb->_Curl->addPost("password", password);
		string res = neb->_Curl->fetchPage(neb->CreateUrl("/connect"), true);
		neb->_Curl->Unlock();

		neb->Parse_Connect(res);
		thread thre(getUserInfos, neb);
	}
	void Nebuleuse::Talk_Connect(string username, string password){
		thread thre(connect, this, username, password);
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
	}
}