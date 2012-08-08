#include "BinaryBot.h"
#include <iostream>
#include <string>
using namespace std;

//#define VERBOSE

BinaryBot::BinaryBot()
{
	// We aren't connected or logged on
	bConnected = false;
	bLoggedOn = false;

	// Set up the client token
	ClientToken = GetTickCount();
}
	
void BinaryBot::SetCDKey(string CDKey)
{
	this->CDKey = CDKey;

	BNetHashing::DecodeCDKey(&ProductCode, &CDKeyVal1, &CDKeyVal2, CDKey.c_str());
	if(CDKeyVal1 == 0)
	{
		InvalidCDKey();
	}
}

void BinaryBot::SetProduct(string Product)
{
	// The product has to be reversed
	this->Product = "";
	for(int i = (int)Product.length() - 1; i >= 0; i--)
		this->Product += Product[i];
}

void BinaryBot::SetStarcraftPath(string path)
{
	if(LoadLibrary(path.c_str()))
	{
		this->StarcraftPath = path;
	}
	else
	{
		this->NoHashFile("Starcraft.exe", path);
	}
}

void BinaryBot::SetUsername(string Username)
{
	this->Username = Username;
	this->BanMessage = Username + " was banned";
	this->KickMessage = Username + " was kicked";
}

void BinaryBot::SetPassword(string Password)
{
	this->Password = Password;
}

void BinaryBot::SetStormPath(string path)
{
	if(LoadLibrary(path.c_str()))
	{
		this->StormPath = path;
	}
	else
	{
		this->NoHashFile("Storm.dll", path);
	}
}

void BinaryBot::SetBattlePath(string path)
{
	if(LoadLibrary(path.c_str()))
	{
		this->BattlePath = path;
	}
	else
	{
		this->NoHashFile("Battle.snp", path);
	}
}

void BinaryBot::SetHomeChannel(string HomeChannel)
{
	this->HomeChannel = HomeChannel;
}

bool BinaryBot::Connect(string Server, WORD Port)
{
	// Set the server and the port
	this->Server = Server;
	this->Port = Port;

	return WS::Connect(Server.c_str(), Port);
}

// These are the functions that process the data
void BinaryBot::DataSent(char *Data, DWORD Length)
{
#ifdef VERBOSE
	Buffer SendBuf((BYTE*)Data, Length);

	cout << "Data sent to battle.net:" << endl;
	cout << SendBuf.toString() << endl;
#endif

}

void BinaryBot::DataRecieved(char *Data, DWORD Length)
{
	Buffer RecvBuf((BYTE*)Data, Length);

#ifdef VERBOSE
	cout << "Data recieved:" << endl;
	cout << RecvBuf.toString() << endl << endl;
#endif

	// Get the header of the buffer
	BYTE Header[4];
	RecvBuf.RemoveFromFront(Header, sizeof(DWORD));

	// This switch is for the packet code
	switch(Header[1])
	{
	case SID_NULL:
		// Reply with a null packet
		{
			Buffer EmptyBuf;
			SendBNetPacket(SID_NULL, EmptyBuf);
		}
		break;
	case SID_PING: // 0x25
		ProcessSID_PING(RecvBuf, Length);
		break;
	case SID_AUTH_INFO: // 0x50
		ProcessSID_AUTH_INFO(RecvBuf, Length);
		break;
	case SID_AUTH_CHECK: // 0x51
		ProcessSID_AUTH_CHECK(RecvBuf, Length);
		break;
	case SID_CHATEVENT:
		ProcessSID_CHATEVENT(RecvBuf, Length);
		break;
	case SID_LOGONRESPONSE:
		ProcessSID_LOGONRESPONSE(RecvBuf, Length);
		break;
	case SID_ENTERCHAT:
		ProcessSID_ENTERCHAT(RecvBuf, Length);
		break;
	case SID_READUSERDATA:
		ProcessSID_READUSERDATA(RecvBuf, Length);
		break;
	default:
		RecievedUnknownPacket(Header[1], RecvBuf, Length);
		break;
	}
}
void BinaryBot::ErrorFunc(const char *ErrorMessage, DWORD ErrorCode)
{
	if(ErrorCode != WSAEWOULDBLOCK)
	{
		WinsockError(ErrorMessage, ErrorCode);
	}
}
void BinaryBot::Connected()
{
	ConnectedToBNet();

	SendByte(1);

	Buffer AuthInfo;

	AuthInfo << (DWORD) 0;
	AuthInfo << "68XI";
	AuthInfo << Product;
	AuthInfo << (DWORD) VersionByte;
	AuthInfo << (DWORD) 0;
	AuthInfo << (DWORD) 0;
	AuthInfo << (DWORD) 0;
	AuthInfo << (DWORD) 0;
	AuthInfo << (DWORD) 0;
	AuthInfo << "CAN";
	AuthInfo << (BYTE) 0;
	AuthInfo << "Canada";
	AuthInfo << (BYTE) 0;
	SendBNetPacket(SID_AUTH_INFO, AuthInfo); 
}
void BinaryBot::Disconnected()
{
	DisconnectedFromBNet();
}

