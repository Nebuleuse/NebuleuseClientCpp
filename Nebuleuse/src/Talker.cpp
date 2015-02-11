#include "Nebuleuse.h"
#include <boost/thread/thread.hpp>

namespace Neb{
	void getStatus(Nebuleuse *neb){
		neb->_Curl->Lock();
		std::string res = neb->_Curl->fetchPage(neb->CreateUrl("/status"));
		neb->_Curl->Unlock();

		neb->Parse_Status(res);
	}
	void Nebuleuse::Talk_GetServiceStatus(){
		boost::thread thre(getStatus, this);
		thre.join();
	}
	void getUserInfos(Nebuleuse *neb){
		neb->_Curl->Lock();
		neb->_Curl->addPost("sessionid", neb->GetSessionID());
		std::string res = neb->_Curl->fetchPage(neb->CreateUrl("/getUserInfos"), true);
		neb->_Curl->Unlock();

		neb->Parse_UserInfos(res);
	}
	void connect(Nebuleuse *neb, std::string username, std::string password){
		neb->_Curl->Lock();
		neb->_Curl->addPost("username", username);
		neb->_Curl->addPost("password", password);
		std::string res = neb->_Curl->fetchPage(neb->CreateUrl("/connect"), true);
		neb->_Curl->Unlock();

		neb->Parse_Connect(res);
		boost::thread thre(getUserInfos, neb);
	}
	void Nebuleuse::Talk_Connect(std::string username, std::string password){
		boost::thread thre(connect, this, username, password);
	}
	void sendComplexStats(Nebuleuse * neb, std::string data){
		neb->_Curl->Lock();
		neb->_Curl->addPost("sessionid", neb->GetSessionID());
		neb->_Curl->addPost("data", data);
		std::string res = neb->_Curl->fetchPage(neb->CreateUrl("/addComplexStats"), true);
		neb->_Curl->Unlock();

		neb->Parse_Errors(res);
	}
	void Nebuleuse::Talk_SendComplexStats(std::string data){
		boost::thread thre(sendComplexStats, this, data);
	}
	void sendAchievements(Nebuleuse *neb, std::string data){
		neb->_Curl->Lock();
		neb->_Curl->addPost("sessionid", neb->GetSessionID());
		neb->_Curl->addPost("data", data);
		std::string res = neb->_Curl->fetchPage(neb->CreateUrl("/updateAchievements"), true);
		neb->_Curl->Unlock();

		neb->Parse_Errors(res);
	}
	void Nebuleuse::Talk_SendAchievementProgress(std::string data){
		boost::thread thre(sendAchievements, this, data);
	}
}