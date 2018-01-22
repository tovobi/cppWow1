#pragma once

#include <string>

class WowObject {
public:
	// general properties
	unsigned long long guid = 0;
	unsigned long long targetGuid = 0;
	//WowObject target;
	unsigned long long summonedBy = 0;
	float xPos = 0;
	float yPos = 0;
	float zPos = 0;
	std::string wowClass = "None";
	double distance = 0;
	unsigned int factionTemplate = 0;
	float factionOffset = 0;
	//WowVector3d vector3d;
	float rotation = 0;
	unsigned int objBaseAddress = 0;
	unsigned int unitFieldsAddress = 0;
	short type = 0;
	unsigned int dodged = 0;
	unsigned long long guidOfAutoAttackTarget;
	bool isInCombat = false;
	bool isFleeing = false;
	bool isStunned = false;
	bool cantMove = false;
	bool isConfused = false;
	bool hasBreakableCc = false;
	//List<Auras> buffList = null;
	//List<Auras> debuffList = null;
	unsigned int channelSpell = 0;
	unsigned int castSpell = 0;
	double playerIsFacingTo = 0;
	unsigned int unitFlags = 0;
	unsigned int dynamicFlags = 0;
	std::string name = "";
	bool isHostile = false; //we havent found a way yet to determine the hostile, thats just for a quick and dirty way for the calculation of # of targets
	unsigned int tempBuffStacks = 0;
	std::string tempNextSpell = "";
	unsigned int tempTargetPrio = 0;

	// more specialised properties (player or mob)
	unsigned int health = 0;
	unsigned int maxHealth = 0;
	unsigned int healthPercent = 100;
	unsigned int mana = 0; // mana, rage and energy will all fall under energy.
	unsigned int maxMana = 0;
	unsigned int manaPercent = 100;
	unsigned int rage = 0; // mana, rage and energy will all fall under energy.
	unsigned int energy = 0;
	unsigned int level = 0;

	unsigned int gameObjectType = 0;
	//WowObject next = null;	

	WowObject()
	{

	}
};
