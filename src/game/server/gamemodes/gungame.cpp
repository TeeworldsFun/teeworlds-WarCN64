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
		pChr->GiveWeapon(aPlayerWeapons[ClientID], 10);

	pChr->SetWeapon(aPlayerWeapons[ClientID]);
}

int CGameControllerGG::OnCharacterDeath(class CCharacter *pVictim, class CPlayer *pKiller, int Weapon)
{
	IGameController::OnCharacterDeath(pVictim, pKiller, Weapon);

	//ignore WEAPON_GAME, WEAPON_SELF, WEAPON_WORLD
	if(Weapon >= 0)
	{
		//never use the same weapon as before
		int NewWeapon = RandomWeapon();
		if(NewWeapon == aPlayerWeapons[pKiller->GetCID()])
			NewWeapon = RandomWeapon(); //generate another weapon
		//set new weapon
		aPlayerWeapons[pKiller->GetCID()] = NewWeapon;

		CCharacter *pChr = pKiller->GetCharacter();
		pChr->RemWeapons(); //remove all weapons
		pChr->GiveWeapon(NewWeapon, 10);
		pChr->SetWeapon(NewWeapon);
	}

	return 0;
}
