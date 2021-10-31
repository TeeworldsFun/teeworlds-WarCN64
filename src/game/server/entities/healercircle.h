/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* Copyright © 2013 Neox.                                                                                                */
/* If you are missing that file, acquire a complete release at https://www.teeworlds.com/forum/viewtopic.php?pid=106707  */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef GAME_SERVER_ENTITIES_HELPERCIRCLE_H
#define GAME_SERVER_ENTITIES_HELPERCIRCLE_H

enum
{
    LASER_NUM = 10,
};

class CHelperCircle : public CEntity
{
public:
	CHelperCircle(CGameWorld *pGameWorld, int Owner, vec2 Pos);

	void Reset();
	void Tick();
	void Snap(int SnappingClient);
	void Fire();

private:
	int m_Owner;
	int m_ReloadTimer;
	int m_LifeSpan;

	vec2 m_aFrom[LASER_NUM-1];
	vec2 m_aTo[LASER_NUM-1];

	int m_aIDs[LASER_NUM];

	int m_aCharReloads[MAX_CLIENTS];
	int m_aFlagReloads[2];
};

#endif