bool BinaryBot::Recv()
{
	char HeaderBuf[4];
	// Peek at the first 4 bytes (the header)
	DWORD RecvReturn = recv(s, HeaderBuf, 4, MSG_PEEK);

	if(RecvReturn == SOCKET_ERROR)
	{
		// There was an error of some sort
		ErrorFunc("There was an error while trying to recieve data", WSAGetLastError());
		return false;
	}
	else if(RecvReturn == 4)
	{
		// This is a battle.net packet, so it always starts with the following:
		// (BYTE) 0xFF
		// (BYTE) PacketCode
		// (WORD) Length

		// Make sure the header is valid
		if((char)HeaderBuf[0] != (char)0xFF)
		{
			Error("Invalid packet recieved from battle.net (no 0xFF at the beginning)", 0xbad);
			return false;
		}

		// Get the length
		WORD Length = *((WORD*)(HeaderBuf + 2));
		// Allocate enough room for the packet
		char *RecvBuf = (char*)malloc(Length);
		// Make the socket block so we'll recieve the entire packet
		this->SetSocketBlocking(true);
		// Recieve the data and remember the length returend
		WORD LengthRecieved = recv(s, RecvBuf, Length, NULL);
		// If the recv failed, call the error routine
		if(LengthRecieved == SOCKET_ERROR)
		{
			Error("Unable to recieve battle.net packet", WSAGetLastError());
			SetSocketBlocking(false);
			free(RecvBuf);
			return false;
		}
		else
		{
			// This shouldn't happen, but make sure it recieved all the bytes
			if(LengthRecieved != Length)
			{
				Error("Not enough bytes recieved.. if this can actually happen, I'm going to have to fix some stuff.  If you see this, please tell me right away! :-)", WSAGetLastError());
				this->SetSocketBlocking(true);
				free(RecvBuf);
				return false;
			}
			else
			{
				// Looks like it succeeded, so call the DataRecieved method
				DataRecieved(RecvBuf, Length);
				this->SetSocketBlocking(true);
				free(RecvBuf);
				return true;
			}
		}
	}
	else // if recv() != 4
	{
		return false;
	}
}

