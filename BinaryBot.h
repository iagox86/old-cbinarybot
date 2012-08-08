// BinaryBot.h
// by Ron and hopefully Sean
// Created 06/18/2003
//
// This is a fairly general Binary Bot class that will log on as Starcraft.

// Some things to do:
// Ability for changing products
// Do SID_AUTH_INFO right
// Event for profile
// Event for game
// Event for ad
// Action for getting game list
// Can enable/disable profanity filter
// Can set auto-rejoin on kick
// Can set auto-home-channel-join on ban
// Can store list of users currently in the channel
// Can find user in channel by name or wildcard
// Reverse CheckRevision
// Buffer to avoid flooding
// Statstring parsing (Warcraft III, make Diablo II more general)

//#define VERBOSE

#ifndef BINARYBOT_H
#define BINARYBOT_H

// Thanks to KP for codes! ;-)
#define SID_NULL                                (0x00)
#define SID_0x0001                              (0x01)
#define SID_STOPADV                             (0x02)
#define SID_0x0003                              (0x03)
#define SID_SERVERLIST                          (0x04)
#define SID_CLIENTID                            (0x05)
#define SID_STARTVERSIONING                     (0x06)
#define SID_REPORTVERSION                       (0x07)
#define SID_0x0008                              (0x08)
#define SID_GETADVLISTEX                        (0x09)
#define SID_ENTERCHAT                           (0x0a)
#define SID_GETCHANNELLIST                      (0x0b)
#define SID_JOINCHANNEL                         (0x0c)
#define SID_0x000D                              (0x0d)
#define SID_CHATCOMMAND                         (0x0e)
#define SID_CHATEVENT                           (0x0f)
#define SID_LEAVECHAT                           (0x10)
#define SID_0x0011                              (0x11)
#define SID_LOCALEINFO                          (0x12)
#define SID_FLOODDETECTED                       (0x13)
#define SID_UDPPINGRESPONSE                     (0x14)
#define SID_CHECKAD                             (0x15)
#define SID_CLICKAD                             (0x16)
#define SID_QUERYMEM                            (0x17)
#define SID_0x0018                              (0x18)
#define SID_MESSAGEBOX                          (0x19)
#define SID_STARTADVEX2                         (0x1a)
#define SID_GAMEDATAADDRESS                     (0x1b)
#define SID_STARTADVEX3                         (0x1c)
#define SID_LOGONCHALLENGEEX                    (0x1d)
#define SID_CLIENTID2                           (0x1e)
#define SID_0x001F                              (0x1f)
#define SID_BROADCAST                           (0x20)
#define SID_DISPLAYAD                           (0x21)
#define SID_NOTIFYJOIN                          (0x22)
#define SID_SETCOOKIE                           (0x23)
#define SID_GETCOOKIE                           (0x24)
#define SID_PING                                (0x25)
#define SID_READUSERDATA                        (0x26)
#define SID_WRITEUSERDATA                       (0x27)
#define SID_LOGONCHALLENGE                      (0x28)
#define SID_LOGONRESPONSE                       (0x29)
#define SID_CREATEACCOUNT                       (0x2a)
#define SID_SYSTEMINFO                          (0x2b)
#define SID_GAMERESULT                          (0x2c)
#define SID_GETICONDATA                         (0x2d)
#define SID_GETLADDERDATA                       (0x2e)
#define SID_FINDLADDERUSER                      (0x2f)
#define SID_CDKEY                               (0x30)
#define SID_CHANGEPASSWORD                      (0x31)
#define SID_0x0032                              (0x32)
#define SID_GETFILETIME                         (0x33)
#define SID_0x0034                              (0x34)
#define SID_0x0035                              (0x35)
#define SID_CDKEY2                              (0x36)
#define SID_0x0037                              (0x37)
#define SID_0x0038                              (0x38)
#define SID_0x0039                              (0x39)
#define SID_0x003A                              (0x3a)
#define SID_0x003B                              (0x3b)
#define SID_CHECKDATAFILE2                      (0x3c)
#define SID_CREATEACCOUNT2                      (0x3d)
#define SID_0x003E                              (0x3e)
#define SID_STARTVERSIONING2                    (0x3f)
#define SID_0x0040                              (0x40)
#define SID_QUERYADURL                          (0x41)
#define SID_CDKEY3                              (0x42)
#define SID_0x0043                              (0x43)
#define SID_0x0044                              (0x44)
#define SID_0x0045                              (0x45)
#define SID_0x0046                              (0x46)
#define SID_0x0047                              (0x47)
#define SID_0x0048                              (0x48)
#define SID_0x0049                              (0x49)
#define SID_0x004A                              (0x4a)
#define SID_0x004B                              (0x4b)
#define SID_0x004C                              (0x4c)
#define SID_0x004D                              (0x4d)
#define SID_0x004E                              (0x4e)
#define SID_0x004F                              (0x4f)
#define SID_AUTH_INFO                           (0x50)
#define SID_AUTH_CHECK                          (0x51)
#define SID_AUTH_ACCOUNTCREATE                  (0x52)
#define SID_AUTH_ACCOUNTLOGON                   (0x53)
#define SID_AUTH_ACCOUNTLOGONPROOF              (0x54)
#define SID_AUTH_ACCOUNTCHANGE                  (0x55)
#define SID_AUTH_ACCOUNTCHANGEPROOF             (0x56)
#define SID_AUTH_ACCOUNTUPGRADE                 (0x57)
#define SID_AUTH_ACCOUNTUPGRADEPROOF            (0x58)
#define SID_AUTH_RECONNECT                      (0x59)
#define SID_AUTH_RECONNECTPROOF                 (0x5a)
#define SID_AUTH_DISCONNECT                     (0x5b)



