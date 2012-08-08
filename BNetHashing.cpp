#include "BNetHashing.h"

const DWORD DecodeCDKeyFunc = 0x19019AB0;
const DWORD HashFunction = 0x19012400;

void DecodeCDKey(DWORD *Product, DWORD *CDKeyVal1, DWORD *CDKeyVal2, const char *CDKey)
{

	__asm
	{
		push	ecx
		push	edx
		mov		ecx, CDKey
		mov		edx, Product
		push	CDKeyVal2
		push	CDKeyVal1
		call	DecodeCDKeyFunc
		pop		edx
		pop		ecx
	}
}
int main()
{
	if(LoadLibrary("C:\\program files\\starcraft\\storm.dll") == NULL)
	{
		printf("unable to open storm\n");
		system("pause");
		return 1;
	}
	if(LoadLibrary("C:\\program files\\starcraft\\battle.snp") == NULL)
	{
		printf("unable to open battle.snp\n");
		system("pause");
		return 1;
	}
	DWORD product = -1;
	DWORD cdkeyval1 = -1;
	DWORD cdkeyval2 = -1;
	char *cdkey = "1234567890123";

	DecodeCDKey(&product, &cdkeyval1, &cdkeyval2, cdkey);

	printf("Product: %u\nval1: %u\nval2: %u\n");

	system("pause");
}

