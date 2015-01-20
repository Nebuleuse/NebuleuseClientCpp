#include <vector>
#include <string>
#include "CurlWrap.h"

#define NEBULEUSEVERSION 1

namespace Neb{
	enum NebuleuseState
	{
		NEBULEUSE_NOTCONNECTED = 0,
		NEBULEUSE_DISABLED,
		NEBULEUSE_CONNECTED,
		NEBULEUSE_WORKING
	};
	enum NebuleuseError
	{
		NEBULEUSE_ERROR_NONE = 0,
		NEBULEUSE_ERROR, //Unspecified error
		NEBULEUSE_ERROR_DISCONNECTED, //The session timed out or never existed
		NEBULEUSE_ERROR_LOGIN, //Error during login
		NEBULEUSE_ERROR_BANNED,
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
		char *Name;
		unsigned int Progress;
		unsigned int ProgressMax;
		unsigned int Id;
		bool IsCompleted() { return Progress >= ProgressMax ? true : false; }
		void Complete() { Progress = ProgressMax; }
	};

	struct PlayerStat
	{
		char *Name;
		int Value;
		bool Changed;
	};

	struct KillInfo
	{
	public:
		KillInfo(const char *weapon, int x, int y, int z) : weapon(weapon), x(x), y(y), z(z)
		{
		}
		~KillInfo()
		{
			delete weapon;
		}
		const char *getWeapon() const
		{
			return weapon;
		}
		void getPos(int& rx, int& ry, int& rz) const
		{
			rx = x;
			ry = y;
			rz = z;
		}
	private:
		const char* weapon;
		const int x, y, z;
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

		///Return the current state of Nebuleuse
		int GetState();
		void SetState(int state);
		bool IsBanned();
		bool IsUnavailable();
		bool IsOutDated();

		std::string getUserName() { return m_cUserName; }
		const std::string getHost() { return m_cHostName; };
		std::string GetSessionID() { return m_cSessionID; };
		void SetCurrentMap(const std::string map);
		void SetOutDated() { LastError = NEBULEUSE_ERROR_OUTDATED; };

		int GetPlayerRank();

		///Get the player stats
		PlayerStat GetPlayerStats(const std::string Name);

		///Set the player stats
		void SetPlayerStats(PlayerStat stat);
		///Send Stats data
		void SendPlayerStats();

		///Add a Kill to the list
		void AddKill(KillInfo *info);

		///Get the specified achievemnt data
		AchievementData GetAchievementData(std::string Name);
		AchievementData GetAchievementData(int index);
		///Set the specified achievement data
		void SetAchievementData(AchievementData data);
		///Update the Progress of this achievement (and send it)
		void UpdateAchievementProgress(std::string Name, int Progress);
		void UpdateAchievementProgress(int index, int Progress);
		///Earn the achievement (and send it)
		void UnlockAchievement(std::string Name);
		void UnlockAchievement(int index);
		///Sends the achievements that have been modified
		void SendAchievements();
		///Achievement earn CallBack
		void SetAchievementCallBack(void(*Callback)(int));
		///Return the number of achievements
		int GetAchievementCount() { return m_iAchievementCount; }
		std::string GetSubMessage() { return m_cMotd; }
		void SetLogCallBack(void(*Callback)(std::string));
		void CustomTalk(char* Url, void(*Callback)(char*));
		void(*m_CustomTalk_Callback)(char *);

		//Parser
		void Parse_Status(std::string);
		void Parse_Connect(std::string);
		void Parse_SessionData(std::string data);
		void Parse_RcvErrorCode(int code);
	private:
		void SetErrorCallBack(void(*Callback)(NebuleuseError));
		void(*m_NebuleuseError_Callback)(NebuleuseError);
		void(*m_NebuleuseLog_Callback)(std::string);
		void(*m_AchievementEarned_CallBack)(int);
		void ThrowError(NebuleuseError e);
		void Log(std::string msg);
		const std::string CreateStatsMsg();

		//Talker
		void Talk_GetServiceStatus();
		void Talk_Connect(std::string username, std::string password);
		void Talk_GetStatData();
		void Talk_GetNewSession();
		void Talk_SendAchievementProgress(int id);
		void Talk_SendStatsUpdate(const std::string stats);
		void Talk_GetAvatar();
		size_t RcvConnectInfos(void *ptr, size_t size, size_t nmemb, void *userdata);
		//size_t RcvSessionInfos( void *ptr, size_t size, size_t nmemb, void *userdata);
	public:
		int LastError;
		char *m_ConnectAns;
		CurlWrap *m_Curl;

	private:
		std::vector<PlayerStat> m_PlayerStats;
		int m_iPlayerStatsCount;
		std::vector<AchievementData> m_Achievements;
		int m_iAchievementCount;

		std::string m_cHostName;
		unsigned int m_uiVersion;
		std::string m_cUserName;
		std::string m_cMotd;
		int m_iPlayerRank;
		int m_iState;
		std::string m_cSessionID;

		std::vector<KillInfo*> m_vKills;
		std::string m_sCurMap;
	};
}