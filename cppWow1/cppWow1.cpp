#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>
#include <windows.h>
#include "wowObject.h"
#include <psapi.h>

using namespace std;

UINT_PTR GetProcessBaseAddress(DWORD processID, HANDLE *processHandle);

int main()
{
	HWND WindowHandle = FindWindow(nullptr, L"World of Warcraft");
	DWORD PID;
	GetWindowThreadProcessId(WindowHandle, &PID);
	HANDLE      processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
	if (processHandle == 0) {
		cout << "Could not open process";
		return 1;
	}

	UINT_PTR BaseAddress = GetProcessBaseAddress(PID, &processHandle);
	UINT_PTR PlayerBaseAddress = (BaseAddress + 0x173D390);
	UINT_PTR PlayerGuidAddress = (BaseAddress + 0x741e30);

	UINT_PTR PlayerBase;
	UINT_PTR PlayerGuid;

	ReadProcessMemory(
		processHandle,
		(void *)PlayerBaseAddress,
		&PlayerBase,
		sizeof(PlayerBase, 0),
		0
	);

	ReadProcessMemory(
		processHandle,
		(void *)PlayerGuidAddress,
		&PlayerGuid,
		sizeof(PlayerGuid, 0),
		0
	);


	cout << "Process base: " << hex << BaseAddress << ", Player Base Address: " << hex << PlayerBaseAddress << ", Actual address: " << hex << PlayerBase << "\n";
	cout << "Process base: " << hex << BaseAddress << ", Player Name Address: " << hex << PlayerGuidAddress << ", Actual address: " << hex << PlayerGuid << "\n";
	CloseHandle(processHandle);
	system("PAUSE");
	return 0;
}


UINT_PTR GetProcessBaseAddress(DWORD processID, HANDLE *processHandle)
{
	DWORD_PTR   baseAddress = 0;

	HMODULE     *moduleArray;
	LPBYTE      moduleArrayBytes;
	DWORD       bytesRequired;

	if (*processHandle)
	{
		if (EnumProcessModulesEx(*processHandle, NULL, 0, &bytesRequired, 0x02))
		{
			if (bytesRequired)
			{
				moduleArrayBytes = (LPBYTE)LocalAlloc(LPTR, bytesRequired);

				if (moduleArrayBytes)
				{
					unsigned int moduleCount;

					moduleCount = bytesRequired / sizeof(HMODULE);
					moduleArray = (HMODULE *)moduleArrayBytes;

					if (EnumProcessModulesEx(*processHandle, moduleArray, bytesRequired, &bytesRequired, 0x02))
					{
						baseAddress = (DWORD_PTR)moduleArray[0];
					}

					LocalFree(moduleArrayBytes);
				}
			}
		}


	}

	return baseAddress;
}



