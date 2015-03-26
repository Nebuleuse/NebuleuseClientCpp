#include "CurlWrap.h"
#include "Nebuleuse.h"
#include <thread>

using namespace std;

namespace Neb{
	void Nebuleuse::Talk_GetServiceStatus(){
		CurlWrap *c = new CurlWrap();


		string res = c->fetchPage(CreateUrl("/status"));

		Log(res);
		Parse_Status(res);
		delete c;
	}

	void Nebuleuse::Thread_Connect(string username, string password){
		CurlWrap *c = new CurlWrap();

		c->addPost("username", username);
		c->addPost("password", password);
		string res = c->fetchPage(CreateUrl("/connect"), true);

		bool success = Parse_Connect(res);
		ProceedConnection(success);
		delete c;
	}
	void Nebuleuse::Talk_Connect(string username, string password){
		thread thre(&Nebuleuse::Thread_Connect, this, username, password);
		thre.detach();
	}

	void Nebuleuse::Thread_GetSelfInfos(){
		CurlWrap *c = new CurlWrap();
		
		c->addPost("sessionid", GetSessionID());
		string res = c->fetchPage(CreateUrl("/getUserInfos"), true);

		Parse_SelfInfos(res);
		delete c;
	}
	void Nebuleuse::Talk_GetSelfInfos(){
		thread thre(&Nebuleuse::Thread_GetSelfInfos, this);
		thre.detach();
	}

	void Nebuleuse::Thread_SendComplexStats(string data){
		CurlWrap *c = new CurlWrap();

		c->addPost("sessionid", GetSessionID());
		c->addPost("data", data);
		string res = c->fetchPage(CreateUrl("/addComplexStats"), true);

		Parse_Errors(res);
		delete c;
	}
	void Nebuleuse::Talk_SendComplexStats(string data){
		thread thre(&Nebuleuse::Thread_SendComplexStats, this, data);
		thre.detach();
	}
	void Nebuleuse::Thread_SendStatsUpdate(string data){
		CurlWrap *c = new CurlWrap();

		c->addPost("sessionid", GetSessionID());
		c->addPost("data", data);
		string res = c->fetchPage(CreateUrl("/updateStats"), true);

		Parse_Errors(res);
		delete c;
	}
	void Nebuleuse::Talk_SendStatsUpdate(string data){
		thread thre(&Nebuleuse::Thread_SendStatsUpdate, this, data);
		thre.detach();
	}

	void Nebuleuse::Thread_SendAchievementProgress(string data){
		CurlWrap *c = new CurlWrap();

		c->addPost("sessionid", GetSessionID());
		c->addPost("data", data);
		string res = c->fetchPage(CreateUrl("/updateAchievements"), true);

		Parse_Errors(res);
		delete c;
	}
	void Nebuleuse::Talk_SendAchievementProgress(string data){
		thread thre(&Nebuleuse::Thread_SendAchievementProgress, this, data);
		thre.detach();
	}
	void Nebuleuse::Thread_GetLongPoll(){
		CurlWrap *c = new CurlWrap();
		while (!IsUnavailable()){
			c->addPost("sessionid", GetSessionID());
			string res = c->fetchPage(CreateUrl("/longpoll"), true);

			Log(res);
		}
		delete c;
	}
	void Nebuleuse::Talk_GetLongPoll(){
		thread thre(&Nebuleuse::Thread_GetLongPoll, this);
		thre.detach();
	}
}