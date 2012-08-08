#include "sock.h"

/*
bool __stdcall EventCallback(void* This, long Event, int Error){
	
	char Buffer[1024];
	void *ErrorMsg;
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, Error, 0, (char*)&ErrorMsg, 0, NULL);

	switch(Event){
		case FD_READ:
			sprintf((char*)&Buffer, "Event: FD_READ (Error: %s)", ErrorMsg);
			break;
		case FD_WRITE:
			sprintf((char*)&Buffer, "Event: FD_WRITE (Error: %s)", ErrorMsg);
			break;
		case FD_OOB:
			sprintf((char*)&Buffer, "Event: FD_OOB (Error: %s)", ErrorMsg);
			break;
		case FD_ACCEPT:
			sprintf((char*)&Buffer, "Event: FD_ACCEPT (Error: %s)", ErrorMsg);
			break;
		case FD_CONNECT:
			BBot
			sprintf((char*)&Buffer, "Event: FD_CONNECT (Error: %s)", ErrorMsg);
			break;
		case FD_CLOSE:
			sprintf((char*)&Buffer, "Event: FD_CLOSE (Error: %s)", ErrorMsg);
			break;
		case FD_QOS:
			sprintf((char*)&Buffer, "Event: FD_QOS (Error: %s)", ErrorMsg);
			break;
		case FD_GROUP_QOS:
			sprintf((char*)&Buffer, "Event: FD_GROUP_QOS (Error: %s)", ErrorMsg);
			break;
		case FD_ROUTING_INTERFACE_CHANGE:
			sprintf((char*)&Buffer, "Event: FD_ROUTING_INTERFACE_CHANGE (Error: %s)", ErrorMsg);
			break;
		case FD_ADDRESS_LIST_CHANGE:
			sprintf((char*)&Buffer, "Event: FD_ADDRESS_LIST_CHANGE (Error: %s)", ErrorMsg);
			break;
	}

	MessageBox(NULL, (char*)&Buffer, "Socket Event", MB_APPLMODAL);

	return false;
}
*/

CSock::CSock(void){
	// Start winsock
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 1), &wsadata);

	s = NULL;
}

CSock::~CSock(void){
	WSACleanup();
}

bool CSock::Initialise(void* CallbackFunc, unsigned char Events){
	
	if(s){
		closesocket(s);
	}

	// Create socket
	s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(!s){
		return false;
	}
	
	// Create event
	hEvent = CreateEvent(0, 0, 0, 0);
	if(WSAEventSelect(s, hEvent, Events) == SOCKET_ERROR){
		return false;
	}
	
	Callback = (EventCallbackProc)CallbackFunc;

	return true;
}

int CSock::Connect(void){
	
	// Make sure Host isn't null. Empty hostname is caught in Resolve()
	if(!Host){
		WSASetLastError(WSAHOST_NOT_FOUND);
		return -1;
	}

	Resolve();

	// Connect
	int Ret = connect(s, (sockaddr*)&Name, sizeof(Name));

	if(Ret == SOCKET_ERROR){
		if(WSAGetLastError() != WSAEWOULDBLOCK){
			return SOCKET_ERROR;
		}
	}

	return 0;
}

int CSock::Send(char *Data, int Length){
	
	return send(s, Data, Length, 0);
}

int CSock::Recv(char *Data, int Length){
	
	return recv(s, Data, Length, 0);
}

int CSock::Disconnect(void){
	
	return shutdown(s, SD_BOTH);
}


bool CSock::Resolve(void){

	memset(&Name, 0, sizeof(Host));
	Name.sin_family = AF_INET;
	Name.sin_port = htons(Port);

	if((Name.sin_addr.s_addr = inet_addr(Host)) == INADDR_NONE){
		hostent *HostEnt = gethostbyname(Host);
		if(!HostEnt){
			WSASetLastError(WSAHOST_NOT_FOUND);
			return false;
		}
		memcpy(&Name.sin_addr, HostEnt->h_addr, HostEnt->h_length);
	}

	return true;
}

SOCKET CSock::GetS() const { 
	return s;
}

sockaddr_in* CSock::GetName() const{
	return (sockaddr_in*)&Name;
}

bool CSock::Enumerate(void){
	WSANETWORKEVENTS NetworkEvents;
	WSAEnumNetworkEvents(s, hEvent, &NetworkEvents);
	
	if(!NetworkEvents.lNetworkEvents || !Callback){
		return false;
	}

	if(NetworkEvents.lNetworkEvents & FD_READ){
		Callback(this, FD_READ, NetworkEvents.iErrorCode[FD_READ_BIT]);
	}

	if(NetworkEvents.lNetworkEvents & FD_WRITE){
		Callback(this, FD_WRITE, NetworkEvents.iErrorCode[FD_WRITE_BIT]);
	}

	if(NetworkEvents.lNetworkEvents & FD_OOB){
		Callback(this, FD_OOB, NetworkEvents.iErrorCode[FD_OOB_BIT]);
	}

	if(NetworkEvents.lNetworkEvents & FD_ACCEPT){
		Callback(this, FD_ACCEPT, NetworkEvents.iErrorCode[FD_ACCEPT_BIT]);
	}

	if(NetworkEvents.lNetworkEvents & FD_CONNECT){
		Callback(this, FD_CONNECT, NetworkEvents.iErrorCode[FD_CONNECT_BIT]);
	}

	if(NetworkEvents.lNetworkEvents & FD_CLOSE){
		Callback(this, FD_CLOSE, NetworkEvents.iErrorCode[FD_CLOSE_BIT]);
	}

	if(NetworkEvents.lNetworkEvents & FD_QOS){
		Callback(this, FD_QOS, NetworkEvents.iErrorCode[FD_QOS_BIT]);
	}

	if(NetworkEvents.lNetworkEvents & FD_GROUP_QOS){
		Callback(this, FD_GROUP_QOS, NetworkEvents.iErrorCode[FD_GROUP_QOS_BIT]);
	}

	if(NetworkEvents.lNetworkEvents & FD_ROUTING_INTERFACE_CHANGE){
		Callback(this, FD_ROUTING_INTERFACE_CHANGE, NetworkEvents.iErrorCode[FD_ROUTING_INTERFACE_CHANGE_BIT]);
	}

	if(NetworkEvents.lNetworkEvents & FD_ADDRESS_LIST_CHANGE){
		Callback(this, FD_ADDRESS_LIST_CHANGE, NetworkEvents.iErrorCode[FD_ADDRESS_LIST_CHANGE_BIT]);
	}

	return true;
}