void ScanObj()
{
	unsigned int firstObject = 0;


	vector<WowObject> cachedUnitlist;
	// set our counter variable to 0 so we can begin counting the objects
	int totalWowObjects = 0;
	WowObject currentObject;
	// set our current object as the first in the object manager
	currentObject.objBaseAddress = firstObject;
	while ((currentObject.objBaseAddress & 1) == 0)
	{
		// type independent informations
		try
		{
			totalWowObjects++;
			currentObject.guid = wowMem.ReadUInt64((currentObject.objBaseAddress + (uint)pointers.WowObject.Guid));
		}
		catch (Exception e)
		{
			LogErrors("currentObject: " + e.ToString());
			Console.WriteLine("currentObject: " + e.ToString());
			currentObject.guid = 0; // cant do a break, then my app stops working and cant leave it w/o break and w/o Guid = 0, then app crashes
		}
		if (currentObject.guid != 0)
		{
			try
			{
				//Log.Print("List: " + currentObject.Guid);
				currentObject.Type = (short)(wowMem.ReadUInt((currentObject.objBaseAddress + (uint)pointers.WowObject.Type)));
				switch (currentObject.Type)
				{


				case (short)constants.ObjType.OT_PLAYER: // a player
					currentObject.castSpell = wowMem.ReadUInt((currentObject.objBaseAddress + (uint)pointers.WowObject.CastSpell));
					currentObject.guidOfAutoAttackTarget = wowMem.ReadUInt64((currentObject.objBaseAddress + (uint)pointers.WowObject.GuidOfAutoAttackTarget));
					currentObject.unitFieldsAddress = wowMem.ReadUInt((currentObject.objBaseAddress + (uint)pointers.WowObject.DataPTR));
					currentObject.channelSpell = wowMem.ReadUInt((currentObject.unitFieldsAddress + (uint)descriptors.WoWUnitFields.ChannelSpell));
					currentObject.health = wowMem.ReadUInt((currentObject.unitFieldsAddress + (uint)descriptors.WoWUnitFields.Health));
					currentObject.maxHealth = wowMem.ReadUInt((currentObject.unitFieldsAddress + (uint)descriptors.WoWUnitFields.MaxHealth));
					currentObject.healthPercent = (uint)Math.Floor(((double)currentObject.Health / (double)currentObject.MaxHealth * 100));
					currentObject.mana = wowMem.ReadUInt((currentObject.unitFieldsAddress + (uint)descriptors.WoWUnitFields.Mana));
					currentObject.maxMana = wowMem.ReadUInt((currentObject.unitFieldsAddress + (uint)descriptors.WoWUnitFields.MaxMana));
					currentObject.rage = (uint)Math.Floor((double)wowMem.ReadUInt((currentObject.unitFieldsAddress + (uint)descriptors.WoWUnitFields.Rage)) / 10);
					currentObject.energy = wowMem.ReadUInt((currentObject.unitFieldsAddress + (uint)descriptors.WoWUnitFields.Energy));
					currentObject.factionTemplate = wowMem.ReadUInt((currentObject.unitFieldsAddress + (uint)descriptors.WoWUnitFields.FactionTemplate));
					currentObject.factionOffset = wowMem.ReadUInt((currentObject.unitFieldsAddress + (uint)descriptors.WoWUnitFields.FactionOffset));
					currentObject.level = wowMem.ReadUInt((currentObject.unitFieldsAddress + (uint)descriptors.WoWUnitFields.Level));
					//currentObject.Dodged = wowMem.ReadUInt((currentObject.unitFieldsAddress + (uint)descriptors.WoWUnitFields.Dodged));
					currentObject.xPos = wowMem.ReadFloat((currentObject.objBaseAddress + (uint)pointers.WowObject.X));
					currentObject.yPos = wowMem.ReadFloat((currentObject.objBaseAddress + (uint)pointers.WowObject.Y));
					currentObject.zPos = wowMem.ReadFloat((currentObject.objBaseAddress + (uint)pointers.WowObject.Z));
					pointers.PlayerClass.TryGetValue(wowMem.ReadByte((currentObject.objBaseAddress + (uint)pointers.WowObject.PlayerClass)), out currentObject.Class);
					currentObject.Rotation = wowMem.ReadFloat((currentObject.objBaseAddress + (uint)pointers.WowObject.Facing));
					//currentObject.PlayerIsFacingTo = GetFacingToUnit(currentObject);
					currentObject.vector3d = new WowVector3d(currentObject.XPos, currentObject.YPos, currentObject.ZPos);
					//currentObject.Distance = Math.Round((LocalPlayer.vector3d.Distance(currentObject.vector3d)), 2);
					currentObject.unitFlags = wowMem.ReadUInt((currentObject.unitFieldsAddress + (uint)descriptors.WoWUnitFields.Flags));
					currentObject.dynamicFlags = wowMem.ReadUInt((currentObject.unitFieldsAddress + (uint)descriptors.WoWUnitFields.DynamicFlags));
					currentObject.isHostile = IsHostile(currentObject);
					currentObject.name = PlayerNameFromGuid(currentObject.Guid);
					DecodeUnitFlags(currentObject);
					currentObject.target = new WowObject();
					currentObject.targetGuid = wowMem.ReadUInt64((currentObject.unitFieldsAddress + (uint)descriptors.WoWUnitFields.Target));

					cachedUnitlist.Add(currentObject);
					break;


				}
			}
			catch (Exception e)
			{
				LogErrors("ScanObj() -> switch/case: " + e.ToString());
				Log.Print("ScanObj() -> switch/case\r\n" + e.Message, 4);
			}
		}

		// set the current object as the next object in the object manager
		WowObject tmpObject = currentObject;
		currentObject = new WowObject();
		try
		{
			currentObject.objBaseAddress = wowMem.ReadUInt((tmpObject.objBaseAddress + (uint)pointers.ObjectManager.NextObjectOffset));
		}
		catch (Exception e)
		{
			LogErrors("ScanObj() -> NextObjectOffset: " + e.ToString());
			Log.Print("ScanObj() -> NextObjectOffset\r\n" + e.Message, 4);
		}
	}

	try
	{

		for (WowObject listObj : cachedUnitlist)
		{


			if (listObj.Type == (short)Constants.ObjType.OT_PLAYER || listObj.Type == (short)Constants.ObjType.OT_UNIT)
			{

				//if the currentobj-guid is already in cache as a targetguid, then the corresponding obj will get his target(currentObject)
				List<WowObject> IsTargetOfList = cachedUnitlist.Where(c = > (c.Type == (short)Constants.ObjType.OT_PLAYER || c.Type == (short)Constants.ObjType.OT_UNIT) && c.TargetGuid == listObj.Guid).ToList();
				if (IsTargetOfList.Count > 0)
				{
					IsTargetOfList.ForEach(c = > c.Target = listObj);
				}
				if (listObj.Guid == LocalPlayer.Guid)
				{
					LocalPlayer = listObj; // Copy whole listObj to LocalPlayer
				}
			}
			if (listObj.Type == (short)Constants.ObjType.OT_PLAYER || listObj.Type == (short)Constants.ObjType.OT_UNIT || listObj.Type == (short)Constants.ObjType.OT_GAMEOBJ)
			{
				if (listObj.vector3d != null && LocalPlayer.vector3d != null)
				{
					listObj.Distance = Math.Round((LocalPlayer.vector3d.Distance(listObj.vector3d)), 2);
				}
				else
				{
					listObj.Distance = 999;
				}
				listObj.PlayerIsFacingTo = GetFacingToUnit(listObj);

			}
			// scan the auras of unit only i unit is near
			if (listObj.Distance <= 30 && (listObj.Type == (short)Constants.ObjType.OT_UNIT || listObj.Type == (short)Constants.ObjType.OT_PLAYER))
			{

				listObj.Dodged = wowMem.ReadUInt((listObj.unitFieldsAddress + (uint)descriptors.WoWUnitFields.Dodged));
				listObj.BuffList = GetUnitBuffs(listObj);

				listObj.DebuffList = GetUnitDebuffs(listObj);

				listObj.HasBreakableCc = listObj.DebuffList.Exists(c = > c.Name == "Polymorph" || c.Name == "Sap" || c.Name == "Blind");
			}
			else
			{
				listObj.Dodged = 0;
				listObj.BuffList = null;
				listObj.DebuffList = null;
				listObj.HasBreakableCc = false;
			}
		}
	}
	catch (Exception e)
	{
		LogErrors("ScanObj() -> WowObject listObj in cachedUnitlist: " + e.ToString());
		Log.Print("ScanObj() -> WowObject listObj in cachedUnitlist\r\n" + e.Message, 4);
	}

	LastScanObj = WowHelpers.GetTime();



}