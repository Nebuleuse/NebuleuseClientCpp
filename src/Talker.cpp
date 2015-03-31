#include "CurlWrap.h"
#include "Nebuleuse.h"
#include <thread>

using namespace std;

namespace Neb{
	void Nebuleuse::GetServiceStatus(){
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

	void Nebuleuse::Thread_GetSelfInfos(){
		CurlWrap *c = new CurlWrap();
		
		c->addPost("sessionid", GetSessionID());
		string res = c->fetchPage(CreateUrl("/getUserInfos"), true);

		Parse_SelfInfos(res);
		delete c;
	}

	void Nebuleuse::Thread_SendComplexStats(string data){
		CurlWrap *c = new CurlWrap();

		c->addPost("sessionid", GetSessionID());
		c->addPost("data", data);
		string res = c->fetchPage(CreateUrl("/addComplexStats"), true);

		Parse_Errors(res);
		delete c;
	}

	void Nebuleuse::Thread_SendStatsUpdate(string data){
		CurlWrap *c = new CurlWrap();

		c->addPost("sessionid", GetSessionID());
		c->addPost("data", data);
		string res = c->fetchPage(CreateUrl("/updateStats"), true);

		Parse_Errors(res);
		delete c;
	}

	void Nebuleuse::Thread_SendAchievementProgress(string data){
		CurlWrap *c = new CurlWrap();

		c->addPost("sessionid", GetSessionID());
		c->addPost("data", data);
		string res = c->fetchPage(CreateUrl("/updateAchievements"), true);

		Parse_Errors(res);
		delete c;
	}

	void Nebuleuse::Thread_GetLongPoll(bool reconnecting){
		CurlWrap *c = new CurlWrap();
		int emptyResponses = 0;

		while (!IsUnavailable()){
			c->addPost("sessionid", GetSessionID());
			string res = c->fetchPage(CreateUrl("/longpoll"), true);
			Log("Longpolled");
			Log(res);
			if (res == ""){
				if (reconnecting) // Quick reconnection failed, get new sessionid
					return Connect(_Username, _Password);

				emptyResponses++;
				if (emptyResponses >= MAXPOLLRETRY){
					ThrowError(NEBULEUSE_ERROR_DISCONNECTED, "Lost connection to poll after retries");
					return;
				}
			}
			else {
				emptyResponses = 0;
				bool success = Parse_Messaging(res);
				if (reconnecting && success){
					reconnecting = false;
					SetState(NEBULEUSE_CONNECTED);
				}
			}
		}
		delete c;
	}
	
	void Nebuleuse::Thread_SubscribeTo(string channel){
		CurlWrap *c = new CurlWrap();

		c->addPost("sessionid", GetSessionID());
		c->addPost("channel", channel);
		string res = c->fetchPage(CreateUrl("/subscribeTo"), true);

		Parse_Errors(res);
		delete c;
	}
	void Nebuleuse::Thread_TryReconnectIn(int sec){
		std::this_thread::sleep_for(std::chrono::seconds(sec));
		Thread_GetLongPoll(true);
	}
}