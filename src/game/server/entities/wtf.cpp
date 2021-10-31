/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* Copyright � 2021 FlowerFell-Sans.                                                                                     */
/* If you are missing that file, acquire a complete release at github.com/teeworldsCNFun/teeworlds-war                   */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <game/generated/protocol.h>
#include <game/server/gamecontext.h>
#include <engine/shared/config.h>

#include "flag.h"
#include "wtf.h"

CWTF::CWTF(CGameWorld *pGameWorld, vec2 Pos)
: CEntity(pGameWorld, CGameWorld::ENTTYPE_PICKUP)
{
	m_StartPos = Pos;

    for(int i = 0; i < 5; i++)
    {
        const int SIZE = 32;
        m_aIDs[i] = Server()->SnapNewID();
        switch(i)
        {
        case 0:
            m_aFrom[i] = vec2(-SIZE, -SIZE);
            m_aTo[i] = vec2(SIZE, -SIZE);
            break;
        case 1:
            m_aFrom[i] = vec2(SIZE, -SIZE);
            m_aTo[i] = vec2(SIZE, SIZE);
            break;
        case 2:
            m_aFrom[i] = vec2(SIZE, SIZE);
            m_aTo[i] = vec2(-SIZE, SIZE);
            break;
        case 3:
            m_aFrom[i] = vec2(-SIZE, SIZE);
            m_aTo[i] = vec2(-SIZE, -SIZE);
            break;
        case 4:
            m_aFrom[i] = vec2(-SIZE, -SIZE);
            m_aTo[i] = vec2(-SIZE, -SIZE);
            break;
        }
    }

	Reset();

	GameWorld()->InsertEntity(this);
}

void CWTF::Reset()
{
    if(m_pCarryingChar && m_pCarryingChar->IsAlive())
        m_pCarryingChar->SetCaryingBox(false);
	m_pCarryingChar = NULL;
	m_Pos = m_StartPos;
	m_IsDropped = false;
	m_IsTaken = false;
	m_DropTick = 0;
}

void CWTF::SearchChar()
{
    for(int i = 0; i < MAX_CLIENTS; i++)
    {
        CCharacter* pClosest = GameServer()->GetPlayerChar(i);

        if(!pClosest)
            continue;

        if(distance(m_Pos, pClosest->m_Pos) > 64)
            continue;

        if(pClosest->GetCaryingBox())
            continue;

        m_pCarryingChar = GameServer()->GetPlayerChar(i);
        m_IsTaken = true;
        m_DropTick = 0;
        m_IsDropped = false;
        m_pCarryingChar->SetCaryingBox(true);
        break;
    }
}

void CWTF::Tick()
{
    if(!m_pCarryingChar || !m_pCarryingChar->IsAlive())
    {
        if(m_IsTaken)
        {
            m_DropTick = Server()->Tick() + Server()->TickSpeed() * 20;
            m_IsTaken = false;
            m_IsDropped = true;
            m_pCarryingChar = NULL;
            m_Vel = vec2(0, 0);
        }
    }

    if(m_IsDropped)
    {
        const float Gravity = GameServer()->m_World.m_Core.m_Tuning.m_Gravity;
        m_Vel.y += Gravity;
        MoveBox(&m_Pos, &m_Vel, vec2(64, 64), 0.5f);
        if(m_DropTick < Server()->Tick())
            Reset();
    }

    if(m_pCarryingChar && m_pCarryingChar->IsAlive())
    {
        m_Pos = m_pCarryingChar->m_Pos + vec2(0, -75);
        if(GameServer()->Collision()->CheckPoint(m_Pos))
            m_Pos = m_pCarryingChar->m_Pos + vec2(0, 75);
        if(GameServer()->Collision()->CheckPoint(m_Pos))
            m_Pos = m_pCarryingChar->m_Pos;
        GiveHealth();
    }

    if(!m_IsTaken)
        SearchChar();

    if(GameServer()->Collision()->GetCollisionAt(m_Pos.x+m_ProximityRadius/3.f, m_Pos.y-m_ProximityRadius/3.f)&CCollision::COLFLAG_DEATH ||
		GameServer()->Collision()->GetCollisionAt(m_Pos.x+m_ProximityRadius/3.f, m_Pos.y+m_ProximityRadius/3.f)&CCollision::COLFLAG_DEATH ||
		GameServer()->Collision()->GetCollisionAt(m_Pos.x-m_ProximityRadius/3.f, m_Pos.y-m_ProximityRadius/3.f)&CCollision::COLFLAG_DEATH ||
		GameServer()->Collision()->GetCollisionAt(m_Pos.x-m_ProximityRadius/3.f, m_Pos.y+m_ProximityRadius/3.f)&CCollision::COLFLAG_DEATH ||
		GameLayerClipped(m_Pos))
	{
		Reset();
	}
}

