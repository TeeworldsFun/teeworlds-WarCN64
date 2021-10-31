/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* Copyright © 2013 Neox.                                                                                                */
/* If you are missing that file, acquire a complete release at https://www.teeworlds.com/forum/viewtopic.php?pid=106707  */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <game/generated/protocol.h>
#include <game/server/gamecontext.h>
#include "wizardhelper.h"
#include "flag.h"
#include "fireball.h"

CWizardHelper::CWizardHelper(CGameWorld *pGameWorld, int Owner)
: CEntity(pGameWorld, CGameWorld::ENTTYPE_PROJECTILE)
{
	m_Owner = Owner;
	if(GameServer()->GetPlayerChar(m_Owner))
        m_Pos = GameServer()->GetPlayerChar(m_Owner)->m_Pos;

	GameWorld()->InsertEntity(this);
}

void CWizardHelper::Reset()
{
	GameServer()->m_World.DestroyEntity(this);
}


void CWizardHelper::Tick()
{
    if(!GameServer()->GetPlayerChar(m_Owner))
    {
        Reset();
        return;
    }

    if(m_ReloadTimer)
        m_ReloadTimer --;

    vec2 CharPos = GameServer()->GetPlayerChar(m_Owner)->m_Pos;

    m_Pos = CharPos + vec2(0, -60);
    if(GameServer()->Collision()->CheckPoint(m_Pos))
        m_Pos = CharPos + vec2(0, 60);
    if(GameServer()->Collision()->CheckPoint(m_Pos))
        m_Pos = CharPos + vec2(-60, 0);
    if(GameServer()->Collision()->CheckPoint(m_Pos))
        m_Pos = CharPos + vec2(60, 0);

    Fire();
}

void CWizardHelper::Fire()
{
    if(m_ReloadTimer)
        return;

    for(int i = 0; i < MAX_CLIENTS; i++)
    {
        CCharacter* pHit = GameServer()->GetPlayerChar(i);

        if(!pHit)
            continue;

        if(pHit->GetPlayer()->GetTeam() == GameServer()->m_apPlayers[m_Owner]->GetTeam())
            continue;

        if(distance(pHit->m_Pos, m_Pos) > 1000)
            continue;

        if(GameServer()->Collision()->IntersectLine(m_Pos, pHit->m_Pos, NULL, NULL))
            continue;

        vec2 Dir = normalize(pHit->m_Pos - m_Pos);

        new CFireBall(&GameServer()->m_World, m_Owner, Dir, m_Pos);
        m_ReloadTimer = Server()->TickSpeed() * 2;

        return;
    }

    for(int i = 0; i < 2; i ++)
    {
        CFlag* pFlag = GameServer()->m_apFlags[i];

        if(pFlag->m_Team == GameServer()->m_apPlayers[m_Owner]->GetTeam())
            continue;

        if(distance(m_Pos, pFlag->m_Pos) > 1000)
            continue;

        if(GameServer()->Collision()->IntersectLine(m_Pos, pFlag->m_Pos, NULL, NULL))
            continue;

        vec2 Dir = normalize(pFlag->m_Pos - m_Pos);

        new CFireBall(&GameServer()->m_World, m_Owner, Dir, m_Pos);
        m_ReloadTimer = Server()->TickSpeed() * 2;

        return;
    }
}

void CWizardHelper::Snap(int SnappingClient)
{
    CNetObj_Projectile *pObj = static_cast<CNetObj_Projectile *>(Server()->SnapNewItem(NETOBJTYPE_PROJECTILE, m_ID, sizeof(CNetObj_Projectile)));
	if(!pObj)
		return;

	pObj->m_X = (int)m_Pos.x;
	pObj->m_Y = (int)m_Pos.y;
	pObj->m_VelX = 1;
	pObj->m_VelY = 1;
	pObj->m_Type = WEAPON_HAMMER;
    pObj->m_StartTick = Server()->Tick() - 1;
}
