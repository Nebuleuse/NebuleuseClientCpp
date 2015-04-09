#include "Nebuleuse.h"
#include "Macros.h"

namespace Neb {
	void Nebuleuse::AddUser(uint userid, uint mask){
		User u;
		u.Loaded = false;
		u.Id = userid;
		u.Mask = mask;
		_Users.push_back(u);
	}
	void Nebuleuse::RemoveUser(uint userid){
		for (vector<User>::iterator it = _Users.begin(); it != _Users.end(); ++it){
			if (it->Id == userid){
				_Users.erase(it);
				break;
			}
		}
	}
	User Nebuleuse::GetUserInfos(uint userid){
		for (vector<User>::iterator it = _Users.begin(); it != _Users.end(); ++it){
			if (it->Id == userid){
				return *it._Ptr;
			}
		}
	}
	User* Nebuleuse::GetUserInfosPtr(uint userid){
		for (vector<User>::iterator it = _Users.begin(); it != _Users.end(); ++it){
			if (it->Id == userid){
				return it._Ptr;
			}
		}
	}
	void Nebuleuse::FetchUser(uint userid, uint mask){
		AddUser(userid, mask);
		FetchUsers();
	}
	void Nebuleuse::FetchUsers(){
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
		return _Self.Loaded && (_Self.Mask & mask) == mask;
	}
}