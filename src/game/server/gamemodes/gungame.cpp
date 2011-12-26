#include <base/system.h>
#include <base/math.h>
#include <game/generated/protocol.h>
#include <engine/shared/config.h>
#include <game/server/gamecontext.h>
#include <game/server/player.h>
#include <game/server/entities/character.h>

#include "gungame.h"

CGameControllerGG::CGameControllerGG(class CGameContext *pGameServer) : IGameController(pGameServer)

{
	m_pGameType = "GunGame";

	//unset all weapons
	for(int i=0;i<MAX_CLIENTS;i++)
		aPlayerWeapons[i] = -1;
}

void CGameControllerGG::Tick()
{
	IGameController::Tick();
}

//this func won't return the same weapon as before
static int RandomWeapon()
{
	static int LastWeapon = -1;
	int NewWeapon = time_get()%(NUM_WEAPONS-1);
	if(LastWeapon == NewWeapon)
		NewWeapon = (NewWeapon+1)%(NUM_WEAPONS-1);

	LastWeapon = NewWeapon;
	return NewWeapon;
}

void CGameControllerGG::OnCharacterSpawn(class CCharacter *pChr)
{
	pChr->IncreaseHealth(10);

	int ClientID = pChr->GetPlayer()->GetCID();
	
	if(aPlayerWeapons[ClientID] == -1)
		aPlayerWeapons[ClientID] = RandomWeapon(); //set random weapon without ninja

	if(aPlayerWeapons[ClientID] == WEAPON_HAMMER)
		pChr->GiveWeapon(WEAPON_HAMMER, -1);
	else
		pChr->GiveWeapon(aPlayerWeapons[ClientID], g_Config.m_SvGGInfiniteAmmo ? -1 : 10);

	pChr->SetWeapon(aPlayerWeapons[ClientID]);
	//PrintNewWeapon(ClientID);
}

int CGameControllerGG::OnCharacterDeath(class CCharacter *pVictim, class CPlayer *pKiller, int Weapon)
{
	IGameController::OnCharacterDeath(pVictim, pKiller, Weapon);

	//ignore WEAPON_GAME, WEAPON_SELF, WEAPON_WORLD
	if(Weapon >= 0 && pKiller->GetCID() != pVictim->GetPlayer()->GetCID())
	{
		//never use the same weapon as before
		int NewWeapon = RandomWeapon();
		if(NewWeapon == aPlayerWeapons[pKiller->GetCID()])
			NewWeapon = RandomWeapon(); //generate another weapon
		//set new weapon
		aPlayerWeapons[pKiller->GetCID()] = NewWeapon;

		CCharacter *pChr = pKiller->GetCharacter();
		if(pChr)
		{
			pChr->RemWeapons(); //remove all weapons
			
			if(NewWeapon == WEAPON_HAMMER)
				pChr->GiveWeapon(WEAPON_HAMMER, -1);
			else
				pChr->GiveWeapon(NewWeapon, g_Config.m_SvGGInfiniteAmmo ? -1 : 10);
		
			pChr->SetWeapon(NewWeapon);
		}

		PrintNewWeapon(pKiller->GetCID());
	}

	return 0;
}

bool CGameControllerGG::OnEntity(int Index, vec2 Pos)
{
	if(g_Config.m_SvGGInfiniteAmmo)
	{
		if(Index == ENTITY_WEAPON_SHOTGUN || Index == ENTITY_WEAPON_GRENADE || Index == ENTITY_WEAPON_RIFLE)
			return false; //don't spawn weapons
	}
	
	//ninja is disabled in gungame
	if(Index == ENTITY_POWERUP_NINJA)
		return false;

	return IGameController::OnEntity(Index, Pos);
}

bool CGameControllerGG::IsGunGame()
{
	//this is gungame
	return true;
}

bool CGameControllerGG::WPickupAllowed(int Weapon, int ClientID)
{
	if(aPlayerWeapons[ClientID] == Weapon)
		return true;

	return false;
}

void CGameControllerGG::PrintNewWeapon(int ClientID)
{
	const char *name = "<invalid>";
	switch(aPlayerWeapons[ClientID])
	{
		case WEAPON_GUN:
			name = "Gun";	
		break;
		case WEAPON_HAMMER:
			name = "Hammer";
		break;
		case WEAPON_SHOTGUN:
			name = "Shotgun";
		break;
		case WEAPON_GRENADE:
			name = "Grenade";
		break;
		case WEAPON_RIFLE:
			name = "Rifle";
		break;
	}

	char buf[128];
	str_format(buf, sizeof(buf), "New Weapon: %s", name);
	GameServer()->SendChatTarget(ClientID, buf);
}
