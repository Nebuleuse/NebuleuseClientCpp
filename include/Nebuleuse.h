#pragma once
#include <vector>
#include <string>
#include <map>

#define NEBULEUSEVERSION 1

class CurlWrap;
using namespace std;

namespace Neb{
	enum NebuleuseState
	{
		NEBULEUSE_NOTCONNECTED = 0,
		NEBULEUSE_DISABLED,
		NEBULEUSE_CONNECTED
	};
	enum NebuleuseError
	{
		NEBULEUSE_ERROR_NONE = 0,
		NEBULEUSE_ERROR, //Unspecified error
		NEBULEUSE_ERROR_DISCONNECTED, //The session timed out or never existed
		NEBULEUSE_ERROR_LOGIN, //Error during login
		NEBULEUSE_ERROR_PARTIAL_FAIL, //Error during multiple insertions or updates
		NEBULEUSE_ERROR_MAINTENANCE,//The service is on maintenance or offline
		NEBULEUSE_ERROR_OUTDATED,//Game is outdated
		NEBULEUSE_ERROR_PARSEFAILED
	};
	enum NebuleuseUserRank
	{
		NEBULEUSE_USER_RANK_BANNED = 0,
		NEBULEUSE_USER_RANK_NORMAL,
		NEBULEUSE_USER_RANK_DEV,
		NEBULEUSE_URER_RANK_ADMIN
	};

	struct Achievement
	{
		string Name;
		unsigned int Progress;
		unsigned int ProgressMax;
		unsigned int Id;
		bool Changed;
		bool IsCompleted() { return Progress >= ProgressMax ? true : false; }
		void Complete() { Progress = ProgressMax; }
	};

	struct UserStat
	{
		string Name;
		int Value;
		bool Changed;
	};
	struct ComplexStat
	{
		string Name;
		map<string, string> Values;
		ComplexStat(string n){
			Name = n;
		}
		void AddValue(string name, string value){
			Values[name] = value;
		}
	};
	struct ComplexStatsTableInfos
	{
		string Name;
		vector<string> Fields;
		bool AutoCount;
	};

	class Nebuleuse
	{
	public:
		///Create the Nebuleuse.
		Nebuleuse(string addr, unsigned int version);
		~Nebuleuse();

		///Start Nebuleuse, true if service is avialable
		bool Init();
		//Connect user
		void Connect(string username, string password);
		//Disconnect user
		void Disconnect(bool fireCallback = false);

		///Return the current state of Nebuleuse
		string getUserName()     { return _Username; }
		string getPassword()     { return _Password; }
		string getHost()         { return _HostName; };
		string GetSessionID()    { return _SessionID; };
		int         GetVersion()      { return _Version; }
		int         GetServerVersion(){ return _ServerVersion; }

		NebuleuseUserRank GetUserRank() { return _UserRank; }
		bool IsBanned()      { return (_UserRank == NEBULEUSE_USER_RANK_BANNED); }
		bool IsUnavailable() { return (GetState() == NEBULEUSE_NOTCONNECTED || GetState() == NEBULEUSE_DISABLED); }
		bool IsOutDated()    { return (_LastError == NEBULEUSE_ERROR_OUTDATED); }
		void SetState(NebuleuseState state){ _State = state; }
		int  GetState()      { return _State; }
		void SetOutDated()   { _LastError = NEBULEUSE_ERROR_OUTDATED; };

		//Stats
		///Get the user stats
		int GetUserStats(string Name);
		///Set the user stats
		void SetUserStats(string name, int value);
		
		//Add the complex stat to the list
		void AddComplexStat(ComplexStat stat);
		//Send the complex stats to the server
		void SendComplexStats();

		//Achievements
		///Get the specified achievemnt data
		Achievement GetAchievement(string Name);
		Achievement GetAchievement(int index);
		///Set the specified achievement data
		void SetAchievement(string name, Achievement data);
		///Update the Progress of this achievement
		void UpdateAchievementProgress(string Name, unsigned int Progress);
		void UpdateAchievementProgress(int index, unsigned int Progress);
		///Earn the achievement
		void UnlockAchievement(string Name);
		void UnlockAchievement(int index);
		//Get the number of achievements
		unsigned int GetAchievementCount();

		//Set callbacks
		void SetLogCallBack(void(*Callback)(string));
		void SetErrorCallBack(void(*Callback)(NebuleuseError, string Msg));
		//Set Achievement CallBack when one is earned
		void SetAchievementCallBack(void(*Callback)(string name));
		//Set Callback called when connect() finished
		void SetConnectCallback(void(*Callback)(bool success));
		//Set Callback called when Nebuleuse is Disconnected
		void SetDisconnectCallback(void(*Callback)());
	private:
		string CreateUrl(string path);
		bool VerifyComplexStat(ComplexStat stat);

		void ProceedConnection();
		void FinishConnect();

		void UpdateAchievement(string, unsigned int progress);

		void SendAchievements();
		void SendStats();
		void SendUnsentAchievements();
		void SendUnsentStats();

		//Callbacks
		void(*_NebuleuseError_Callback)(NebuleuseError, string Msg);
		void(*_NebuleuseLog_Callback)(string);
		void(*_AchievementEarned_CallBack)(string);
		void(*_Connect_Callback)(bool sucess);
		void(*_Disconnect_Callback)();

		void ThrowError(NebuleuseError e, string Msg = "");
		void ThrowError(int e, string Msg = "");
		void Log(string msg);

		//Talker
		void Talk_GetServiceStatus();
		void Talk_Connect(string username, string password);
		void Talk_GetUserInfos();
		void Talk_SendComplexStats(string data);
		void Talk_SendAchievementProgress(string data);
		void Talk_SendStatsUpdate(string stats);
		void Talk_GetAvatar();
		//Talker Threads
		void Thread_GetServiceStatus();
		void Thread_Connect(string username, string password);
		void Thread_GetUserInfos();
		void Thread_SendComplexStats(string data);
		void Thread_SendAchievementProgress(string data);
		void Thread_SendStatsUpdate(string stats);
		void Thread_GetAvatar();

		//Parser
		string Parse_CreateComplexStatJson();
		string Parse_CreateChangedAchievementsJson();
		string Parse_CreateChangedStatsJson();
		void Parse_Status(string);
		void Parse_Connect(string);
		void Parse_UserInfos(string);
		void Parse_Errors(string);

	private:
		CurlWrap *_Curl;

		string _HostName;
		unsigned int _Version;
		unsigned int _ServerVersion;
		string _Username;
		string _Password;
		NebuleuseUserRank _UserRank;
		NebuleuseState _State;
		NebuleuseError _LastError;
		string _SessionID;
		string _AvatarUrl;

		vector<ComplexStat> _CStats;
		map<string, UserStat> _UserStats;
		map<string, Achievement> _Achievements;
		map<string, ComplexStatsTableInfos> _CStatsTableInfos;
	};
}