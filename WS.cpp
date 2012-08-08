#include "WS.h"

void WS::Initialize()
{
	// Set up the member variables
	bConnected = false;
	s = INVALID_SOCKET;
	bReady = false;
	
	// Create a TCP socket
	s = socket(2, SOCK_STREAM, IPPROTO_IP);
	if(s != INVALID_SOCKET)
	{
		bReady = true;
	}
	else
	{
		Error("Could not initialize winsock.", WSAGetLastError());
	}

	SetSocketBlocking(false);
}

WS::WS()
{
	WSAStartup(0x101, &WinsockData);
	Initialize();
}

WS::~WS()
{
	Close();
	bReady = false;
	WSACleanup();
}

bool WS::Connect(const char *Server, WORD Port)
{
	// I'm not totally sure how I did this, but somehow it connects to Server on Port.
	// Server is either, "www.google.com" or "127.0.0.1"
	hostent *Host;
	sockaddr_in sock;
	char hstip[16];

	SetSocketBlocking(true);
	if(isdigit(*Server))
	{
		sock.sin_addr.s_addr = inet_addr(Server);
	}
	else
	{
		Host = gethostbyname(Server);
		if(Host)
		{
			sprintf (hstip, "%u.%u.%u.%u", 
				(unsigned char) Host->h_addr_list[0][0],
				(unsigned char) Host->h_addr_list[0][1],
				(unsigned char) Host->h_addr_list[0][2],
				(unsigned char) Host->h_addr_list[0][3]);
			sock.sin_addr.s_addr = inet_addr(hstip);	 
		}
		else
		{
			Error("Unable to resolve host.", WSAGetLastError());
			SetSocketBlocking(false);
			return false;
		}
	}		

	sock.sin_family = AF_INET;
	sock.sin_port = htons(Port);
	if(!(connect(s, (sockaddr*)&sock, sizeof(sockaddr_in))))
	{
		bConnected = true;
		bReady = false;
		Connected();
		SetSocketBlocking(false);
		return true;
	}
	else
	{
		bConnected = false;
		bReady = true;
		Error("Error Connecting.", WSAGetLastError());
		SetSocketBlocking(false);
		return false;
	}
}

bool WS::Close()
{
	// Make the socket block while we disconnect
	SetSocketBlocking(true);

	// Attempt to close the socket
	if(!closesocket(s))
	{	
		Disconnected();
		SetSocketBlocking(false);
		Initialize();
		return true;
	}
	else
	{
		Error("Error closing socket.", WSAGetLastError());
		SetSocketBlocking(false);
		return false;
	}
}

bool WS::Send(char *Data, DWORD Length)
{
	this->SetSocketBlocking(true);
	DWORD LengthSent = send(s, Data, Length, NULL);
	this->SetSocketBlocking(false);

	if(LengthSent != SOCKET_ERROR)
	{
		DataSent(Data, LengthSent);
		return true;
	}
	else
	{
		Error("Error sending data", WSAGetLastError());
		return false;
	}
}

DWORD WS::Recv(char *Data, DWORD Length)
{
	DWORD retLength = recv(s, Data, Length, 0);
	
	if(retLength != SOCKET_ERROR)
	{
		DataRecieved(Data, retLength);
		return retLength;
	}
	else
	{
		Error("Error recieving data.", WSAGetLastError());
		return retLength;
	}
}

int WS::GetLastError()
{
	return WSAGetLastError();
}

