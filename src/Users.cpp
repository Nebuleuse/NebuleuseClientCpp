#include "Nebuleuse.h"

namespace Neb {
	void Nebuleuse::AddUser(uint userid, uint){
		User u;
		u.Loaded = false;
		u.UserID = userid;
		_Users.push_back(u);
	}
}