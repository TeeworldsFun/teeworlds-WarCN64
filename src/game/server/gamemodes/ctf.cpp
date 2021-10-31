/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* Copyright � 2013 Neox.                                                                                                */
/* If you are missing that file, acquire a complete release at https://www.teeworlds.com/forum/viewtopic.php?pid=106707  */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <engine/shared/config.h>

#include <game/mapitems.h>

#include <game/server/entities/character.h>
#include <game/server/entities/flag.h>
#include <game/server/player.h>
#include <game/server/gamecontext.h>
#include "ctf.h"

CGameControllerCTF::CGameControllerCTF(class CGameContext *pGameServer)
: IGameController(pGameServer)
{
	m_apFlags[0] = 0;
	m_apFlags[1] = 0;
	m_pGameType = "WarCN 64";
	m_GameFlags = GAMEFLAG_TEAMS|GAMEFLAG_FLAGS;
	m_aTeamscore[TEAM_RED] = g_Config.m_FlagHealth;
	m_aTeamscore[TEAM_BLUE] = g_Config.m_FlagHealth;
}

bool CGameControllerCTF::OnEntity(int Index, vec2 Pos)
{
	if(IGameController::OnEntity(Index, Pos))
		return true;

	int Team = -1;
	if(Index == ENTITY_FLAGSTAND_RED) Team = TEAM_RED;
	if(Index == ENTITY_FLAGSTAND_BLUE) Team = TEAM_BLUE;
	if(Team == -1 || m_apFlags[Team])
		return false;

	CFlag *F = new CFlag(&GameServer()->m_World, Team);
	F->m_StandPos = Pos;
	F->m_Pos = Pos;
	F->m_Pos2 = Pos + vec2(0, -50);
	m_apFlags[Team] = F;
	GameServer()->m_World.InsertEntity(F);
	return true;
}

void CGameControllerCTF::DoWincheck()
{
	if(m_GameOverTick == -1 && !m_Warmup)
	{
		// check score win condition
		if(((m_aTeamscore[TEAM_RED] <= 0 || m_aTeamscore[TEAM_BLUE] <= 0)) ||
			(g_Config.m_SvTimelimit > 0 && (Server()->Tick()-m_RoundStartTick) >= g_Config.m_SvTimelimit*Server()->TickSpeed()*60))
		{
			if(m_SuddenDeath)
			{
				if(m_aTeamscore[TEAM_RED]/100 != m_aTeamscore[TEAM_BLUE]/100)
					EndRound();
			}
			else
			{
				if(m_aTeamscore[TEAM_RED] != m_aTeamscore[TEAM_BLUE])
					EndRound();
				else
					m_SuddenDeath = 1;
			}
		}
	}
}

void CGameControllerCTF::Snap(int SnappingClient)
{
	IGameController::Snap(SnappingClient);

	CNetObj_GameData *pGameDataObj = (CNetObj_GameData *)Server()->SnapNewItem(NETOBJTYPE_GAMEDATA, 0, sizeof(CNetObj_GameData));
	if(!pGameDataObj)
		return;

    CFlag *RedF = m_apFlags[TEAM_RED];
    CFlag *BlueF = m_apFlags[TEAM_BLUE];

    if(RedF)
    {
        pGameDataObj->m_TeamscoreRed = RedF->GetHealth();
        m_aTeamscore[TEAM_RED] = RedF->GetHealth();
        pGameDataObj->m_FlagCarrierRed = FLAG_ATSTAND;
    }
    else
        pGameDataObj->m_FlagCarrierBlue = FLAG_MISSING;

    if(BlueF)
    {
        pGameDataObj->m_TeamscoreBlue = BlueF->GetHealth();
        m_aTeamscore[TEAM_BLUE] = BlueF->GetHealth();
        pGameDataObj->m_FlagCarrierBlue = FLAG_ATSTAND;
    }
    else
        pGameDataObj->m_FlagCarrierBlue = FLAG_MISSING;

}

void CGameControllerCTF::Tick()
{
	IGameController::Tick();

	if(GameServer()->m_World.m_ResetRequested || GameServer()->m_World.m_Paused)
		return;

	for(int fi = 0; fi < 2; fi++)
	{
		CFlag *F = m_apFlags[fi];

		if(!F)
			continue;
	}
}