#include <iostream>
#include "Nebuleuse.h"
#include <boost/thread/thread.hpp>

namespace Neb{

	Nebuleuse::Nebuleuse(std::string addr, unsigned int version){
		m_cHostName = addr;
		m_Curl = new CurlWrap();
		SetState(NEBULEUSE_NOTCONNECTED);
		LastError = NEBULEUSE_ERROR_NONE;

		m_uiVersion = version;

		m_iAchievementCount = 0;
		m_iPlayerRank = 0;
		m_iPlayerStatsCount = 0;
	}
	Nebuleuse::~Nebuleuse() {}
	bool Nebuleuse::Init(){
		//Talk_GetServiceStatus();
		return LastError == NEBULEUSE_ERROR_NONE;
	}
	bool Nebuleuse::Connect(std::string username, std::string password){
		m_cUserName = username;
		Talk_Connect(username, password);
		return LastError == NEBULEUSE_ERROR_NONE;
	}

	int Nebuleuse::GetPlayerRank() { return m_iPlayerRank; }
	bool Nebuleuse::IsBanned() { return (m_iPlayerRank == NEBULEUSE_USER_RANK_BANNED); }
	bool Nebuleuse::IsUnavailable()
	{
		return (LastError != NEBULEUSE_ERROR_NONE) || (GetState() == NEBULEUSE_NOTCONNECTED);
	}
	bool Nebuleuse::IsOutDated() { return (LastError == NEBULEUSE_ERROR_OUTDATED); }

	void Nebuleuse::SetState(int state){ m_iState = state; }
	int Nebuleuse::GetState(){ return m_iState; }

	void Nebuleuse::ThrowError(NebuleuseError e)
	{
		LastError = e;
		if (m_NebuleuseError_Callback)
			m_NebuleuseError_Callback(e);
	}
	void Nebuleuse::Log(std::string msg)
	{
		if (m_NebuleuseLog_Callback)
			m_NebuleuseLog_Callback(msg);
	}

	void Nebuleuse::SetLogCallBack(void(*Callback)(std::string))
	{
		if (Callback)
			m_NebuleuseLog_Callback = Callback;
	}
	void Nebuleuse::SetErrorCallBack(void(*Callback)(NebuleuseError))
	{
		if (Callback)
			m_NebuleuseError_Callback = Callback;
	}
	void Nebuleuse::SetAchievementCallBack(void(*Callback)(int))
	{
		if (Callback)
			m_AchievementEarned_CallBack = Callback;
	}
	void Nebuleuse::SetCurrentMap(const std::string map)
	{
		m_sCurMap = map;
	}
}