/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* Copyright © 2013 Neox.                                                                                                */
/* If you are missing that file, acquire a complete release at https://www.teeworlds.com/forum/viewtopic.php?pid=106707  */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef GAME_SERVER_ENTITIES_FLAG_H
#define GAME_SERVER_ENTITIES_FLAG_H

#include <game/server/entity.h>

class CFlag : public CEntity
{
public:
	static const int ms_PhysSize = 20;
	vec2 m_StandPos;

	int m_Team;
    vec2 m_Pos2;

	CFlag(CGameWorld *pGameWorld, int Team);

	virtual void Reset();
	virtual void Snap(int SnappingClient);
	void TakeDamage(int Dmg, int From);
	int GetHealth() const {return m_Health;}
	void IncreaseHealth(int Amount);

private:

    int m_ID2;
    int m_ID3;
    int m_ID4;
    int m_ID5;
    int m_ID6;

    int m_StartHealth;
    int m_DamageTaken;
    int m_DamageTakenTick;
    int m_Health;
};

#endif