bool BinaryBot::SendBNetPacket(BYTE Code, Buffer Packet)
{
	// Add the size of the header to Length
	WORD Length = (WORD)Packet.GetSize() + 4;

	// Build the header
	Buffer ToSend((BYTE)0xFF);
	ToSend << Code;
	ToSend << (WORD)(Length);
	ToSend << Packet;

	return Send((char*)ToSend.c_str(), Length);
}
void BinaryBot::ProcessSID_PING(Buffer Data, DWORD Length)
{
	// Format:
	// (DWORD)		 Timestamp 
	// Remarks: Used to calculate Client's ping. The received timestamp should be send directly 
	//  back to Battle.net. The ping displayed when in chat can be artificially inflated by 
	//  delaying before sending this packet. On some clients, ping can be set to -1 (Strictly 
	//  speaking, 0xFFFFFFFF, since ping is unsigned) by not responding to this packet at all. 

	// Turn off the Nagle algorithm off to make sure it's sent out immediately
	DWORD NagleState = true;
	setsockopt(s, IPPROTO_TCP, TCP_NODELAY, (char*)&NagleState, 4);

	// Respond to the ping request with our own ping packet
	SendBNetPacket(SID_PING, Data);

	// Turn the Nagle algorithm back on
	NagleState = false;
	setsockopt(s, IPPROTO_TCP, TCP_NODELAY, (char*)&NagleState, 4);

	Pinged();
}

void BinaryBot::ProcessSID_AUTH_INFO(Buffer Data, DWORD Length)
{
	// Format:
	//  (DWORD)		 Logon Type	
	//  (DWORD)		 Server Token
	//  (DWORD)		 UDPValue**
	//  (FILETIME)	 MPQ filetime
	//  (STRING)		 IX86ver filename
	//  (STRING)		 ValueString
	// Remarks: Contains the Server Token, and the values used in CheckRevision.
 
	DWORD LogonType;
	DWORD UDPValue;
	FILETIME MPQFiletime;
	string IX86Filename;
	string ChecksumFormula;

	Data >> LogonType;
	Data >> ServerToken;
	Data >> UDPValue;
	Data.RemoveFromFront((BYTE*)&MPQFiletime, sizeof(FILETIME));
	Data >> IX86Filename;
	Data >> ChecksumFormula;

	// Get ready to start building the response, SID_AUTH_CHECK
	Buffer Response;

	// Load the appropriate CheckRevision library into memory.  Although they are named
	// .mpq files, they are actually .dll files.
	IX86Filename.resize(IX86Filename.length() - 4);
	IX86Filename += ".dll";
	

	HMODULE RevFunc = LoadLibrary(IX86Filename.c_str());
	if(RevFunc == NULL)
	{
		NoIX86File(IX86Filename);
	}

	// Declare VersionInfo/CheckRevision variables
	DWORD VersionHash = 0;
	DWORD CheckSum = 0;
	char ExeInfo[0x100];
	BNetHashing::GetVersionInfo(StormPath.c_str(), BattlePath.c_str(), StarcraftPath.c_str(), RevFunc, ChecksumFormula.c_str(), ExeInfo, &VersionHash, &CheckSum, IX86Filename);

	// Get the hash value
	// Buffer to store the hash in
	DWORD HashValue[5];
	// Buffer to store that data that will be
	DWORD HashData[6];
	HashData[0] = ClientToken;
	HashData[1] = ServerToken;
	HashData[2] = ProductCode;
	HashData[3] = CDKeyVal1;
	HashData[4] = 0;
	HashData[5] = CDKeyVal2;
	BNetHashing::GetHash(HashValue, HashData, sizeof(DWORD) * 6);

	// Build the packet that will be sent out

	Buffer ResponsePacket;
	//ResponsePacket << (BYTE)0xFF;
	//ResponsePacket << (BYTE)0x51;
	//ResponsePacket << (WORD)0;
	ResponsePacket << ClientToken;
	ResponsePacket << VersionHash;
	ResponsePacket << CheckSum;
	ResponsePacket << (DWORD)1;
	ResponsePacket << (DWORD)false;
	ResponsePacket << (DWORD)CDKey.length();
	ResponsePacket << ProductCode;
	ResponsePacket << CDKeyVal1;
	ResponsePacket << (DWORD)0;
	ResponsePacket << HashValue[0] << HashValue[1] << HashValue[2] << HashValue[3] << HashValue[4];
	ResponsePacket << ExeInfo;
	ResponsePacket << (BYTE)0;
	ResponsePacket << Username;
	ResponsePacket << (BYTE)0;
	
	SendBNetPacket(SID_AUTH_CHECK, ResponsePacket);
}
void BinaryBot::ProcessSID_AUTH_CHECK(Buffer Data, DWORD Length)
{
	DWORD Response;
	string MoreInfo;
	Data >> Response;
	Data >> MoreInfo;

	if(Response == 0)
	{
		PassedVersionCheck();
		SendUserInfo();
	}
	else
	{
		FailedVersionCheck(Response, MoreInfo);
	}
}