// The following are dwId's for 0x0F packets:
#define EID_USERINCHANNEL             0x01
#define EID_USERJOINCHANNEL           0x02
#define EID_USERLEAVECHANNEL          0x03
#define EID_WHISPERFROM               0x04
#define EID_INCOMING_CHAT             0x05
#define EID_ERROR                     0x06
#define EID_CHANNELJOIN               0x07
#define EID_CHANGEFLAGS               0x09
#define EID_WHISPERTO                 0x0A
#define EID_INFORMATION               0x12
#define EID_CHANNELFULL               0x13
#define EID_IGNOREON                  0x15
#define EID_IGNOREOFF                 0x16
#define EID_EMOTE                     0x17

#include <iostream>
#include <string>
#include "WS.h"
#include "Buffer.h"
#include "BNetHashing.h"
#include "ParseStatString.h"
using namespace std;

// Extend the winsock class as protected, since we don't want people screwing with the functions
class BinaryBot : public WS
{
public:
	// Here are the flags used in channels
	static const DWORD OPERATOR_FLAG               = 0x02;
	static const DWORD SPEAKER_FLAG                = 0x04;
	static const DWORD SYSTEMADMIN_FLAG            = 0x08;
	static const DWORD CHAT_FLAG                   = 0x10;
	static const DWORD SQUELCHED_FLAG              = 0x20;
	static const DWORD SPECTATOR_FLAG              = 0x40;

protected:
	// Stores the cd-key of the product
	string CDKey;
	// Stores the decoded version of the cd-key
	DWORD CDKeyVal1;
	DWORD CDKeyVal2;
	DWORD ProductCode;

	// The username to logon with
	string Username;
	// This is, for example, "iago was banned by"
	string BanMessage;
	// This is, for example, "iago was kicked by"
	string KickMessage;
	// Store the name of the channel we're currently in
	string Channel;

	// The most recent profile request
	string LastProfileName;
	static const DWORD ProfileCookie = 0xbaadf00d;
	static const DWORD RecDataCookie = 0xdeadbeef;

