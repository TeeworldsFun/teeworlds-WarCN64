/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* Copyright � 2021 FlowerFell-Sans.                                                                                     */
/* If you are missing that file, acquire a complete release at github.com/teeworldsCNFun/teeworlds-war                   */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <game/generated/protocol.h>
#include <game/server/gamecontext.h>
#include "ninjadec.h"
#include "flag.h"


CNinjaDec::CNinjaDec(CGameWorld *pGameWorld, int Owner)
: CEntity(pGameWorld, CGameWorld::ENTTYPE_PROJECTILE)
{
	m_Owner = Owner;
	if(GameServer()->GetPlayerChar(m_Owner))
        m_Pos = GameServer()->GetPlayerChar(m_Owner)->m_Pos;

	GameWorld()->InsertEntity(this);
}

void CNinjaDec::Reset()
{
	GameServer()->m_World.DestroyEntity(this);
}

void CNinjaDec::Tick()
{
    if(!GameServer()->GetPlayerChar(m_Owner))
    {
        Reset();
        return;
    }
    GameServer()->CreateExplosion(m_Pos, m_Owner, -1, false);

    vec2 CharPos = GameServer()->GetPlayerChar(m_Owner)->m_Pos;

    m_Pos = CharPos + vec2(0, 0);
    if(GameServer()->Collision()->CheckPoint(m_Pos))
        m_Pos = CharPos + vec2(0, 0);
    if(GameServer()->Collision()->CheckPoint(m_Pos))
        m_Pos = CharPos + vec2(0, 0);
    if(GameServer()->Collision()->CheckPoint(m_Pos))
        m_Pos = CharPos + vec2(0, 0);
}

void CNinjaDec::Snap(int SnappingClient)
{
	CNetObj_Pickup *pObj = static_cast<CNetObj_Pickup *>(Server()->SnapNewItem(NETOBJTYPE_PICKUP, m_ID, sizeof(CNetObj_Pickup)));
	if(!pObj)
		return;

	pObj->m_X = (int)m_Pos.x;
	pObj->m_Y = (int)m_Pos.y;
	pObj->m_Type = POWERUP_NINJA;
    pObj->m_Subtype = WEAPON_NINJA;
}
