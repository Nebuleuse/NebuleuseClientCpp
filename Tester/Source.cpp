#include <iostream>
#include "Nebuleuse.h"

void main(){
	Neb::Nebuleuse *neb = new Neb::Nebuleuse("http://127.0.0.1:8080", 1);
	neb->Init();
	neb->Connect("test", "test");

	system("pause");
}