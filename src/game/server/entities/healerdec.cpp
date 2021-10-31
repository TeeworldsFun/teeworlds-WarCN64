/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* Copyright © 2013 Neox.                                                                                                */
/* If you are missing that file, acquire a complete release at https://www.teeworlds.com/forum/viewtopic.php?pid=106707  */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <game/generated/protocol.h>
#include <game/server/gamecontext.h>
#include "healerdec.h"

CHealerDec::CHealerDec(CGameWorld *pGameWorld, int Owner)
: CEntity(pGameWorld, CGameWorld::ENTTYPE_PROJECTILE)
{
	m_Owner = Owner;
	if(GameServer()->GetPlayerChar(m_Owner))
        m_Pos = GameServer()->GetPlayerChar(m_Owner)->m_Pos;

	GameWorld()->InsertEntity(this);
}

void CHealerDec::Reset()
{
	GameServer()->m_World.DestroyEntity(this);
}


void CHealerDec::Tick()
{
    if(!GameServer()->GetPlayerChar(m_Owner))
    {
        Reset();
        return;
    }

    vec2 CharPos = GameServer()->GetPlayerChar(m_Owner)->m_Pos;

    m_Pos = CharPos + vec2(0, -60);
    if(GameServer()->Collision()->CheckPoint(m_Pos))
        m_Pos = CharPos + vec2(0, 60);
    if(GameServer()->Collision()->CheckPoint(m_Pos))
        m_Pos = CharPos + vec2(-60, 0);
    if(GameServer()->Collision()->CheckPoint(m_Pos))
        m_Pos = CharPos + vec2(60, 0);
}

void CHealerDec::Snap(int SnappingClient)
{
	CNetObj_Pickup *pObj = static_cast<CNetObj_Pickup *>(Server()->SnapNewItem(NETOBJTYPE_PICKUP, m_ID, sizeof(CNetObj_Pickup)));
	if(!pObj)
		return;

	pObj->m_X = (int)m_Pos.x;
	pObj->m_Y = (int)m_Pos.y;
	pObj->m_Type = WEAPON_RIFLE;
}