	// The password to use
	string Password;

	// The server to logon to
	string Server;
	WORD Port;

	// This is the version byte for starcraft/brood war
	static const DWORD VersionByte = 0xc7;

	// This is the 4-digit product code
	string Product;

	// These are the paths to the important storm files
	string StarcraftPath;
	string BattlePath;
	string StormPath;
	string HomeChannel;


	// The client and server token is used for hashing data
	DWORD ClientToken;
	DWORD ServerToken;

	// Whether or not it is verbose (verbose is for debugging info)
	bool bVerbose;

	// Whether or not we are connected and logged on
	bool bConnected;
	bool bLoggedOn;

	//static const Port = 6112;
	virtual void DataSent(char *Data, DWORD Length);
	virtual void DataRecieved(char *Data, DWORD Length);
	virtual void ErrorFunc(const char *Description, DWORD Code);
	virtual void Connected();
	virtual void Disconnected();

	// This send out the username and password.
	void SendUserInfo();

	// These are specific functions for processing battle.net packets.
	// By default, they display the relevant information from the packet to stdout and
	// send out any necessary response.
	virtual void ProcessSID_PING(Buffer Data, DWORD Length);
	virtual void ProcessSID_AUTH_INFO(Buffer Data, DWORD Length);
	virtual void ProcessSID_AUTH_CHECK(Buffer Data, DWORD Length);
	virtual void ProcessSID_LOGONRESPONSE(Buffer Data, DWORD Length);
	virtual void ProcessSID_CHATEVENT(Buffer Data, DWORD Length);
	virtual void ProcessSID_ENTERCHAT(Buffer Data, DWORD Length);
	virtual void ProcessSID_READUSERDATA(Buffer Data, DWORD Length);


	// These are the functions that should be overwritten to make a new bot
	// This should be left blank, it happens when the connection is initialized, but before
	// any data is sent
	virtual void ConnectedToBNet() = 0;
	// This occurs after the user successfully logs in.
	// I would recommend joining a home channel, setting the caption to connected, etc.
	virtual void LoggedIn() = 0;
	// This happens after losing a connection to battle.net.  I would suggest reconnecting
	// and displaying notification to the user.
	virtual void DisconnectedFromBNet() = 0;

	// These are error functions.  I would suggest displaying an error to the user
	// This is called when there is a winsock error, it would be a good place to close
	// the socket and reconnect
	virtual void WinsockError(string ErrorMessage, DWORD ErrorCode) = 0;
	// This indicates a hashfile is missing.  It should display an error to the user and
	// abort the connection
	virtual void NoHashFile(string FileRequired, string Filename) = 0;
	// This indicates that ix86ver0.dll, ix86ver1.dll, etc. is missing.  See NoHashFile().
	virtual void NoIX86File(string Filename) = 0;
	// The password was not accepted.  I would suggest prompting for a password and reconnecting
	virtual void PasswordRejected() = 0;
	// The version check failed.  I would suggest informing the user that a new version is required
	virtual void FailedVersionCheck(DWORD Code, string ExtraData) = 0;
	// This occurs when an unknown packet is recieved.  I would recommend display a notification
	// and continuing on normally.
	virtual void RecievedUnknownPacket(BYTE Code, Buffer Data, DWORD Length) = 0;
	// This occurs if the user has an obviously invalid cdkey (it was picked up by the hashing
	// function.. FailedVersionCheck() occurs if battle.net rejects the key)
	virtual void InvalidCDKey() = 0;

	// This occurs when a ping is recieved, and after battle.net responds to it.
	virtual void Pinged() = 0;
	// This occurs when the password is accepted by battle.net
	virtual void PasswordAccepted() = 0;
	// This occurs when the user passes the version check
	virtual void PassedVersionCheck() = 0;


