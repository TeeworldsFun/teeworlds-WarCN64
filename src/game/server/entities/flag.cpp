/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* Copyright © 2013 Neox.                                                                                                */
/* If you are missing that file, acquire a complete release at https://www.teeworlds.com/forum/viewtopic.php?pid=106707  */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <game/server/gamecontext.h>
#include "flag.h"
#include <engine/shared/config.h>

CFlag::CFlag(CGameWorld *pGameWorld, int Team)
: CEntity(pGameWorld, CGameWorld::ENTTYPE_FLAG)
{
	m_Team = Team;
	m_Health = g_Config.m_FlagHealth;
	m_ProximityRadius = ms_PhysSize;
	m_ID2 = Server()->SnapNewID();
	m_ID3 = Server()->SnapNewID();
	m_ID4 = Server()->SnapNewID();
	m_ID5 = Server()->SnapNewID();
	m_ID6 = Server()->SnapNewID();
	GameServer()->m_apFlags[m_Team] = this;

	Reset();
}

void CFlag::Reset()
{
	m_Pos = m_StandPos;
	m_Pos2 = m_Pos + vec2(0, -50);
}

void CFlag::TakeDamage(int Dmg, int From)
{
    if(GameServer()->m_apPlayers[From])
    {
        if(GameServer()->m_apPlayers[From]->GetTeam() == m_Team)
            return;

        GameServer()->m_apPlayers[From]->AddGivenDamage(Dmg);
    }
    m_DamageTaken++;

	// create healthmod indicator
	if(Server()->Tick() < m_DamageTakenTick+25)
	{
		// make sure that the damage indicators doesn't group together
		GameServer()->CreateDamageInd(m_Pos, m_DamageTaken*0.25f, Dmg);
	}
	else
	{
		m_DamageTaken = 0;
		GameServer()->CreateDamageInd(m_Pos, 0, Dmg);
	}

	m_Health -= Dmg;
	if(m_Health < 0)
        m_Health = 0;

	m_DamageTakenTick = Server()->Tick();

	if(From >= 0 && GameServer()->m_apPlayers[From])
	{
		int Mask = CmaskOne(From);
		for(int i = 0; i < MAX_CLIENTS; i++)
		{
			if(GameServer()->m_apPlayers[i] && GameServer()->m_apPlayers[i]->GetTeam() == TEAM_SPECTATORS && GameServer()->m_apPlayers[i]->m_SpectatorID == From)
				Mask |= CmaskOne(i);
		}
		GameServer()->CreateSound(GameServer()->m_apPlayers[From]->m_ViewPos, SOUND_HIT, Mask);
	}
}

void CFlag::IncreaseHealth(int Amount)
{
    if(m_Health + Amount > g_Config.m_FlagHealth)
        m_Health = g_Config.m_FlagHealth;
    else
        m_Health += Amount;
}

void CFlag::Snap(int SnappingClient)
{
	if(NetworkClipped(SnappingClient))
		return;

	CNetObj_Flag *pFlag = (CNetObj_Flag *)Server()->SnapNewItem(NETOBJTYPE_FLAG, m_Team, sizeof(CNetObj_Flag));
	if(!pFlag)
		return;

	pFlag->m_X = (int)m_Pos.x;
	pFlag->m_Y = (int)m_Pos.y;
	pFlag->m_Team = m_Team;

    CNetObj_Laser *pObj = static_cast<CNetObj_Laser *>(Server()->SnapNewItem(NETOBJTYPE_LASER, m_ID, sizeof(CNetObj_Laser)));
	if(!pObj)
		return;
	pObj->m_X = (int)m_Pos.x - 32;
	pObj->m_Y = (int)m_Pos.y - 80;
	pObj->m_FromX = (int)m_Pos.x + 32;
	pObj->m_FromY = (int)m_Pos.y - 80;
	pObj->m_StartTick = Server()->Tick()-3;

	CNetObj_Laser *pObj2 = static_cast<CNetObj_Laser *>(Server()->SnapNewItem(NETOBJTYPE_LASER, m_ID2, sizeof(CNetObj_Laser)));
	if(!pObj2)
		return;
	pObj2->m_X = (int)m_Pos.x + 32;
	pObj2->m_Y = (int)m_Pos.y - 80;
	pObj2->m_FromX = (int)m_Pos.x + 32;
	pObj2->m_FromY = (int)m_Pos.y + 48;
	pObj2->m_StartTick = Server()->Tick()-3;

	CNetObj_Laser *pObj3 = static_cast<CNetObj_Laser *>(Server()->SnapNewItem(NETOBJTYPE_LASER, m_ID3, sizeof(CNetObj_Laser)));
	if(!pObj3)
		return;
	pObj3->m_X = (int)m_Pos.x + 32;
	pObj3->m_Y = (int)m_Pos.y + 48;
	pObj3->m_FromX = (int)m_Pos.x - 32;
	pObj3->m_FromY = (int)m_Pos.y + 48;
	pObj3->m_StartTick = Server()->Tick()-3;

	CNetObj_Laser *pObj4 = static_cast<CNetObj_Laser *>(Server()->SnapNewItem(NETOBJTYPE_LASER, m_ID4, sizeof(CNetObj_Laser)));
	if(!pObj4)
		return;
	pObj4->m_X = (int)m_Pos.x - 32;
	pObj4->m_Y = (int)m_Pos.y + 48;
	pObj4->m_FromX = (int)m_Pos.x - 32;
	pObj4->m_FromY = (int)m_Pos.y - 80;
	pObj4->m_StartTick = Server()->Tick()-3;

	CNetObj_Laser *pObj5 = static_cast<CNetObj_Laser *>(Server()->SnapNewItem(NETOBJTYPE_LASER, m_ID5, sizeof(CNetObj_Laser)));
	if(!pObj5)
		return;
	pObj5->m_X = (int)m_Pos.x - 32;
	pObj5->m_Y = (int)m_Pos.y - 80;
	pObj5->m_FromX = (int)m_Pos.x - 32;
	pObj5->m_FromY = (int)m_Pos.y - 80;
	pObj5->m_StartTick = Server()->Tick()-3;
}
