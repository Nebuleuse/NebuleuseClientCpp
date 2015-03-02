# Nebuleuse Cpp Client
This library is the official Nebuleuse C++ client. It allows developpers to integrate stats, achievements, matchmaking and more into your games. For more informations about Nebuleuse, please visit the [Nebuleuse Homepage][1].

# Installation & building
- git clone https://github.com/Orygin/NebuleuseCppClient.git
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
	neb = new Neb::Nebuleuse("http://127.0.0.1:8080", 1);
	
	neb->SetLogCallBack([](std::string l) {
		std::cout << l;
	});
	neb->SetAchievementCallBack([](std::string name){
	});
	neb->SetErrorCallBack([](Neb::NebuleuseError err, std::string Msg){
		std::cout << Msg;
	});
	neb->SetConnectCallback([](bool success){
		std::cout << "Connected\n";
		connected = success;
	});

	if (!neb->Init()){
		return;
	}
	neb->Connect("test", "test");

// Here we need to wait for the client to be connected to be able to continue.
	while (!connected);

	Neb::Achievement ach = neb->GetAchievement(1);
	ach.Progress = ach.Progress + 1;
	neb->SetAchievement(ach);

	Neb::ComplexStat st("kills");
	st.AddValue("x", std::to_string(5));
	st.AddValue("y", std::to_string(5));
	st.AddValue("z", std::to_string(5));
	st.AddValue("weapon", "Flower");
	st.AddValue("map", "test");
	neb->AddComplexStat(st);

	neb->SendComplexStats();

	int val = neb->GetUserStats("kills");
	neb->SetUserStats("kills", val+1);
	
	system("pause");
}
```
[1]:https://github.com/Orygin/Nebuleuse
