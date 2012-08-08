// This is going to be a console-based test of my BinaryBot class that I will use before
// writing a GUI for it.

#include <iostream>
#include <string>
#include <stdlib.h>
#include "BedBot.h"
#include "ParseStatString.h"

using namespace std;

int main(int argc, char *argv[]) // Add the parameters to main just to bug Sean.
{
	LoadLibrary("battle.snp");

	DWORD HashBuffer[5];
	DWORD Data[13] = { 0x1234567, 0x7654321, 0xbaaad };

	BNetHashing::GetRealHash(HashBuffer, (DWORD*)Data, 12);
	printf("Hash = %08x %08x %08x %08x %08x\n\n", HashBuffer[0], HashBuffer[1], HashBuffer[2], HashBuffer[3], HashBuffer[4]);

	BNetHashing::GetHash(HashBuffer, (DWORD*)Data, sizeof(DWORD) * 3);
	printf("Hash = %08x %08x %08x %08x %08x\n\n", HashBuffer[0], HashBuffer[1], HashBuffer[2], HashBuffer[3], HashBuffer[4]);




	// Set up our variables
	//BinaryBot *BB = new BedBot;

	//cout << "Username: ";
	//cin >> Username;
	//cout << "Password: ";
	//cin >> Password;

	//BB->SetProduct("STAR");
	//BB->SetStarcraftPath("starcraft.exe");
	//BB->SetStormPath("storm.dll");
	//BB->SetBattlePath("battle.snp");
	//BB->SetCDKey("1234567890123");
	//BB->SetCDKey("1234567890123");
	//BB->SetHomeChannel("Op FuxDux");
	//BB->SetUsername(Username);
	//BB->SetPassword(Password);
	//BB->Connect("useast.battle.net", 6112);

	//while(1)
	{
	//	BB->Recv();
	}

	system("pause");
	return 0;
}
