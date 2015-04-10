#include <iostream>
#include "Nebuleuse.h"

using namespace Neb;
using namespace std;

Nebuleuse *neb;
bool connected = false;
int main(){
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
		while (cin.get() != '\n')
		return 0;
	}
	neb->Connect("test2", "test");

	while (!connected); // Here we need to wait for the client to be connected to be able to successfully use it
	
	neb->GetSelfInfos(NEBULEUSE_USER_MASK_ALL);
	while (!neb->HasSelfInfos(NEBULEUSE_USER_MASK_BASE));
	neb->FetchUser(2, NEBULEUSE_USER_MASK_ALL);

	/*Achievement ach = neb->GetAchievement(1);
	ach.Progress = ach.Progress + 1;
	neb->SetAchievement(ach.Name, ach);

	ComplexStat st("kills");
	st.AddValue("x", to_string(5));
	st.AddValue("y", to_string(5));
	st.AddValue("z", to_string(5));
	st.AddValue("weapon", "Flower");
	st.AddValue("map", "test");
	neb->AddComplexStat(st);

	neb->SendComplexStats();

	int val = neb->GetUserStats("kills");
	neb->SetUserStats("kills", val+1);*/

	neb->SubscribeTo("msg");
	while (true);
	system("pause");
	return 0;
}