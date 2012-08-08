// BNetHashing
// by Ron
// Created 07/11/2003
//
// This class has static functions used for calculating the hash and the cdkeyval buffer
// NOTE: Battle.snp and Storm.dll must be loaded
#ifndef BNETHASHING_H
#define BNETHASHING_H

#include <iostream>
#include <string>
#include <windows.h>

using namespace std;

class BNetHashing
{
private:
	// Fills the hashbuffer with initial random-ish data
	static void GetInitialHash(DWORD Buffer[5]);
public:
	// This fills in the BufferForExeInfo, VersionHash, and CheckSum based on the Starcraft.exe,
	// battle.snp, and storm.dll files
	static BOOL GetVersionInfo(const char *StormPath, const char *BattlePath, const char *StarcraftPath, HMODULE CR, const char *ChecksumFormula, char *BufferForExeInfo, DWORD *VersionHash, DWORD *CheckSum, string IX86File);
	// This fills in the Product, CDKeyVal1, and CDKeyVal2 based on the CDKey
	// (Works for Starcraft, Brood War, and Warcraft II)
	static bool DecodeCDKey(DWORD *Product, DWORD *CDKeyVal1, DWORD *CDKeyVal2, const char *CDKey);
	// This fills in the HashBuffer based on HashData
	static void GetHash(DWORD HashBuffer[5], DWORD *HashData, DWORD BytesToHash);
	static void GetRealHash(DWORD HashBuffer[5], DWORD *HashData, DWORD BytesToHash);
};

#endif