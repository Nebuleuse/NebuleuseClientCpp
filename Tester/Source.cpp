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

	while (!connected); // Here we need to wait for the client to be connected to be able to successfully use it

	Neb::Achievement ach = neb->GetAchievement(1);
	ach.Progress = ach.Progress + 1;
	neb->SetAchievement(ach);

	Neb::ComplexStat st;
	st.Name = "kills";
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