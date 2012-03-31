/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */

#include <engine/shared/config.h>
#include <game/server/entities/character.h>
#include <game/server/player.h>

#include "zcatch.h"


CGameControllerZCATCH::CGameControllerZCATCH(class CGameContext *pGameServer)
: IGameController(pGameServer)
{
	m_pGameType = "zCatch";
}

void CGameControllerZCATCH::Tick()
{
	IGameController::Tick();
}

int CGameControllerZCATCH::OnCharacterDeath(class CCharacter *pVictim, class CPlayer *pKiller, int Weapon)
{
	char buf[256];
	int m_ClientID = pVictim->GetPlayer()->GetCID();
	int i;

	if(pKiller != pVictim->GetPlayer())
	{
		pKiller->m_Score++;
		GameServer()->m_apPlayers[m_ClientID]->m_CaughtBy = pKiller->GetCID();
		GameServer()->m_apPlayers[m_ClientID]->SetTeamDirect(-1);
		
		GameServer()->m_apPlayers[m_ClientID]->m_SpectatorID = pKiller->GetCID();
	
		str_format(buf, 255, "Caught by \"%s\". You will join the game automatically when \"%s\" dies.", Server()->ClientName(pKiller->GetCID()), Server()->ClientName(pKiller->GetCID()));
		GameServer()->SendChatTarget(m_ClientID, buf);
	}
	
	for(i=0;i<MAX_CLIENTS;i++)
	{
		if(GameServer()->m_apPlayers[i] && GameServer()->m_apPlayers[i]->m_CaughtBy == m_ClientID)
		{
			GameServer()->m_apPlayers[i]->m_CaughtBy = -1;
			//GameServer()->m_apPlayers[i]->SetTeamDirect(GameServer()->m_pController->ClampTeam(1));
			GameServer()->m_apPlayers[i]->SetTeamDirect(1);
			GameServer()->m_pController->OnPlayerInfoChange(GameServer()->m_apPlayers[i]);
			
			if(pKiller != pVictim->GetPlayer())
				pKiller->m_Score++;
		}
	}
	
	return 0;
}

void CGameControllerZCATCH::OnCharacterSpawn(class CCharacter *pChr)
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
