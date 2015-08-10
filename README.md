# Nebuleuse Cpp Client
This library is the official Nebuleuse C++ client. It allows developers to integrate stats, achievements, matchmaking and more into your games. For more informations about Nebuleuse, please visit the [Nebuleuse Homepage][1].

## Installation & building
- git clone https://github.com/Nebuleuse/NebuleuseClientCpp.git
- git submodule init
- git submodule update
- Build cURL
- Build project solution/makefiles using premake5
- Build the library
- Add and link library to your project

## Compatibility
This library is currently Work in Progress, following Nebuleuse backend changes. Support for VS2013 and GCC has been tested. Building requires C++11 support.

## Unreal Engine 4 integration
This client is compatible with Epic's Unreal Engine 4. You can follow [this guide](https://wiki.unrealengine.com/Linking_Static_Libraries_Using_The_Build_System) for the steps needed to integrate the library into your project. The file *UE4Sample.Build.cs* contains a sample of what your build configuration file should look like.  
This project currently does not offer a wrapper for Blueprint usage so you will have to make your own.  

## Quick example
This sample will show you most of Nebuleuse features used
```cpp
#include <iostream>
#include "Nebuleuse.h"

Neb::Nebuleuse *neb;
bool connected = false;
void main(){
	//Create Nebuleuse
	neb = new Nebuleuse("http://127.0.0.1:8080", 1);

	//Set callbacks
	neb->SetLogCallBack([](string l) {
		cout << l;
	});
	neb->SetAchievementCallBack([](string name){
		cout << "Achievement unlocked: " << l;
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
		//No connection could be made to the server
		return;
	}

	neb->Connect("test", "test");

 	//In this example, we need to wait for the client to be connected to continue
	while (!connected);
	
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
	//You can add multiple stat and send them together
	neb->SendComplexStats();

	//Getting and setting users' stats
	int val = neb->GetUserStats("level");
	neb->SetUserStats("level", val+1);

	//Subscribe to events
	neb->SubscribeTo("msg");
}
```
[1]:https://nebuleuse.github.io/
