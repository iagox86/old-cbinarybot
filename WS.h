// This is a wrapper for winsock that will allow a person to easily connect
// to a remote site over TCP and send/recieve data.

#ifndef WS_H
#define WS_H

#include <winsock2.h>
#include <stdio.h>
#include "Buffer.h"
//#include <ws2tcpip.h>


 
class WS
{
protected:
	// The socket used.. probably shouldn't be played with
	SOCKET s;
	// A couple booleans to keep track of the state
	bool bReady;
	bool bConnected;
	// This is probably useless, but I left it here anyway
	WSAData WinsockData;
	// This initializes winsock and such
	void Initialize();
	// This sets the socket to either block or not block
	void SetSocketBlocking(DWORD Block);

	// Calls the error callback if it's an important error
	void Error(char *Message, DWORD Code);

	virtual void DataSent(char *Data, DWORD Length);
	virtual void DataRecieved(char *Data, DWORD Length);
	virtual void ErrorFunc(char *Description, DWORD Code);
	virtual void Connected();
	virtual void Disconnected();

public:
	// Default constructor
	WS();
	~WS();

	// Connects to an ip or domain name specified by the string server on the port specified
	// by Port
	bool Connect(const char *Server, WORD Port);
	// Closes the connection
	bool Close();
	// Send out Length bytes of Data
	bool Send(char *Data, DWORD Length);
	// Send out a single byte
	bool SendByte(BYTE Data);
	// Send out a single word
	bool SendWord(WORD Data);
	// Send out a single dword
	bool SendDWord(DWORD Data);
	// Send out an ascii string with or without the null terminator
	bool SendAscii(char *Data, bool SendNull = false);
	// Send out a buffer
	bool SendBuffer(Buffer Data);

	// Manually recieve Length bytes of data
	virtual DWORD Recv(char *Data, DWORD Length);
	// If there is any data waiting, recieve all of it
	virtual bool Recv();

	// Get the code for the last error
	int GetLastError();
	// Gets the equivolant textstring for the error code
	static char *GetErrorString(DWORD ErrorCode);
};











#endif