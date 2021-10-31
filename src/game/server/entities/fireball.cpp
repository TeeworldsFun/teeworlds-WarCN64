/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* Copyright © 2013 Neox.                                                                                                */
/* If you are missing that file, acquire a complete release at https://www.teeworlds.com/forum/viewtopic.php?pid=106707  */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <game/generated/protocol.h>
#include <game/server/gamecontext.h>
#include "fireball.h"
#include "flag.h"

CFireBall::CFireBall(CGameWorld *pGameWorld, int Owner, vec2 Dir, vec2 Pos)
: CEntity(pGameWorld, CGameWorld::ENTTYPE_PROJECTILE)
{
	m_Direction = Dir;
	m_LifeSpan = 10 * Server()->TickSpeed();
	m_Owner = Owner;
	m_Pos = Pos;

    for(int i = 0; i < MAX_CLIENTS; i ++)
    {
        m_aCharHits[i] = false;

        if(i > 1)
            continue;

        m_aFlagHits[i] = false;
    }

	GameWorld()->InsertEntity(this);
}

void CFireBall::Reset()
{
	GameServer()->m_World.DestroyEntity(this);
}


void CFireBall::Tick()
{
    m_Pos += m_Direction * 20;
    bool NoDamage = true;
    int Team = -1;
    if(GameServer()->m_apPlayers[m_Owner])
        Team = GameServer()->m_apPlayers[m_Owner]->GetTeam();
	const bool Collide = GameServer()->Collision()->CheckPoint(m_Pos);
	CCharacter *OwnerChar = GameServer()->GetPlayerChar(m_Owner);
	CFlag *TargetFlag = GameServer()->m_World.ClosestFlag(m_Pos, 50.f, Team);

    for(int i = 0; i < MAX_CLIENTS; i++)
    {
        CCharacter* pHit = GameServer()->GetPlayerChar(i);

        if(!pHit)
            continue;

        if(pHit == OwnerChar)
            continue;

        if(OwnerChar)
        {
            if(OwnerChar->GetPlayer()->GetTeam() == pHit->GetPlayer()->GetTeam())
                continue;
        }

        if(distance(m_Pos, pHit->m_Pos) > 50)
            continue;

        if(m_aCharHits[i])
            continue;

        NoDamage = false;
        m_aCharHits[i] = true;
    }

	m_LifeSpan--;

	if(Collide || m_LifeSpan < 0 || GameLayerClipped(m_Pos))
	{
		Reset();
		return;
	}

	if(TargetFlag && !m_aFlagHits[TargetFlag->m_Team])
    {
        bool Dmg = false;
        if(OwnerChar)
        {
            if(OwnerChar->GetPlayer()->GetTeam() == TargetFlag->m_Team)
                Dmg = true; // The flag has the same team as the owner, damages are useless !
            if(!NoDamage)
                Dmg = false; // This is to prevent if an ennemy is on a flag (so we needed to hit already)
        }
        m_aFlagHits[TargetFlag->m_Team] = true;
        NoDamage = Dmg;
    }

    GameServer()->CreateExplosion(m_Pos, m_Owner, -1, NoDamage);
}

void CFireBall::Snap(int SnappingClient)
{
}