void BinaryBot::ProcessSID_LOGONRESPONSE(Buffer Data, DWORD Length)
{
	DWORD Response;
	Data >> Response;

	if(Response)
	{
		PasswordAccepted();

		// Send the enterchat packet
		Buffer EnterChat;
		EnterChat << Username << (BYTE)0;
		EnterChat << (BYTE)0;
		SendBNetPacket(SID_ENTERCHAT, EnterChat);

		// Because I don't know how this packet works, enter the default
		// channel.
		Buffer JoinChannel;
		JoinChannel << (DWORD) 1;
		JoinChannel << "Brood War" << (BYTE) 0;
		SendBNetPacket(SID_JOINCHANNEL, JoinChannel);
	}
	else
	{
		PasswordRejected();
	}
}

void BinaryBot::SendUserInfo()
{
	DWORD PassHash[5];
	for(DWORD i = 0; i < Password.length(); i++)
	{
		Password[i] = tolower(Password[i]);
	}
	BNetHashing::GetHash(PassHash, (DWORD*)Password.c_str(), (DWORD)Password.length());

	DWORD PassHashData[7];
	PassHashData[0] = ClientToken;
	PassHashData[1] = ServerToken;
	PassHashData[2] = PassHash[0];
	PassHashData[3] = PassHash[1];
	PassHashData[4] = PassHash[2];
	PassHashData[5] = PassHash[3];
	PassHashData[6] = PassHash[4];

	DWORD EntireHash[5];
	BNetHashing::GetHash(EntireHash, PassHashData, 7 * sizeof(DWORD));

	Buffer PassPacket;
	PassPacket << ClientToken;
	PassPacket << ServerToken;
	PassPacket << EntireHash[0] << EntireHash[1] << EntireHash[2] << EntireHash[3] << EntireHash[4];
	PassPacket << Username;
	PassPacket << (BYTE) 0;

	SendBNetPacket(SID_LOGONRESPONSE, PassPacket);
}

void BinaryBot::ProcessSID_CHATEVENT(Buffer Data, DWORD Length)
{
	// Format:
	//  (DWORD)		Event ID
	//  (DWORD)		User's Flags
	//  (DWORD)		Ping
	//  (DWORD)		IP Address (Defunct)
	//  (DWORD)		Account number (Defunct)
	//  (DWORD)		Registration Authority (Defunct)
	//  (STRING)	Username
	//  (STRING)	Text 
	// Remarks: Contains all chat events. 

	DWORD EID;
	DWORD Flags;
	DWORD Ping;
	DWORD BAADF00D;
	DWORD BAADF00D2;
	DWORD Empty;
	string User;
	string Message;

	Data >> EID;
	Data >> Flags;
	Data >> Ping;
	Data >> BAADF00D;
	Data >> BAADF00D2;
	Data >> Empty;
	Data >> User;
	Data >> Message;

	switch(EID)
	{
	case EID_USERINCHANNEL:
		UserInChannel(Flags, Ping, User, ParseStatString::GetStatString((char*)Message.c_str()));
		break;
	case EID_USERJOINCHANNEL:
		UserJoinedChannel(Flags, Ping, User, ParseStatString::GetStatString((char*)Message.c_str()));
		break;
	case EID_USERLEAVECHANNEL:
		UserLeftChannel(Flags, Ping, User, Message);
		break;
	case EID_WHISPERFROM:
		WhisperRecieved(Flags, Ping, User, Message);
		break;
	case EID_INCOMING_CHAT:
		ChatRecieved(Flags, Ping, User, Message);
		break;
	case EID_ERROR:
		BNetError(Message);
		break;
	case EID_CHANNELJOIN:
		Channel = Message;
		JoinedChannel(Flags, Message);
		break;
	case EID_CHANGEFLAGS:
		FlagsChanged(Flags, Ping, User, Message);
		break;
	case EID_WHISPERTO:
		WhisperSent(Flags, Ping, User, Message);
		break;
	case EID_INFORMATION:
		{
			// Check for bans/kicks
			string CheckBan = Message;
			CheckBan.resize(BanMessage.length());
			if(CheckBan == BanMessage)
			{
				BannedFromChannel();
			}
			else if(CheckBan == KickMessage)
			{
				KickedFromChannel();
			}

			InfoRecieved(Message);
			break;
		}
	case EID_CHANNELFULL:
		BNetError(Message);
		break;
	case EID_IGNOREON:
		IgnoringUser(User, Message);
		break;
	case EID_IGNOREOFF:
		NotIgnoringUser(User, Message);
		break;
	case EID_EMOTE:
		Emote(Flags, Ping, User, Message);
		break;
	default:
		UnknownEvent(Flags, Ping, User, Message);
	}
}