void CWTF::GiveHealth()
{
    for(int i = 0; i < 2; i ++)
    {
        CFlag* pFlag = GameServer()->m_apFlags[i];

        if(pFlag->m_Team == m_pCarryingChar->GetPlayer()->GetTeam())
            continue;

        if(distance(m_Pos, pFlag->m_Pos) > 32 && distance(m_Pos, pFlag->m_Pos2) > 32)
            continue;

        if(GameServer()->Collision()->IntersectLine(m_Pos, pFlag->m_Pos, NULL, NULL) &&
           GameServer()->Collision()->IntersectLine(m_Pos, pFlag->m_Pos, NULL, NULL))
            continue;

        pFlag->IncreaseHealth(g_Config.m_DamageboxAmount);
        GameServer()->CreatePlayerSpawn(m_Pos);
        Reset();

        return;
    }
}

bool CWTF::TestBox(vec2 Pos, vec2 Size)
{
	Size *= 0.5f;
	if(GameServer()->Collision()->CheckPoint(Pos.x-Size.x, Pos.y+Size.y))
		return true;
	if(GameServer()->Collision()->CheckPoint(Pos.x+Size.x, Pos.y+Size.y))
		return true;
	return false;
}

void CWTF::MoveBox(vec2 *pInoutPos, vec2 *pInoutVel, vec2 Size, float Elasticity)
{
	// do the move
	vec2 Pos = *pInoutPos;
	vec2 Vel = *pInoutVel;

	float Distance = length(Vel);
	int Max = (int)Distance;

	if(Distance > 0.00001f)
	{
		//vec2 old_pos = pos;
		float Fraction = 1.0f/(float)(Max+1);
		for(int i = 0; i <= Max; i++)
		{
			//float amount = i/(float)max;
			//if(max == 0)
				//amount = 0;

			vec2 NewPos = Pos + Vel*Fraction; // TODO: this row is not nice

			if(TestBox(vec2(NewPos.x, NewPos.y), Size))
			{
				int Hits = 0;

				if(TestBox(vec2(Pos.x, NewPos.y), Size))
				{
					NewPos.y = Pos.y;
					Vel.y *= -Elasticity;
					Hits++;
				}

				if(TestBox(vec2(NewPos.x, Pos.y), Size))
				{
					NewPos.x = Pos.x;
					Vel.x *= -Elasticity;
					Hits++;
				}

				// neither of the tests got a collision.
				// this is a real _corner case_!
				if(Hits == 0)
				{
					NewPos.y = Pos.y;
					Vel.y *= -Elasticity;
					NewPos.x = Pos.x;
					Vel.x *= -Elasticity;
				}
			}

			Pos = NewPos;
		}
	}

	*pInoutPos = Pos;
	*pInoutVel = Vel;
}

void CWTF::Snap(int SnappingClient)
{
    CNetObj_Pickup *pObj = static_cast<CNetObj_Pickup *>(Server()->SnapNewItem(NETOBJTYPE_PICKUP, m_ID, sizeof(CNetObj_Pickup)));
	if(!pObj)
		return;

	pObj->m_X = (int)m_Pos.x;
	pObj->m_Y = (int)m_Pos.y;
	pObj->m_Type = POWERUP_NINJA;

	CNetObj_Laser *pLaser[5];

    for(int i = 0; i < 5; i++)
    {
        pLaser[i] = static_cast<CNetObj_Laser *>(Server()->SnapNewItem(NETOBJTYPE_LASER, m_aIDs[i], sizeof(CNetObj_Laser)));

        if(!pLaser[i])
            return;

        pLaser[i]->m_X = (int)m_Pos.x + (int)m_aFrom[i].x;
        pLaser[i]->m_Y = (int)m_Pos.y + (int)m_aFrom[i].y;
        pLaser[i]->m_FromX = (int)m_Pos.x + (int)m_aTo[i].x;
        pLaser[i]->m_FromY = (int)m_Pos.y + (int)m_aTo[i].y;
        pLaser[i]->m_StartTick = Server()->Tick() - 3;
    }
}
