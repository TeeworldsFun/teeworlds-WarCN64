/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#include <engine/shared/config.h>

#include <game/server/entities/character.h>
#include <game/server/player.h>
#include "itdm.h"

CGameControllerITDM::CGameControllerITDM(class CGameContext *pGameServer) : IGameController(pGameServer)
{
	m_pGameType = "ITDM";
	m_GameFlags = GAMEFLAG_TEAMS;
}

int CGameControllerITDM::OnCharacterDeath(class CCharacter *pVictim, class CPlayer *pKiller, int Weapon)
{
	IGameController::OnCharacterDeath(pVictim, pKiller, Weapon);


	if(Weapon != WEAPON_GAME)
	{
		// do team scoring
		if(pKiller == pVictim->GetPlayer() || pKiller->GetTeam() == pVictim->GetPlayer()->GetTeam())
			m_aTeamscore[pKiller->GetTeam()&1]--; // klant arschel
		else
			m_aTeamscore[pKiller->GetTeam()&1]++; // good shit
	}

	pVictim->GetPlayer()->m_RespawnTick = max(pVictim->GetPlayer()->m_RespawnTick, Server()->Tick()+Server()->TickSpeed()*g_Config.m_SvRespawnDelayTDM);

	return 0;
}

void CGameControllerITDM::Snap(int SnappingClient)
{
	IGameController::Snap(SnappingClient);

	CNetObj_GameData *pGameDataObj = (CNetObj_GameData *)Server()->SnapNewItem(NETOBJTYPE_GAMEDATA, 0, sizeof(CNetObj_GameData));
	if(!pGameDataObj)
		return;

	pGameDataObj->m_TeamscoreRed = m_aTeamscore[TEAM_RED];
	pGameDataObj->m_TeamscoreBlue = m_aTeamscore[TEAM_BLUE];

	pGameDataObj->m_FlagCarrierRed = 0;
	pGameDataObj->m_FlagCarrierBlue = 0;
}

void CGameControllerITDM::Tick()
{
	IGameController::Tick();
}

void CGameControllerITDM::OnCharacterSpawn(class CCharacter *pChr)
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
