#include <iostream>
#include "Nebuleuse.h"

void main(){
	Neb::Nebuleuse *neb = new Neb::Nebuleuse("http://127.0.0.1:8080", 1);
	neb->SetLogCallBack([](std::string l) {
		std::cout << l;
	});
	neb->SetAchievementCallBack([](std::string name){
	});
	neb->SetErrorCallBack([](Neb::NebuleuseError err, std::string Msg){
		std::cout << Msg;
	});
	neb->SetConnectCallback([](bool success){
		std::cout << "Connected";
	});

	if (!neb->Init()){
		return;
	}
	neb->Connect("test", "test");

	Neb::ComplexStat st;
	st.Name = "kills";
	st.AddValue("x", std::to_string(5));
	st.AddValue("y", std::to_string(5));
	st.AddValue("z", std::to_string(5));
	st.AddValue("weapon", "Flower");
	st.AddValue("map", "test");
	neb->AddComplexStat(st);

	system("pause");
}