void BinaryBot::ProcessSID_ENTERCHAT(Buffer Data, DWORD Length)
{
	// After this is recieved, we're ready to get going
	LoggedIn();
}

bool BinaryBot::SendAscii(string Text)
{
	Buffer MyBuf;
	MyBuf << Text << (BYTE) 0;
	return SendBNetPacket(SID_CHATCOMMAND, MyBuf);
}

string BinaryBot::GetChannel()
{
	return Channel;
}

void BinaryBot::SendProfileRequest(string User)
{
	LastProfileName = User;

	Buffer PRequest;

	PRequest << (DWORD) 1; // Always a single request
	PRequest << (DWORD) 4; // Always 4 fields
	PRequest << ProfileCookie; // The "cookie" for the request
	PRequest << User << (BYTE) 0; // The name of the person to request the profile of
	PRequest << "profile\\sex" << (BYTE) 0; // Get the sex
	PRequest << "profile\\age" << (BYTE) 0; // Get the age
	PRequest << "profile\\location" << (BYTE) 0; // Get the location
	PRequest << "profile\\description" << (BYTE) 0; // Get the description
	SendBNetPacket(SID_READUSERDATA, PRequest);
}

void BinaryBot::SendRecDataRequest(string User)
{
	LastProfileName = User;
	Buffer Request;
	Request << (DWORD) 1; // One user
	Request << (DWORD) 31; // With 31 fields
	Request << RecDataCookie; // The record cookie
	Request << User << (BYTE) 0; // The username

	Request << "profile\\sex" << (BYTE) 0; // The standard profile data
	Request << "profile\\age" << (BYTE) 0;
	Request << "profile\\location" << (BYTE) 0;
	Request << "profile\\description" << (BYTE) 0;
	
	Request << "Record\\SEXP\\0\\wins" << (BYTE) 0; // Brood war regular stuff
	Request << "Record\\SEXP\\0\\losses" << (BYTE) 0;
	Request << "Record\\SEXP\\0\\disconnects" << (BYTE) 0;
	Request << "Record\\SEXP\\0\\last game result" << (BYTE) 0;
	Request << "Record\\SEXP\\1\\wins" << (BYTE) 0; // Brood war ladder stuff
	Request << "Record\\SEXP\\1\\losses" << (BYTE) 0;
	Request << "Record\\SEXP\\1\\disconnects" << (BYTE) 0;
	Request << "Record\\SEXP\\1\\rating" << (BYTE) 0;
	Request << "Record\\SEXP\\1\\last game result" << (BYTE) 0;
	
	Request << "Record\\STAR\\0\\wins" << (BYTE) 0; // Starcraft regular
	Request << "Record\\STAR\\0\\losses" << (BYTE) 0;
	Request << "Record\\STAR\\0\\disconnects" << (BYTE) 0;
	Request << "Record\\STAR\\0\\last game result" << (BYTE) 0;
	Request << "Record\\STAR\\1\\wins" << (BYTE) 0; // Starcraft ladder
	Request << "Record\\STAR\\1\\losses" << (BYTE) 0;
	Request << "Record\\STAR\\1\\disconnects" << (BYTE) 0;
	Request << "Record\\STAR\\1\\rating" << (BYTE) 0;
	Request << "Record\\STAR\\1\\last game result" << (BYTE) 0;
	
	Request << "Record\\W2BN\\0\\wins" << (BYTE) 0; // War2 regular
	Request << "Record\\W2BN\\0\\losses" << (BYTE) 0;
	Request << "Record\\W2BN\\0\\disconnects" << (BYTE) 0;
	Request << "Record\\W2BN\\0\\last game result" << (BYTE) 0;
	Request << "Record\\W2BN\\1\\wins" << (BYTE) 0; // War2 ladder
	Request << "Record\\W2BN\\1\\losses" << (BYTE) 0;
	Request << "Record\\W2BN\\1\\disconnects" << (BYTE) 0;
	Request << "Record\\W2BN\\1\\rating" << (BYTE) 0;
	Request << "Record\\W2BN\\1\\last game result" << (BYTE) 0;

	SendBNetPacket(SID_READUSERDATA, Request);
}

