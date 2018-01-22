#include "stdafx.h"


struct Pointers
{
public:
	// StaticAddresses
	unsigned int currentTargetGUID = 0x74e2d8;
	unsigned int localPlayerGUID = 0x741e30;
	unsigned int Timestamp = 0xC7B2A0;
	unsigned int textCaretActive = 0x884ca8;
	unsigned int isInGame = 0x00B4B424; // not validated, from :http://www.ownedcore.com/forums/world-of-warcraft/world-of-warcraft-bots-programs/wow-memory-editing/328263-wow-1-12-1-5875-info-dump-thread-post2436167.html#post2436167
	unsigned int stance = 0x00BC6E88; // pure address, dont add baseaddress

};


struct ObjectManager
{
public:
	unsigned int curMgrPointer = 0x00741414;
	unsigned int curMgrOffset = 0xAC;
	unsigned int nextObjectOffset = 0x3c;
	unsigned int firstObjectOffset = 0xAC;
	unsigned int localGUID = 0xC0;
	unsigned int descriptorOffset = 0x8;
	unsigned int curPlayerSpellPtr = 0x00B700F0;
	unsigned int spellHistory = 0xCECAEC;
};