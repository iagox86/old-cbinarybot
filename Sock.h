#pragma once

#include <winsock2.h>

typedef void (WINAPI* EventCallbackProc)(void* This, long Event, int Error);

class CSock{
public:
	CSock(void);
	~CSock(void);

	bool Initialise(void* CallbackFunc, unsigned char Events);
	int Connect(void);
	bool Enumerate(void);
	int Send(char *Data, int Length);
	int Recv(char *Data, int Length);
	int Disconnect(void);

	SOCKET GetS() const;

	HANDLE hEvent;
	char *Host;
	unsigned short Port;
	
private:
	bool Resolve(void);
	sockaddr_in* GetName() const;

	SOCKET s;
	sockaddr_in Name;
	EventCallbackProc Callback;
};
