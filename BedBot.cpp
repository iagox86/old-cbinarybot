#include "BedBot.h"

BedBot::BedBot()
{
	SetConsoleTitle("Bedbot Version 1.0 by iago - Not Connected");
	STDOUT = GetStdHandle(STD_OUTPUT_HANDLE);
}

void BedBot::SetColor(WORD FG, WORD BG)
{
	SetConsoleTextAttribute(this->STDOUT, FG | (BG << 16));
}


// This should be left blank, it happens when the connection is initialized, but before
// any data is sent
void BedBot::ConnectedToBNet()
{
	SetColor(WHITE);
	cout << "You have successfully connected to ";
	SetColor(YELLOW);
	cout << this->Server; 
	SetColor(WHITE);
	cout << " on port ";
	SetColor(YELLOW);
	cout << this->Port << endl;
}

// This occurs after the user successfully logs in.
// I would recommend joining a home channel, setting the caption to connected, etc.
void BedBot::LoggedIn()
{
	SetColor(WHITE);
	cout << "Successfully logged on with account ";
	SetColor(YELLOW);
	cout << this->Username << endl << endl;

	SendAscii("/join op iago.");
}

// This happens after losing a connection to battle.net.  I would suggest reconnecting
// and displaying notification to the user.
void BedBot::DisconnectedFromBNet()
{
	SetColor(RED);
	cout << "Lost connection to battle.net.  Attempting to reconnect..." << endl;
	this->Close();
	this->Connect(Server, Port);
}


// These are error functions.  I would suggest displaying an error to the user
// This is called when there is a winsock error, it would be a good place to close
// the socket and reconnect
void BedBot::WinsockError(string ErrorMessage, DWORD ErrorCode)
{
	SetColor(RED);
	cout << ErrorMessage << endl;
	cout << "Winsock Error #" << ErrorCode << ": " << BinaryBot::GetErrorString(ErrorCode) << endl;
	cout << "Fatal error: exiting" << endl;
	SetColor(WHITE);
	system("pause");
	exit(1);
}

// This indicates a hashfile is missing.  It should display an error to the user and
// abort the connection
void BedBot::NoHashFile(string FileRequired, string Filename)
{
}

// This indicates that ix86ver0.dll, ix86ver1.dll, etc. is missing.  See NoHashFile().
void BedBot::NoIX86File(string Filename)
{
}

// The password was not accepted.  I would suggest prompting for a password and reconnecting
void BedBot::PasswordRejected()
{
	SetColor(RED);
	cout << "Your password was rejected by battle.net!" << endl;
	SetColor(WHITE);
	system("pause");
	exit(1);
}

// The version check failed.  I would suggest informing the user that a new version is required
void BedBot::FailedVersionCheck(DWORD Code, string ExtraData)
{
}

// This occurs when an unknown packet is recieved.  I would recommend display a notification
// and continuing on normally.
void BedBot::RecievedUnknownPacket(BYTE Code, Buffer Data, DWORD Length)
{
}

// This occurs if the user has an obviously invalid cdkey (it was picked up by the hashing
// function.. FailedVersionCheck() occurs if battle.net rejects the key)
void BedBot::InvalidCDKey()
{

}


// This occurs when a ping is recieved, and after battle.net responds to it.
void BedBot::Pinged()
{
	SetColor(GREY);
	cout << "Ping recieved from battle.net." << endl;
}

// This occurs when the password is accepted by battle.net
void BedBot::PasswordAccepted()
{
}

// This occurs when the user passes the version check
void BedBot::PassedVersionCheck()
{
	SetColor(WHITE);
	cout << "Successfully passed version check!" << endl;
}



// This occurs when a user joins the channel
void BedBot::UserJoinedChannel(DWORD Flags, DWORD Ping, string User, string StatString)
{
	SetColor(GREEN);
	cout << User << " has joined the channel with Ping " << Ping << " and flags " << Flags << ", " << StatString << endl;
}

// This occurs when somebody is already in a channel that the bot joined
void BedBot::UserInChannel(DWORD Flags, DWORD Ping, string User, string StatString)
{
	SetColor(GREEN);
	cout << User << " is in the channel, with Ping " << Ping << " and flags " << Flags << ", " << StatString << endl;
}