char *WS::GetErrorString(DWORD ErrorCode)
{
	switch(ErrorCode)
	{
		case 0: return "WSABASEERR No Error";
		case 10004: return "WSAEINTR: - Interrupted system call";
		case 10009: return "WSAEBADF: - Bad file number";
		case 10013: return "WSAEACCES: - Permission denied";
		case 10014: return "WSAEFAULT: - Bad address";
		case 10022: return "WSAEINVAL: - Invalid argument";
		case 10024: return "WSAEMFILE: - Too many open files";
		case 10035: return "WSAEWOULDBLOCK: - Operation would block";
		case 10036: return "WSAEINPROGRESS: - Operation now in progress";
		case 10037: return "WSAEALREADY: - Operation already in progress";
		case 10038: return "WSAENOTSOCK: - Socket operation on non-socket";
		case 10039: return "WSAEDESTADDRREQ: - Destination address required";
		case 10040: return "WSAEMSGSIZE: - Message too long";
		case 10041: return "WSAEPROTOTYPE: - Protocol wrong type for socket";
		case 10042: return "WSAENOPROTOOPT: - Bad protocol option";
		case 10043: return "WSAEPROTONOSUPPORT: - Protocol not supported";
		case 10044: return "WSAESOCKTNOSUPPORT: - Socket type not supported";
		case 10045: return "WSAEOPNOTSUPP: - Operation not supported on socket";
		case 10046: return "WSAEPFNOSUPPORT: - Protocol family not supported";
		case 10047: return "WSAEAFNOSUPPORT: - Address family not supported by protocol family";
		case 10048: return "WSAEADDRINUSE: - Address already in use";
		case 10049: return "WSAEADDRNOTAVAIL: - Can't assign requested address";
		case 10050: return "WSAENETDOWN: - Network is down";
		case 10051: return "WSAENETUNREACH: - Network is unreachable";
		case 10052: return "WSAENETRESET: - Net dropped connection or reset";
		case 10053: return "WSAECONNABORTED: - Software caused connection abort";
		case 10054: return "WSAECONNRESET: - Connection reset by peer";
		case 10055: return "WSAENOBUFS: - No buffer space available";
		case 10056: return "WSAEISCONN: - Socket is already connected";
		case 10057: return "WSAENOTCONN: - Socket is not connected";
		case 10058: return "WSAESHUTDOWN: - Can't send after socket shutdown";
		case 10059: return "WSAETOOMANYREFS: - Too many references, can't splice";
		case 10060: return "WSAETIMEDOUT: - Connection timed out";
		case 10061: return "WSAECONNREFUSED: - Connection refused";
		case 10062: return "WSAELOOP: - Too many levels of symbolic links";
		case 10063: return "WSAENAMETOOLONG: - File name too long";
		case 10064: return "WSAEHOSTDOWN: - Host is down";
		case 10065: return "WSAEHOSTUNREACH: - No Route to Host";
		case 10066: return "WSAENOTEMPTY: - Directory not empty";
		case 10067: return "WSAEPROCLIM: - Too many processes";
		case 10068: return "WSAEUSERS: - Too many users";
		case 10069: return "WSAEDQUOT: - Disc Quota Exceeded";
		case 10070: return "WSAESTALE: - Stale NFS file handle";
		case 10071: return "WSAEREMOTE: - Too many levels of remote in path";
		case 10091: return "WSASYSNOTREADY: - Network SubSystem is unavailable";
		case 10092: return "WSAVERNOTSUPPORTED: - Winsock DLL Version out of range";
		case 10093: return "WSANOTINITIALISED: - Successful WSASTARTUP not yet performed";
		case 10110: return "WSA_E_NO_MORE: - No more results can be returned by WSALookupServiceNext";
		case 11001: return "WSAHOST_NOT_FOUND: - Host not found";
		case 11002: return "WSATRY_AGAIN: - Non-Authoritative Host not found";
		case 11003: return "WSANO_RECOVERY: - Non-Recoverable Errors: (FORMERR, REFUSED, NOTIMP)"; 
		case 11004: return "WSANO_DATA: - Valid name, no data record of requested type";
		default: return "Unknown error!";
	}
}

// Will recieve up to 5000 bytes
bool WS::Recv()
{
	// Set the recieve buffer to 5000 bytes
	char RecvBuffer[5000];
	int Length = recv(s, RecvBuffer, 4999, NULL);
	if(Length == SOCKET_ERROR)
	{
		Error("Error recieving data", WSAGetLastError());
		return false;
	}
	else if(Length > 0)
	{
		DataRecieved(RecvBuffer, Length);
		return true;
	}
	return false;
}

void WS::SetSocketBlocking(DWORD Block)
{
	// Block is backwords, so fix it
	Block = !Block;
	// ioctlsocket is the big fancy function that controls the blocking
	if(ioctlsocket(s, FIONBIO, &Block) == SOCKET_ERROR)
	{
		Error("Error setting socket blocking (ioctlsocket)", WSAGetLastError());
	}
}

bool WS::SendByte(BYTE Data)
{
	return Send((char*)&Data, sizeof(BYTE));	
}

bool WS::SendWord(WORD Data)
{
	return Send((char*)&Data, sizeof(WORD));
}

bool WS::SendDWord(DWORD Data)
{
	return Send((char*)&Data, sizeof(DWORD));
}

bool WS::SendAscii(char *Data, bool SendNull)
{
	// IF SendNull is true, it will send the null-terminator after the string.
	return Send(Data, SendNull == false ? (DWORD)strlen(Data) : (DWORD)strlen(Data) + 1);
}

// This is probably the preferred function to use, but that's just my opinion because I 
// wrote the buffer class :)
bool WS::SendBuffer(Buffer Data)
{
	return Send(((char*)Data.c_str()), Data.GetSize());
}


void WS::Error(char *Message, DWORD Code)
{
	// WSAEWOULDBLOCK is a worthless error for my purposes
	if(Code != WSAEWOULDBLOCK)
	{
		ErrorFunc(Message, Code);
	}
}

// By default the events are all blank
void WS::DataSent(char *Data, DWORD Length)
{
}

void WS::DataRecieved(char *Data, DWORD Length)
{
}

void WS::ErrorFunc(char *Description, DWORD Code)
{
}

void WS::Connected()
{
}

void WS::Disconnected()
{
}
