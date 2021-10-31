/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* Copyright © 2013 Neox.                                                                                                */
/* If you are missing that file, acquire a complete release at https://www.teeworlds.com/forum/viewtopic.php?pid=106707  */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef GAME_SERVER_ENTITIES_SOLDIERDEC_H
#define GAME_SERVER_ENTITIES_SOLDIERDEC_H

class CSoldierDec : public CEntity
{
public:
	CSoldierDec(CGameWorld *pGameWorld, int Owner);

	void Reset();
	void Tick();
	void Snap(int SnappingClient);

private:
	int m_Owner;
};

#endif
