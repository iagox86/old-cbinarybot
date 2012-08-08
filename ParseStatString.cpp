#include "ParseStatString.h"

#include <string>
#include <strstream>
using namespace std;

const char *Races[] = 
	{ "Unknown", 
	"Amazon", 
	"Sorceress", 
	"Necromancer",
	"Paladin",
	"Barbarian",
	"Druid",
	"Assassin",
	"Unknown",
	"Unknown (Grey)",
	"Unknown (Grey)",
	"Diablo 1",
	"Diablo 1",
	"Diablo 1",
	"Starcraft Marine",
	"Brood War Medic",
	"Warcraft II Grunt",
	"Blizzard Rep",
	"Moderator",
	"Sysop",
	"Referee",
	"Chat",
	"Speaker" };



string ParseStatString::GetStatString(char *SS)
{
	string Product = SS;
	strstream Return;
	Product.resize(4);
	DWORD a, b, c, d, e, f, g, h, i, j;

	if(Product == "RHSS")
	{
		return "using Starcraft Shareware";
	}
	
	
	if(Product == "RATS" || Product == "PXES" || Product == "NB2W")
	{
		if(Product[0] == 'R')
			Return << "using Starcraft";
		else if(Product[0] == 'P')
			Return << "using Starcraft Broodwar";
		else
			Return << "using Warcraft II";

		if(sscanf(SS + 5, "%d %d %d %d %d %d %d %d %d", &a, &b, &c, &d, &e, &f, &g, &h, &i, &j) != 9)
		{
			Return << (BYTE)0;
			return Return.str();
		}
		else
		{
			if(d)
				Return << " spawn";
			if(a > 0)
			{
				Return << " (" << c << " wins, ladder: " << a << ", ladder max: " << f << ")";
			}
			else
			{
				Return << " (" << c << " wins)";
			}
			Return << (BYTE) 0;
			return Return.str();
		}
	}

	if(Product == "PX2D" || Product == "VD2D")
	{
		if(Product == "PX2D")
		{
			Return << "using Diablo 2: LoD";
		}
		else
		{
			Return << "using Diablo 2";
		}
		
		SS += 4;
		
		char *CharName = strchr(SS, ',');
		// Make sure there was a comma in it
		if(CharName == NULL)
		{
			Return << (BYTE) '\0';
			return Return.str();
		}
		// Set the last character in the realm to NULL
		*CharName = '\0';
		// Put the realm into a string
		string Realm = SS;
		// Go to the first character of the character name
		CharName++;
		
		// Get the first part of the stats section
		char *Stats = strchr(CharName, ',');
		// If no comma was found, return what we have
		if(Stats == NULL || strlen(Stats + 1) != 33)
		{
			Return << (BYTE) '\0';
			return Return.str();
		}
		// Set the end of CharName to NULL
		*Stats = '\0';
		Stats++;
		string Character = CharName;
		
		// Ok, we must be good, we will parse the rest of it now
		BYTE Level = Stats[25];
		BYTE Race = Stats[13];
		BOOL Hardcore = (BOOL)(Stats[26] & 4);
		BOOL Dead = (BOOL)(Stats[26] & 8);
		BYTE Helmet = Stats[2];
		BYTE Weapon = Stats[7];
		BYTE Shield = Stats[9];
	
		if(Race > 22)
		{
			Race = 0;
		}
		string strRace = Races[Race];

		string strHelm;
		switch(Helmet)
		{
		case 4:
		case 57:
			strHelm = "Cap";
			break;
		case 5:
		case 58:
			strHelm = "Skullcap";
			break;
		case 6:
		case 59:
			strHelm = "Helm";
			break;
		case 7:
		case 60:
			strHelm = "Fullhelm";
			break;
		case 8:
		case 61:
			strHelm = "Greathelm";
			break;
		case 10:
		case 63:
			strHelm = "Mask";
			break;
		case 40:
		case 82:
			strHelm = "Bonehelm";
			break;
		case 89:
			strHelm = "Fanged Helm";
			break;
		case 90:
			strHelm = "Warhelm";
			break;
		case 91:
			strHelm = "Winged Helm";
			break;
		default:
			strHelm = "No helm (";
			strHelm += (int) Helmet;
			strHelm += ")";
			break;
		}

		string strWeapon;

		switch(Weapon)
		{
		case 4:
			strWeapon = "Hatchet/Waraxe"; //
			break;
		case 5:
			strWeapon = "Axe"; //
			break;
		case 6:
			strWeapon = "Large Axe 6";
			break;
		case 7:
			strWeapon = "Large Axe 7";
			break;
		case 8:
			strWeapon = "Great Axe";
			break;
		case 9:
			strWeapon = "Wand 9";
			break;
		case 10:
			strWeapon = "Wand 10";
			break;
		case 11:
			strWeapon = "Wand 11";
			break;
		case 12:
			strWeapon = "Spiked Club"; //
			break;
		case 13:
			strWeapon = "Scepter"; //
			break;
		case 14:
			strWeapon = "Hammer";
			break;
		case 15:
			strWeapon = "Flail";
			break;
		case 16:
			strWeapon = "Maul";
			break;
		case 17:
			strWeapon = "Short Sword";
			break;
		case 18:
			strWeapon = "Scimitar/Saber"; //
			break;
		case 19:
			strWeapon = "Warsword"; //
			break;
		case 20:
			strWeapon = "Crystal Sword";
			break;
		case 21:
			strWeapon = "Sword 21";
			break;
		case 22:
			strWeapon = "Sword 22";
			break;
		case 23:
			strWeapon = "Sword 23";
			break;
		case 24:
			strWeapon = "Sword 24";
			break;
		case 25:
			strWeapon = "Dagger"; //
			break;
		case 26:
			strWeapon = "Dirk/Kris"; //
			break;
		case 27:
			strWeapon = "Unk 27";
			break;
		case 28:
			strWeapon = "Unk 28";
			break;
		case 29:
			strWeapon = "Unk 29";
			break;
		case 30:
			strWeapon = "Spear"; //
			break;
		case 31:
			strWeapon = "Trident";
			break;
		case 32:
			strWeapon = "Spetum";
			break;
		case 33:
			strWeapon = "Pike";
			break;
		case 34:
			strWeapon = "Bardiche/Halberd"; //
			break;
		case 35:
			strWeapon = "Sickle";
			break;
		case 36:
			strWeapon = "Poleaxe";
			break;
		case 37:
			strWeapon = "Staff 37";
			break;
		case 38:
			strWeapon = "Staff 38";
			break;
		case 39:
			strWeapon = "Staff 39";
			break;
		case 40:
			strWeapon = "Staff 40";
			break;
		case 49:
			strWeapon = "Unk 49";
			break;
		case 50:
			strWeapon = "Unk 50";
			break;
		case 56:
			strWeapon = "Unk 56";
			break;
		case 121:
			strWeapon = "Unk 121";
			break;
		case 122:
			strWeapon = "Unk 122";
			break;
		case 123:
			strWeapon = "Unk 123";
			break;
		case 124:
			strWeapon = "Unk 124";
			break;
		case 125:
			strWeapon = "Poison Potion";
			break;
		case 126:
			strWeapon = "Fulmigating Potion";
			break;
		case 127:
			strWeapon = "Potion 3";
			break;
		case 128:
			strWeapon = "Potion 4";
			break;
		case 129:
			strWeapon = "Potion 5";
			break;
		default:
			strWeapon = "No weapon (";
			strWeapon += (int)Weapon;
			strWeapon += ")";
		}

		string strShield;

		switch(Shield)
		{
		case 79:
			strShield = "Small shield";
			break;
		case 80:
			strShield = "Buckler";
			break;
		case 81:
			strShield = "Kite Shield";
			break;
		case 82:
			strShield = "Tower Shield";
			break;
		case 84:
			strShield = "Bone Shield";
			break;
		case 85:
			strShield = "Spiked Shield";
			break;
		case 92:
			strShield = "Rondache";
			break;
		case 94:
			strShield = "Crown Shield";
			break;
		default:
			strShield = "No Shield (";
			strShield += (int)Shield;
			strShield += ")";
		}

		// Now that we have all the pieces, build the packet
		Return 
			<< " (" 
			<< Character 
			<< ", a level " 
			<< (int)Level 
			<< " " 
			<< Races[Race]
			<< " using a " 
			<< strWeapon
			<< ", a "
			<< strHelm
			<< ", and a "
			<< strShield;

		if(Hardcore)
			Return << ", Hardcore";
		if(Dead)
			Return << ", Dead";

		Return << ")" << (BYTE) '\0';

		return Return.str();
	}

	if(Product == "LTRD" || Product == "RHSD")
	{
		if(Product == "LTRD")
			Return << "using Diablo";
		else
			Return << "using Diablo Shareware";

		if(sscanf(SS + 5, "%d %d %d %d %d %d %d %d %d", &a, &b, &c, &d, &e, &f, &g, &h, &i, &j) != 9)
		{
			Return << (BYTE) '\0';
			return Return.str();
		}
		
		Return << ", a Level " << a;
		if(b == 0)
		{
			Return << " warrior";
		}
		else if(b == 1)
		{
			Return << " rogue";
		}
		else if(b == 2)
		{
			Return << " sorcerer";
		}
		
		Return << " with " << c << " dots, " << d << " strength, " << e << " magic, " << f << " dexterity, " << g << " vitality, and " << h << " gold" << (BYTE) '\0';
		return Return.str();
	}

	if(Product == "TAHC")
	{
		Return << "using a Chat Bot" << (BYTE) '\0';
	}

	if(Product == "3RAW")
	{
		Return << "using Warcraft 3" << (BYTE) 0;
		return Return.str();
	}


			

	Return << "with an unrecognized statstring: " << SS << '\0';
	return Return.str();
}


