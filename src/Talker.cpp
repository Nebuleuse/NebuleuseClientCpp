#include "CurlWrap.h"
#include "Nebuleuse.h"
#include <thread>

using namespace std;

namespace Neb{
	void Nebuleuse::GetServiceStatus(){
		CurlWrap c;


		string res = c.fetchPage(CreateUrl("/status"));

		Parse_Status(res);
	}

	void Nebuleuse::Thread_Connect(string username, string password){
		CurlWrap c;

		c.addPost("username", username);
		c.addPost("password", password);
		string res = c.fetchPage(CreateUrl("/connect"), true);

		bool success = Parse_Connect(res);
		ProceedConnection(success);
		
	}

	void Nebuleuse::Thread_GetSelfInfos(uint mask){
		CurlWrap c;
		
		c.addPost("sessionid", GetSessionID());
		c.addPost("infomask", to_string(mask));
		string res = c.fetchPage(CreateUrl("/getUserInfos"), true);

		Parse_SelfInfos(res);
		
	}

	void Nebuleuse::Thread_GetUserInfos(uint userid, uint mask){
		CurlWrap c;

		c.addPost("userid", to_string(userid));
		c.addPost("infomask", to_string(mask));
		string res = c.fetchPage(CreateUrl("/getUserInfos"), true);

		Parse_UserInfos(res, userid);
	}

	void Nebuleuse::Thread_SendComplexStats(string data){
		CurlWrap c;

		c.addPost("sessionid", GetSessionID());
		c.addPost("data", data);
		string res = c.fetchPage(CreateUrl("/addComplexStats"), true);

		Parse_Errors(res);
		
	}

	void Nebuleuse::Thread_SendStatsUpdate(string data){
		CurlWrap c;

		c.addPost("sessionid", GetSessionID());
		c.addPost("data", data);
		string res = c.fetchPage(CreateUrl("/setUserStats"), true);

		Parse_Errors(res);
		
	}

	void Nebuleuse::Thread_SendAchievementProgress(string data){
		CurlWrap c;

		c.addPost("sessionid", GetSessionID());
		c.addPost("data", data);
		string res = c.fetchPage(CreateUrl("/setUserAchievements"), true);

		Parse_Errors(res);
		
	}

	void Nebuleuse::Thread_GetLongPoll(bool reconnecting){
		CurlWrap c;
		int emptyResponses = 0;

		while (!IsUnavailable() || reconnecting){
			c.addPost("sessionid", GetSessionID());
			string res = c.fetchPage(CreateUrl("/longpoll"), true);
			Log("Longpolled");
			Log(res);
			if (c.lastError() != ""){
				ThrowError(NEBULEUSE_ERROR_DISCONNECTED, c.lastError());
				return;
			}
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
		
	}

	void Nebuleuse::Thread_SubscribeTo(string channel){
		CurlWrap c;

		c.addPost("sessionid", GetSessionID());
		c.addPost("channel", channel);
		string res = c.fetchPage(CreateUrl("/subscribeTo"), true);

		Parse_Errors(res);

	}

	void Nebuleuse::Thread_UnSubscribeTo(string channel){
		CurlWrap c;

		c.addPost("sessionid", GetSessionID());
		c.addPost("channel", channel);
		string res = c.fetchPage(CreateUrl("/unSubscribeTo"), true);

		Parse_Errors(res);

	}

	void Nebuleuse::Thread_TryReconnectIn(int sec){
		std::this_thread::sleep_for(std::chrono::seconds(sec));
		Thread_GetLongPoll(true);
	}
}