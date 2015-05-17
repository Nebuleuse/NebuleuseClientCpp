# Nebuleuse Cpp Client
This library is the official Nebuleuse C++ client. It allows developpers to integrate stats, achievements, matchmaking and more into your games. For more informations about Nebuleuse, please visit the [Nebuleuse Homepage][1].

# Installation & building
- git clone https://github.com/Nebuleuse/NebuleuseClientCpp.git
- git submodule init
- git submodule update
- Build cURL
- Build project solution/makefiles using premake5
- Build the client

#Compatibility
This library is currently Work in Progress. It has been tested using Visual Studio 2013 on Windows but cross-platform compatibility is an important feature.  
Compiler requirement is C++11 support for threads.

#Quick example
```cpp
#include <iostream>
#include "Nebuleuse.h"

Neb::Nebuleuse *neb;
bool connected = false;
void main(){
	neb = new Nebuleuse("http://127.0.0.1:8080", 1);
	neb->SetLogCallBack([](string l) {
		cout << l;
	});
	neb->SetAchievementCallBack([](string name){
	});
	neb->SetErrorCallBack([](NebuleuseError err, string Msg){
		cout << Msg;
	});
	neb->SetConnectCallback([](bool success){
		cout << "Connected\n";
		connected = success;
	});
	neb->SetDisconnectCallback([](){
		cout << "DisConnected\n";
		neb->TryReconnectIn(5);
	});

	if (!neb->Init()){
		return;
	}

	neb->Connect("test", "test");

	while (!connected); // Here we need to wait for the client to be connected to be able to successfully use it
	
	//Getting users informations
	neb->GetSelfInfos(NEBULEUSE_USER_MASK_ALL);
	neb->FetchUser(2, NEBULEUSE_USER_MASK_ALL);

	//Getting and using achievements
	Achievement ach = neb->GetAchievement(1);
	ach.Progress = ach.Progress + 1;
	neb->SetAchievement(ach.Name, ach);

	//Inserting a stat
	ComplexStat st("kills");
	st.AddValue("x", to_string(5));
	st.AddValue("y", to_string(5));
	st.AddValue("z", to_string(5));
	st.AddValue("weapon", "Flower");
	st.AddValue("map", "test");
	neb->AddComplexStat(st);
	neb->SendComplexStats();

	//Getting and setting users' stats
	int val = neb->GetUserStats("level");
	neb->SetUserStats("level", val+1);

	//Subscribe to events
	neb->SubscribeTo("msg");
	
	system("pause");
	return 0;
}
```
[1]:https://nebuleuse.github.io/