void BinaryBot::ProcessSID_READUSERDATA(Buffer Data, DWORD Length)
{
	DWORD Requests;
	DWORD Fields;
	DWORD Cookie;

	Data >> Requests;
	Data >> Fields;
	Data >> Cookie;

	// If it's an invalid cookie or an invalid number of fields, it's just ignored
	if(Cookie == ProfileCookie)
	{
		if(Fields == 4)
		{
			// It was valid
			string Sex;
			string Age;
			string Location;
			string Description;

			Data >> Sex >> Age >> Location >> Description;

			ProfileRecieved(LastProfileName, Sex, Age, Location, Description);
		}
	}
	else if(Cookie == RecDataCookie)
	{
		if(Fields == 31)
		{
			// Define our many variables
			string Sex, Age, Location, Description;
			string BWWins, BWLosses, BWDisconnects, BWLastGame;
			string BWLWins, BWLLosses, BWLDisconnects, BWLRating, BWLLastGame;
			string SCWins, SCLosses, SCDisconnects, SCLastGame;
			string SCLWins, SCLLosses, SCLDisconnects, SCLRating, SCLLastGame;
			string WC2Wins, WC2Losses, WC2Disconnects, WC2LastGame;
			string WC2LWins, WC2LLosses, WC2LDisconnects, WC2LRating, WC2LLastGame;

			Data >> Sex >> Age >> Location >> Description;
			Data >> BWWins >> BWLosses >> BWDisconnects >> BWLastGame;
			Data >> BWLWins >> BWLLosses >> BWLDisconnects >> BWLRating >> BWLLastGame;
			Data >> SCWins >> SCLosses >> SCDisconnects >> SCLastGame;
			Data >> SCLWins >> SCLLosses >> SCLDisconnects >> SCLRating >> SCLLastGame;
			Data >> WC2Wins >> WC2Losses >> WC2Disconnects >> WC2LastGame;
			Data >> WC2LWins >> WC2LLosses >> WC2LDisconnects >> WC2LRating >> WC2LLastGame;

			RecDataRecieved(LastProfileName, Sex, Age, Location, Description, 
							BWWins, BWLosses, BWDisconnects, BWLastGame,
							BWLWins, BWLLosses, BWLDisconnects, BWLRating, BWLLastGame,
							SCWins, SCLosses, SCDisconnects, SCLastGame,
							SCLWins, SCLLosses, SCLDisconnects, SCLRating, SCLLastGame,
							WC2Wins, WC2Losses, WC2Disconnects, WC2LastGame,
							WC2LWins, WC2LLosses, WC2LDisconnects, WC2LRating, WC2LLastGame);
		}
	}
}