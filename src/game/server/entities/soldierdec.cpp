/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* Copyright © 2013 Neox.                                                                                                */
/* If you are missing that file, acquire a complete release at https://www.teeworlds.com/forum/viewtopic.php?pid=106707  */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <game/generated/protocol.h>
#include <game/server/gamecontext.h>
#include "soldierdec.h"
#include "flag.h"


CSoldierDec::CSoldierDec(CGameWorld *pGameWorld, int Owner)
: CEntity(pGameWorld, CGameWorld::ENTTYPE_PROJECTILE)
{
	m_Owner = Owner;
	if(GameServer()->GetPlayerChar(m_Owner))
        m_Pos = GameServer()->GetPlayerChar(m_Owner)->m_Pos;

	GameWorld()->InsertEntity(this);
}

void CSoldierDec::Reset()
{
	GameServer()->m_World.DestroyEntity(this);
}

void CSoldierDec::Tick()
{
    if(!GameServer()->GetPlayerChar(m_Owner))
    {
        Reset();
        return;
    }

    vec2 CharPos = GameServer()->GetPlayerChar(m_Owner)->m_Pos;

    m_Pos = CharPos + vec2(0, 25);
    if(GameServer()->Collision()->CheckPoint(m_Pos))
        m_Pos = CharPos + vec2(0, 25);
    if(GameServer()->Collision()->CheckPoint(m_Pos))
        m_Pos = CharPos + vec2(0, 25);
    if(GameServer()->Collision()->CheckPoint(m_Pos))
        m_Pos = CharPos + vec2(0, 25);
}

void CSoldierDec::Snap(int SnappingClient)
{
	CNetObj_Pickup *pObj = static_cast<CNetObj_Pickup *>(Server()->SnapNewItem(NETOBJTYPE_PICKUP, m_ID, sizeof(CNetObj_Pickup)));
	if(!pObj)
		return;

	pObj->m_X = (int)m_Pos.x;
	pObj->m_Y = (int)m_Pos.y;
	pObj->m_Type = WEAPON_GRENADE;
}
