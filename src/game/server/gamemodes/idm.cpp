/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#include <engine/shared/config.h>
#include <game/server/entities/character.h>
#include <game/server/player.h>

#include "idm.h"

CGameControllerIDM::CGameControllerIDM(class CGameContext *pGameServer)
: IGameController(pGameServer)
{
	// Exchange this to a string that identifies your game mode.
	// DM, TDM and CTF are reserved for teeworlds original modes.
	m_pGameType = "IDM";

	//m_GameFlags = GAMEFLAG_TEAMS; // GAMEFLAG_TEAMS makes it a two-team gamemode
}

void CGameControllerIDM::Tick()
{
	// this is the main part of the gamemode, this function is run every tick
	//DoPlayerScoreWincheck(); // checks for winners, no teams version
	//DoTeamScoreWincheck(); // checks for winners, two teams version

	IGameController::Tick();
}

void CGameControllerIDM::OnCharacterSpawn(class CCharacter *pChr)
{
	pChr->RemoveWeapons();		

	pChr->IncreaseHealth(10);

	if(g_Config.m_SvWeaponType == 2)
	{
		pChr->GiveWeapon(WEAPON_GRENADE, -1);
		pChr->SetWeapon(WEAPON_GRENADE);
	}
	else if(g_Config.m_SvWeaponType == 3)
	{
		pChr->GiveWeapon(WEAPON_GUN, -1);
		pChr->SetWeapon(WEAPON_GUN);
	}
	else if(g_Config.m_SvWeaponType == 4)
	{
		pChr->GiveWeapon(WEAPON_HAMMER, -1);
		pChr->SetWeapon(WEAPON_HAMMER);
	}
	else if(g_Config.m_SvWeaponType == 5)
	{
		pChr->GiveWeapon(WEAPON_SHOTGUN, -1);
		pChr->SetWeapon(WEAPON_SHOTGUN);
	}
	else if(g_Config.m_SvWeaponType == 6)
	{
		pChr->GiveWeapon(WEAPON_NINJA, -1);
		pChr->SetWeapon(WEAPON_NINJA);
	}
	else
	{
		pChr->GiveWeapon(WEAPON_RIFLE, -1);
		pChr->SetWeapon(WEAPON_RIFLE);
	}
}
