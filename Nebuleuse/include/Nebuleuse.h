#include <vector>
#include <string>
#include "CurlWrap.h"

#define NEBULEUSEVERSION 1

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
		NEBULEUSE_ERROR_MAINTENANCE,//The service is on maintenance or offline
		NEBULEUSE_ERROR_OUTDATED,//Game is outdated
		NEBULEUSE_ERROR_PARSEFAILED
	};
	enum NebuleusePlayerRank
	{
		NEBULEUSE_USER_RANK_BANNED = 0,
		NEBULEUSE_USER_RANK_NORMAL,
		NEBULEUSE_USER_RANK_DEV,
		NEBULEUSE_URER_RANK_ADMIN
	};

	struct AchievementData
	{
		std::string Name;
		unsigned int Progress;
		unsigned int ProgressMax;
		unsigned int Id;
		bool IsCompleted() { return Progress >= ProgressMax ? true : false; }
		void Complete() { Progress = ProgressMax; }
	};

	struct PlayerStat
	{
		std::string Name;
		int Value;
		bool Changed;
	};
	struct Stat
	{
		Stat(std::string n, std::string v){
			Name = n;
			Value = v;
		}
		std::string Name;
		std::string Value;
	};
	struct ComplexStat
	{
		std::string Name;
		std::vector<Stat> Values;
		void AddValue(std::string name, std::string value){
			Values.push_back(Stat(name, value));
		}
	};

	class Nebuleuse
	{
	public:
		///Create the Nebuleuse.
		Nebuleuse(std::string addr, unsigned int version);
		~Nebuleuse();

		///Start Nebuleuse, true if service is avialable
		bool Init();
		bool Connect(std::string username, std::string password);
		void Disconnect(bool fireCallback = false);

		///Return the current state of Nebuleuse
		std::string getUserName() { return _Username; }
		std::string getPassword() { return _Password; }
		std::string getHost() { return _HostName; };
		std::string GetSessionID() { return _SessionID; };
		std::string GetSubMessage() { return _Motd; }

		NebuleusePlayerRank GetPlayerRank() { return _PlayerRank; }
		bool IsBanned() { return (_PlayerRank == NEBULEUSE_USER_RANK_BANNED); }
		bool IsUnavailable() { return (LastError != NEBULEUSE_ERROR_NONE) || (GetState() == NEBULEUSE_NOTCONNECTED); }
		bool IsOutDated() { return (LastError == NEBULEUSE_ERROR_OUTDATED); }
		void SetState(NebuleuseState state){ _State = state; }
		int GetState(){ return _State; }
		void SetOutDated() { LastError = NEBULEUSE_ERROR_OUTDATED; };

		std::string CreateUrl(std::string path);

		///Get the player stats
		PlayerStat GetPlayerStats(const std::string Name);

		///Set the player stats
		void SetPlayerStats(PlayerStat stat);
		///Send Stats data
		void SendPlayerStats();

		//Add the complex stat to the list
		void AddComplexStat(ComplexStat stat);
		//Send the complex stats to the server
		void SendComplexStats();

		//Achievements

		///Get the specified achievemnt data
		AchievementData GetAchievementData(std::string Name);
		AchievementData GetAchievementData(int index);
		///Set the specified achievement data
		void SetAchievementData(AchievementData data);
		///Update the Progress of this achievement
		void UpdateAchievementProgress(std::string Name, int Progress);
		void UpdateAchievementProgress(int index, int Progress);
		///Earn the achievement
		void UnlockAchievement(std::string Name);
		void UnlockAchievement(int index);
		///Sends the achievements that have been modified
		void SendAchievements();
		///Get the number of achievements
		int GetAchievementCount();

		//Set callbacks
		void SetLogCallBack(void(*Callback)(std::string));
		void SetErrorCallBack(void(*Callback)(NebuleuseError, std::string Msg));
		//Set Achievement CallBack when one is earned
		void SetAchievementCallBack(void(*Callback)(std::string name));
		//Set Callback called when connect() finished
		void SetConnectCallback(void(*Callback)(bool success));
		//Set Callback called when Nebuleuse is Disconnected
		void SetDisconnectCallback(void(*Callback)());

		//Parser
		void Parse_Status(std::string);
		void Parse_Connect(std::string);
		void Parse_UserInfos(std::string);
		void Parse_Errors(std::string);
	private:
		void FinishConnect();

		//Callbacks
		void(*_NebuleuseError_Callback)(NebuleuseError, std::string Msg);
		void(*_NebuleuseLog_Callback)(std::string);
		void(*_AchievementEarned_CallBack)(std::string);
		void(*_Connect_Callback)(bool sucess);
		void(*_Disconnect_Callback)();

		void ThrowError(NebuleuseError e, std::string Msg = "");
		void ThrowError(int e, std::string Msg = "");
		void Log(std::string msg);
		const std::string CreateStatsMsg();

		//Talker
		void Talk_GetServiceStatus();
		void Talk_Connect(std::string username, std::string password);
		void Talk_GetUserInfo();
		void Talk_SendComplexStats(std::string data);
		void Talk_SendAchievementProgress();
		void Talk_SendStatsUpdate(std::string stats);
		void Talk_GetAvatar();

		//Parser
		std::string Parse_CreateComplexStatJson();
	public:
		int LastError;
		CurlWrap *_Curl;

	private:
		std::string _HostName;
		unsigned int _Version;
		std::string _Username;
		std::string _Password;
		std::string _Motd;
		NebuleusePlayerRank _PlayerRank;
		NebuleuseState _State;
		std::string _SessionID;
		std::string _AvatarUrl;

		std::vector<ComplexStat> _CStats;
		std::vector<PlayerStat> _PlayerStats;
		std::vector<AchievementData> _Achievements;
	};
}