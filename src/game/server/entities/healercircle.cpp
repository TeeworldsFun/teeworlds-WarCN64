/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* Copyright © 2013 Neox.                                                                                                */
/* If you are missing that file, acquire a complete release at https://www.teeworlds.com/forum/viewtopic.php?pid=106707  */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <game/generated/protocol.h>
#include <game/server/gamecontext.h>
#include <engine/shared/config.h>

#include "healercircle.h"
#include "flag.h"

CHelperCircle::CHelperCircle(CGameWorld *pGameWorld, int Owner, vec2 Pos)
: CEntity(pGameWorld, CGameWorld::ENTTYPE_PROJECTILE)
{
	m_Owner = Owner;
	m_Pos = Pos;
	for(int i = 0; i < LASER_NUM; i ++)
    {
        m_aIDs[i] = Server()->SnapNewID();
        if(i == LASER_NUM-1)
            break;
        m_aFrom[i] = m_Pos + normalize(GetDir(pi/180 * ((i*(360/(LASER_NUM-1)))%360+1)))*g_Config.m_HealersCircleSize;
        m_aTo[i] = m_Pos + normalize(GetDir(pi/180 * (((i+1)*(360/(LASER_NUM-1)))%360+1)))*g_Config.m_HealersCircleSize;
    }
    m_LifeSpan = 15 * Server()->TickSpeed();

    GameServer()->CreatePlayerSpawn(m_Pos);

	GameWorld()->InsertEntity(this);
}

void CHelperCircle::Reset()
{
    for(int i = 0; i < LASER_NUM; i ++)
        Server()->SnapFreeID(m_aIDs[i]);

	GameServer()->m_World.DestroyEntity(this);
}


void CHelperCircle::Tick()
{
    if(!GameServer()->GetPlayerChar(m_Owner))
    {
        Reset();
        return;
    }

    for(int i = 0; i < MAX_CLIENTS; i ++)
    {
        if(m_aCharReloads[i])
            m_aCharReloads[i] --;

        if(i > 1)
            continue;

        if(m_aFlagReloads[i])
            m_aFlagReloads[i] --;
    }

    Fire();

    if(m_LifeSpan)
        m_LifeSpan --;
    else
        Reset();
}

void CHelperCircle::Fire()
{
    for(int i = 0; i < MAX_CLIENTS; i++)
    {
        CCharacter* pHit = GameServer()->GetPlayerChar(i);

        if(!pHit)
            continue;

        if(distance(pHit->m_Pos, m_Pos) > g_Config.m_HealersCircleSize)
            continue;

        if(m_aCharReloads[i])
            continue;

        if(pHit->GetPlayer()->GetTeam() == GameServer()->m_apPlayers[m_Owner]->GetTeam())
        {
            if(pHit->GetHealth() < pHit->GetPlayer()->TotalHP())
                pHit->IncreaseHealth(g_Config.m_HealersCircleHeal);
            else if(pHit->GetArmor() < pHit->GetPlayer()->TotalAP())
                pHit->IncreaseArmor(g_Config.m_HealersCircleHeal);
            else
                continue;
        }
        else
            pHit->TakeDamage(vec2(0, 0), g_Config.m_HealersCircleDamage, m_Owner, -1);

        m_aCharReloads[i] = Server()->TickSpeed();
    }

    for(int i = 0; i < 2; i ++)
    {
        CFlag* pFlag = GameServer()->m_apFlags[i];

        if(distance(m_Pos, pFlag->m_Pos) > g_Config.m_HealersCircleSize && distance(m_Pos, pFlag->m_Pos2) > g_Config.m_HealersCircleSize)
            continue;

        if(m_aFlagReloads[i])
            continue;

        if(pFlag->m_Team == GameServer()->m_apPlayers[m_Owner]->GetTeam())
        {
            if(pFlag->GetHealth() < g_Config.m_FlagHealth)
                pFlag->IncreaseHealth(g_Config.m_HealersCircleHeal);
            else
                continue;
        }
        else
            pFlag->TakeDamage(g_Config.m_HealersCircleDamage, m_Owner);

        m_aFlagReloads[i] = Server()->TickSpeed();
    }
}

void CHelperCircle::Snap(int SnappingClient)
{
    if(!GameServer()->m_apPlayers[m_Owner])
        return;

    if(GameServer()->m_apPlayers[m_Owner]->GetTeam() == TEAM_RED)
    {
        CNetObj_Projectile *pObj = static_cast<CNetObj_Projectile *>(Server()->SnapNewItem(NETOBJTYPE_PROJECTILE, m_ID, sizeof(CNetObj_Projectile)));
        if(!pObj)
            return;

        pObj->m_X = (int)m_Pos.x;
        pObj->m_Y = (int)m_Pos.y;
        pObj->m_VelX = 1;
        pObj->m_VelY = 1;
        pObj->m_Type = WEAPON_GRENADE;
        pObj->m_StartTick = Server()->Tick() - 1;
    }
    else
    {
        CNetObj_Laser *pObj = static_cast<CNetObj_Laser *>(Server()->SnapNewItem(NETOBJTYPE_LASER, m_ID, sizeof(CNetObj_Laser)));
        if(!pObj)
            return;

        pObj->m_X = (int)m_Pos.x;
        pObj->m_Y = (int)m_Pos.y;
        pObj->m_FromX = (int)m_Pos.x;
        pObj->m_FromY = (int)m_Pos.y;
        pObj->m_StartTick = Server()->Tick() - 1;
    }

    CNetObj_Laser *apObjs[LASER_NUM];

    for(int i = 0; i < LASER_NUM-1; i++)
    {
        apObjs[i] = static_cast<CNetObj_Laser *>(Server()->SnapNewItem(NETOBJTYPE_LASER, m_aIDs[i], sizeof(CNetObj_Laser)));

        if(!apObjs[i])
            return;

        apObjs[i]->m_X = (int)m_aFrom[i].x;
        apObjs[i]->m_Y = (int)m_aFrom[i].y;
        apObjs[i]->m_FromX = (int)m_aTo[i].x;
        apObjs[i]->m_FromY = (int)m_aTo[i].y;
        apObjs[i]->m_StartTick = Server()->Tick() - 1;
    }

    CNetObj_Laser *pExtraLaser = static_cast<CNetObj_Laser *>(Server()->SnapNewItem(NETOBJTYPE_LASER, m_aIDs[LASER_NUM-1], sizeof(CNetObj_Laser)));

    if(!pExtraLaser)
        return;

    pExtraLaser->m_X = (int)m_aTo[LASER_NUM-2].x;
    pExtraLaser->m_Y = (int)m_aTo[LASER_NUM-2].y;
    pExtraLaser->m_FromX = (int)m_aTo[LASER_NUM-2].x;
    pExtraLaser->m_FromY = (int)m_aTo[LASER_NUM-2].y;
    pExtraLaser->m_StartTick = Server()->Tick() - 1;
}