// This occurs when somebody leaves the channel
void BedBot::UserLeftChannel(DWORD Flags, DWORD Ping, string User, string StatString)
{
	SetColor(GREEN);
	cout << User << " has left the channel (Ping " << Ping << " and flags " << Flags << ")" << endl;
}

// This occurs when a whisper is recieved
void BedBot::WhisperRecieved(DWORD Flags, DWORD Ping, string User, string Message)
{
	SetColor(WHITE);
	cout << "<From: ";
	SetColor(YELLOW);
	cout << User;
	SetColor(WHITE);
	cout << "> ";
	SetColor(DARK_WHITE);
	cout << Message << endl;
}

// This occurs when somebody chats with a normal chat command
void BedBot::ChatRecieved(DWORD Flags, DWORD Ping, string User, string Message)
{
	SetColor(YELLOW);
	cout << "<" << User << "> ";
	SetColor(WHITE);
	cout << Message << endl;
}

// This occurs when an error occurs on battle.net (like invalid command, etc)
void BedBot::BNetError(string Message)
{
	SetColor(RED);
	cout << Message << endl;
}

// This occurs when the bot joins a new channel
void BedBot::JoinedChannel(DWORD Flags, string ChannelName)
{
	SetColor(GREEN);
	cout << "Joining channel: ";
	SetColor(WHITE);
	cout << ChannelName << endl;
}

// This occurs when a user's flags change.
// This is useful if the bot is currently in the void, since that will indicate an 
// invisible user
void BedBot::FlagsChanged(DWORD Flags, DWORD Ping, string User, string Message)
{
	SetColor(YELLOW);
	cout << "Flags changed for " << User << " (";
	SetColor(WHITE);
	if(Flags & OPERATOR_FLAG)
		cout << "OPERATOR,";
	if(Flags & SPEAKER_FLAG)
		cout << "SPEAKER,";
	if(Flags & SYSTEMADMIN_FLAG)
		cout << "SYSTEMADMIN,";
	if(Flags & CHAT_FLAG)
		cout << "CHAT,";
	if(Flags & SQUELCHED_FLAG)
		cout << "SQUELCHED,";
	if(Flags & SPECTATOR_FLAG)
		cout << "SPECTATOR,";
	SetColor(YELLOW);
	cout << ")" << endl;

}

// This occurs when a whisper is sent out
void BedBot::WhisperSent(DWORD Flags, DWORD Ping, string User, string Message)
{
	SetColor(WHITE);
	cout << "<To: ";
	SetColor(YELLOW);
	cout << User;
	SetColor(WHITE);
	cout << "> ";
	SetColor(DARK_WHITE);
	cout << Message << endl;
}

// This occurs when an info message is recieved
void BedBot::InfoRecieved(string Message)
{
	SetColor(YELLOW);
	cout << Message << endl;
}

// This occurs when the bot begins ignoring a user
void BedBot::IgnoringUser(string User, string Message)
{
}

// This occurs when the bot stops ignoring a user
void BedBot::NotIgnoringUser(string User, string Message)
{
}

// This occurs when a user emotes
void BedBot::Emote(DWORD Flags, DWORD Ping, string User, string Message)
{
	SetColor(YELLOW);
	cout << "<" << User << " " << Message << endl;
}

// This occurs when an unknown event occurs (never, ideally, but who knows?)
void BedBot::UnknownEvent(DWORD Flags, DWORD Ping, string User, string Message)
{
}

void BedBot::KickedFromChannel()
{
	string Rejoin;
	Rejoin = "/join ";
	Rejoin = Rejoin + Channel;
	SendAscii(Rejoin);
}

void BedBot::BannedFromChannel()
{
	string GoHome;
	GoHome = "/join ";
	GoHome = GoHome + HomeChannel;
	SendAscii(GoHome);
}

// This occurs when a profile request is recieved
void BedBot::ProfileRecieved(string Name, string Sex, string Age, string Location, string Description)
{
	SetColor(GREEN);
	cout << "Profile request recieved for " << Name << endl;
	SetColor(YELLOW);
	cout << "Sex: ";
	SetColor(WHITE);
	cout << Sex << endl;

	SetColor(YELLOW);
	cout << "Age: ";
	SetColor(WHITE);
	cout << Age << endl;

	SetColor(YELLOW);
	cout << "Location: ";
	SetColor(WHITE);
	cout << Location << endl;

	SetColor(YELLOW);
	cout << "Description: ";
	SetColor(WHITE);
	cout << Description << endl;
}

