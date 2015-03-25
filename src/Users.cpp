#include "Nebuleuse.h"

namespace Neb {
	void Nebuleuse::AddUser(uint userid, uint){
		User u;
		u.Loaded = false;
		u.Id = userid;
		_Users.push_back(u);
	}
	void Nebuleuse::GetSelfInfos(int mask){
		Talk_GetSelfInfos();
	}
	bool Nebuleuse::HasSelfInfos(int mask){
		return (_Self.AvialableInfos & mask) == mask;
	}
}