	// This occurs when a user joins the channel
	virtual void UserJoinedChannel(DWORD Flags, DWORD Ping, string User, string StatString) = 0;
	// This occurs when somebody is already in a channel that the bot joined.  StatString is an
	// english version of the user's statstring.
	virtual void UserInChannel(DWORD Flags, DWORD Ping, string User, string StatString) = 0;
	// This occurs when somebody leaves the channel
	virtual void UserLeftChannel(DWORD Flags, DWORD Ping, string User, string StatString) = 0;
	// This occurs when a whisper is recieved
	virtual void WhisperRecieved(DWORD Flags, DWORD Ping, string User, string Message) = 0;
	// This occurs when somebody chats with a normal chat command
	virtual void ChatRecieved(DWORD Flags, DWORD Ping, string User, string Message) = 0;
	// This occurs when an error occurs on battle.net (like invalid command, etc)
	virtual void BNetError(string Message) = 0;
	// This occurs when the bot joins a new channel
	virtual void JoinedChannel(DWORD Flags, string ChannelName) = 0;
	// This occurs when a user's flags change.
	// This is useful if the bot is currently in the void, since that will indicate an 
	// invisible user
	virtual void FlagsChanged(DWORD Flags, DWORD Ping, string User, string Message) = 0;
	// This occurs when a whisper is sent out
	virtual void WhisperSent(DWORD Flags, DWORD Ping, string User, string Message) = 0;
	// This occurs when an info message is recieved
	virtual void InfoRecieved(string Message) = 0;
	// This occurs when the bot begins ignoring a user
	virtual void IgnoringUser(string User, string Message) = 0;
	// This occurs when the bot stops ignoring a user
	virtual void NotIgnoringUser(string User, string Message) = 0;
	// This occurs when a user emotes
	virtual void Emote(DWORD Flags, DWORD Ping, string User, string Message) = 0;
	// This occurs when an unknown event occurs (never, ideally, but who knows?)
	virtual void UnknownEvent(DWORD Flags, DWORD Ping, string User, string Message) = 0;
	// This occurs when banned from a channel
	virtual void BannedFromChannel() = 0;
	// This occurs when kicked from a channel
	virtual void KickedFromChannel() = 0;
	// This occurs when a profile request is recieved
	virtual void ProfileRecieved(string Name, string Sex, string Age, string Location, string Description) = 0;
	// This occurs when a record data request is recieved (yes, it's big.. meh..)
	virtual void RecDataRecieved(string Name, string Sex, string Age, string Location, string Description,
							string BWWins, string BWLosses, string BWDisconnects, string BWLastGame,
							string BWLWins, string BWLLosses, string BWLDisconnects, string BWLRating, string BWLLastGame,
							string SCWins, string SCLosses, string SCDisconnects, string SCLastGame,
							string SCLWins, string SCLLosses, string SCLDisconnects, string SCLRating, string SCLLastGame,
							string WC2Wins, string WC2Losses, string WC2Disconnects, string WC2LastGame,
							string WC2LWins, string WC2LLosses, string WC2LDisconnects, string WC2LRating, string WC2LLastGame) = 0;

public:
	BinaryBot();
	
	// Functions for setting local variables
	void SetCDKey(string CDKey);
	void SetProduct(string Product);
	void SetStarcraftPath(string path);
	void SetStormPath(string path);
	void SetBattlePath(string path);
	void SetUsername(string Username);
	void SetPassword(string Password);
	void SetHomeChannel(string HomeChannel);
	string GetChannel();

	// A special recieve function designed for Battle.net packets.  Calls the appropriate subclass function.
	virtual bool Recv();

	// Connect to battle.net on the specified server
	bool Connect(string Server, WORD Port);
	// This adds the header to the packet and sends it to battle.net
	bool SendBNetPacket(BYTE Code, Buffer Packet);
	// This sends a chat packet
	bool SendAscii(string Text);
	// This sends out a standard profile request
	void SendProfileRequest(string User);
	// This gets extended record data, including profile
	void SendRecDataRequest(string User);
};


#endif