// This occurs when a record data request is recieved (yes, it's big.. meh..)
void BedBot::RecDataRecieved(string Name, string Sex, string Age, string Location, string Description,
							string BWWins, string BWLosses, string BWDisconnects, string BWLastGame,
							string BWLWins, string BWLLosses, string BWLDisconnects, string BWLRating, string BWLLastGame,
							string SCWins, string SCLosses, string SCDisconnects, string SCLastGame,
							string SCLWins, string SCLLosses, string SCLDisconnects, string SCLRating, string SCLLastGame,
							string WC2Wins, string WC2Losses, string WC2Disconnects, string WC2LastGame,
							string WC2LWins, string WC2LLosses, string WC2LDisconnects, string WC2LRating, string WC2LLastGame)
{
	if(BWWins == "") BWWins = "0";
	if(BWLosses == "") BWLosses = "0";
	if(BWDisconnects == "") BWDisconnects = "0";
	if(BWLWins == "") BWLWins = "0";
	if(BWLLosses == "") BWLLosses = "0";
	if(BWLDisconnects == "") BWLDisconnects = "0";
	if(SCWins == "") SCWins = "0";
	if(SCLosses == "") SCLosses = "0";
	if(SCDisconnects == "") SCDisconnects = "0";
	if(SCLWins == "") SCLWins = "0";
	if(SCLLosses == "") SCLLosses = "0";
	if(SCLDisconnects == "") SCLDisconnects = "0";
	if(WC2Wins == "") WC2Wins = "0";
	if(WC2Losses == "") WC2Losses = "0";
	if(WC2Disconnects == "") WC2Disconnects = "0";
	if(WC2LWins == "") WC2LWins = "0";
	if(WC2LLosses == "") WC2LLosses = "0";
	if(WC2LDisconnects == "") WC2LDisconnects = "0";
	if(SCLRating == "") SCLRating = "(n/a)";
	if(BWLRating == "") BWLRating = "(n/a)";
	if(WC2LRating == "") WC2LRating = "(n/a)";

	SetColor(GREEN);
	cout << "Profile request recieved for " << Name << endl;
	SetColor(YELLOW);
	cout << "Sex: ";
	SetColor(WHITE);
	cout << Sex << endl;

	SetColor(YELLOW);
	cout << "Age: ";
	SetColor(WHITE);
	cout << Age << endl;

	SetColor(YELLOW);
	cout << "Location: ";
	SetColor(WHITE);
	cout << Location << endl;

	SetColor(YELLOW);
	cout << "Description: ";
	SetColor(WHITE);
	cout << Description << endl;

	SetColor(YELLOW);
	cout << "Starcraft W/L/D: ";
	SetColor(WHITE);
	cout << SCWins << "/" << SCLosses << "/" << SCDisconnects << endl;
	SetColor(YELLOW);
	cout << "Starcraft Ladder W/L/D: ";
	SetColor(WHITE);
	cout << SCLWins << "/" << SCLLosses << "/" << SCDisconnects << endl;
	SetColor(YELLOW);
	cout << "Starcraft ladder rating: ";
	SetColor(WHITE);
	cout << SCLRating << endl;

	SetColor(YELLOW);
	cout << "Brood War W/L/D: ";
	SetColor(WHITE);
	cout << BWWins << "/" << BWLosses << "/" << BWDisconnects << endl;
	SetColor(YELLOW);
	cout << "Brood War Ladder W/L/D: ";
	SetColor(WHITE);
	cout << BWLWins << "/" << BWLLosses << "/" << BWDisconnects << endl;
	SetColor(YELLOW);
	cout << "Brood war ladder rating: ";
	SetColor(WHITE);
	cout << BWLRating << endl;

	SetColor(YELLOW);
	cout << "Warcraft 2 W/L/D: ";
	SetColor(WHITE);
	cout << WC2Wins << "/" << WC2Losses << "/" << WC2Disconnects << endl;
	SetColor(YELLOW);
	cout << "Warcraft 2 Ladder W/L/D: ";
	SetColor(WHITE);
	cout << WC2LWins << "/" << WC2LLosses << "/" << WC2Disconnects << endl;
	SetColor(YELLOW);
	cout << "Warcraft 2 ladder rating: ";
	SetColor(WHITE);
	cout << WC2LRating << endl;


}
