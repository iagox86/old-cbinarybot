// BedBot.h
// Version 1.0
// by Ron
// Created 07/16/2003
//
// This is a very simple console bot, based on my BinaryBot (v1.0) class.  It 
// basically demonstrates how to fill out each function in a way that works
// and looks nice.

#ifndef BEDBOT_H
#define BEDBOT_H

#include "BinaryBot.h"
#include "Buffer.h"
#include <stdio.h>
using namespace std;

class BedBot : public BinaryBot
{
private:
	HANDLE STDOUT;

public:
	BedBot();

	static const DWORD BLACK = 0;
	static const DWORD DARK_BLUE = 1;
	static const DWORD DARK_GREEN = 2;
	static const DWORD DARK_CYAN = 3;
	static const DWORD DARK_RED = 4;
	static const DWORD DARK_MAGENTA = 5;
	static const DWORD DARK_YELLOW = 6;
	static const DWORD DARK_WHITE = 7;
	static const DWORD GREY = 8;
	static const DWORD BLUE = 9;
	static const DWORD GREEN = 10;
	static const DWORD TEAL = 11;
	static const DWORD RED = 12;
	static const DWORD MAGENTA = 13;
	static const DWORD YELLOW = 14;
	static const DWORD WHITE = 15;
protected:
	// This sets the text's Fore- and Background colors.
	// Always returns 0
	void SetColor(WORD FG, WORD BG = 0);
	// This should be left blank, it happens when the connection is initialized, but before
	// any data is sent
	virtual void ConnectedToBNet();
	// This occurs after the user successfully logs in.
	// I would recommend joining a home channel, setting the caption to connected, etc.
	virtual void LoggedIn();
	// This happens after losing a connection to battle.net.  I would suggest reconnecting
	// and displaying notification to the user.
	virtual void DisconnectedFromBNet();

	// These are error functions.  I would suggest displaying an error to the user
	// This is called when there is a winsock error, it would be a good place to close
	// the socket and reconnect
	virtual void WinsockError(string ErrorMessage, DWORD ErrorCode);
	// This indicates a hashfile is missing.  It should display an error to the user and
	// abort the connection
	virtual void NoHashFile(string FileRequired, string Filename);
	// This indicates that ix86ver0.dll, ix86ver1.dll, etc. is missing.  See NoHashFile().
	virtual void NoIX86File(string Filename);
	// The password was not accepted.  I would suggest prompting for a password and reconnecting
	virtual void PasswordRejected();
	// The version check failed.  I would suggest informing the user that a new version is required
	virtual void FailedVersionCheck(DWORD Code, string ExtraData);
	// This occurs when an unknown packet is recieved.  I would recommend display a notification
	// and continuing on normally.
	virtual void RecievedUnknownPacket(BYTE Code, Buffer Data, DWORD Length);
	// This occurs if the user has an obviously invalid cdkey (it was picked up by the hashing
	// function.. FailedVersionCheck() occurs if battle.net rejects the key)
	virtual void InvalidCDKey();

	// This occurs when a ping is recieved, and after battle.net responds to it.
	virtual void Pinged();
	// This occurs when the password is accepted by battle.net
	virtual void PasswordAccepted();
	// This occurs when the user passes the version check
	virtual void PassedVersionCheck();


	// This occurs when a user joins the channel
	virtual void UserJoinedChannel(DWORD Flags, DWORD Ping, string User, string StatString);
	// This occurs when somebody is already in a channel that the bot joined
	virtual void UserInChannel(DWORD Flags, DWORD Ping, string User, string StatString);
	// This occurs when somebody leaves the channel
	virtual void UserLeftChannel(DWORD Flags, DWORD Ping, string User, string StatString);
	// This occurs when a whisper is recieved
	virtual void WhisperRecieved(DWORD Flags, DWORD Ping, string User, string Message);
	// This occurs when somebody chats with a normal chat command
	virtual void ChatRecieved(DWORD Flags, DWORD Ping, string User, string Message);
	// This occurs when an error occurs on battle.net (like invalid command, etc)
	virtual void BNetError(string Message);
	// This occurs when the bot joins a new channel
	virtual void JoinedChannel(DWORD Flags, string ChannelName);
	// This occurs when a user's flags change.
	// This is useful if the bot is currently in the void, since that will indicate an 
	// invisible user
	virtual void FlagsChanged(DWORD Flags, DWORD Ping, string User, string Message);
	// This occurs when a whisper is sent out
	virtual void WhisperSent(DWORD Flags, DWORD Ping, string User, string Message);
	// This occurs when an info message is recieved
	virtual void InfoRecieved(string Message);
	// This occurs when the bot begins ignoring a user
	virtual void IgnoringUser(string User, string Message);
	// This occurs when the bot stops ignoring a user
	virtual void NotIgnoringUser(string User, string Message);
	// This occurs when a user emotes
	virtual void Emote(DWORD Flags, DWORD Ping, string User, string Message);
	// This occurs when an unknown event occurs (never, ideally, but who knows?)
	virtual void UnknownEvent(DWORD Flags, DWORD Ping, string User, string Message);
	// This occurs when banned from a channel
	virtual void BannedFromChannel();
	// This occurs when kicked from a channel
	virtual void KickedFromChannel();
	// This occurs when a profile request is recieved
	virtual void ProfileRecieved(string Name, string Sex, string Age, string Location, string Description);
	// This occurs when a record data request is recieved (yes, it's big.. meh..)
	virtual void RecDataRecieved(string Name, string Sex, string Age, string Location, string Description,
							string BWWins, string BWLosses, string BWDisconnects, string BWLastGame,
							string BWLWins, string BWLLosses, string BWLDisconnects, string BWLRating, string BWLLastGame,
							string SCWins, string SCLosses, string SCDisconnects, string SCLastGame,
							string SCLWins, string SCLLosses, string SCLDisconnects, string SCLRating, string SCLLastGame,
							string WC2Wins, string WC2Losses, string WC2Disconnects, string WC2LastGame,
							string WC2LWins, string WC2LLosses, string WC2LDisconnects, string WC2LRating, string WC2LLastGame);
};

#endif