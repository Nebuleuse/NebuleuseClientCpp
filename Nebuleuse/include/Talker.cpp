#include "Nebuleuse.h"

namespace Neb{
	void getStatus(Nebuleuse *neb){
		std::string url;
		url.append(neb->getHost());
		url.append("/status");

		neb->m_Curl->Lock();
		std::string res = neb->m_Curl->fetchPage(url);
		neb->m_Curl->Unlock();

		neb->Parse_Status(res);
	}
	void Nebuleuse::Talk_GetServiceStatus(){
		boost::thread thre(getStatus, this);
		thre.join();
	}
	void connect(Nebuleuse *neb, std::string username, std::string password){
		std::string url;
		url.append(neb->getHost());
		url.append("/connect");

		neb->m_Curl->Lock();
		neb->m_Curl->addPost("username", username);
		neb->m_Curl->addPost("password", password);
		std::string res = neb->m_Curl->fetchPage(url, true);
		neb->m_Curl->Unlock();

		neb->Parse_Connect(res);
	}
	void Nebuleuse::Talk_Connect(std::string username, std::string password){
		boost::thread thre(connect, this, username, password);
	}
}