#include "Nebuleuse.h"
#include "Macros.h"

namespace Neb {
	void Nebuleuse::AddUser(uint userid, uint mask){
		User u;
		u.Loaded = false;
		u.Id = userid;
		u.Mask = mask;
		_DataLock.lock();
		_Users.push_back(u);
		_DataLock.unlock();
	}
	void Nebuleuse::RemoveUser(uint userid){
		for (vector<User>::iterator it = _Users.begin(); it != _Users.end(); ++it){
			if (it->Id == userid){
				_DataLock.lock();
				_Users.erase(it);
				_DataLock.unlock();
				break;
			}
		}
	}
	User Nebuleuse::GetUserInfos(uint userid){
		lock_guard<mutex> lock(_DataLock);
		for (vector<User>::iterator it = _Users.begin(); it != _Users.end(); ++it){
			if (it->Id == userid){
				return *it;
			}
		}
		return User();
	}
	User* Nebuleuse::GetUserInfosPtr(uint userid){
		lock_guard<mutex> lock(_DataLock);
		for (vector<User>::iterator it = _Users.begin(); it != _Users.end(); ++it){
			if (it->Id == userid){
				return &it[0];
			}
		}
		return NULL;
	}
	void Nebuleuse::FetchUser(uint userid, uint mask){
		AddUser(userid, mask);
		FetchUsers();
	}
	void Nebuleuse::FetchUsers(){
		lock_guard<mutex> lock(_DataLock);
		for (int i = 0; i < _Users.size(); i++) {
			if (_Users[i].Loaded)
				continue;
			STARTCOMTHREAD(GetUserInfos, _Users[i].Id, _Users[i].Mask)
		}
	}
	void Nebuleuse::FetchInformations(uint userid, uint mask){
		STARTCOMTHREAD(GetUserInfos, userid, mask)
	}
	void Nebuleuse::GetSelfInfos(uint mask){
		STARTCOMTHREAD(GetSelfInfos, mask)
	}
	bool Nebuleuse::HasSelfInfos(uint mask){
		lock_guard<mutex> lock(_DataLock);
		return _Self.Loaded && (_Self.Mask & mask) == mask;

	}
}