#include "server.h"

CRITICAL_SECTION cs;

int main(int argc, char *argv[])
{
	InitializeCriticalSection(&cs);
	// 접속부분
	
	if (Server::GetInstace()->Init() == false) {
		return -1;
	}
	cout << "\nServer Init complete.." << endl;
	if (Server::GetInstace()->Bind() == false) {
		return -1;
	}
	cout << "\nServer Bind complete.." << endl;
	if (Server::GetInstace()->Listen() == false) {
		return -1;
	}
	cout << "\nServer Listen complete" << endl;
	cout << "\nServer Accept start.." << endl;
	while (1) {
		if (Server::GetInstace()->Accept() == false) {
			continue;
		}
	}

	Server::GetInstace()->Release();
	cout << "\nServer Release complete.." << endl;

	return 0;
}