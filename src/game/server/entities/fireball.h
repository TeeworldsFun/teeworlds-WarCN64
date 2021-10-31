/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* Copyright © 2013 Neox.                                                                                                */
/* If you are missing that file, acquire a complete release at https://www.teeworlds.com/forum/viewtopic.php?pid=106707  */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef GAME_SERVER_ENTITIES_FIREBALL_H
#define GAME_SERVER_ENTITIES_FIREBALL_H

class CFireBall : public CEntity
{
public:
	CFireBall(CGameWorld *pGameWorld, int Owner, vec2 Dir, vec2 Pos);

	virtual void Reset();
	virtual void Tick();
	virtual void Snap(int SnappingClient);

private:
	vec2 m_Direction;
	int m_LifeSpan;
	int m_Owner;

	bool m_aFlagHits[2];
	bool m_aCharHits[MAX_CLIENTS];
};

#endif
