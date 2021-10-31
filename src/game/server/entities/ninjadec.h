/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* Copyright � 2021 FlowerFell-Sans.                                                                                     */
/* If you are missing that file, acquire a complete release at github.com/teeworldsCNFun/teeworlds-war                   */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef GAME_SERVER_ENTITIES_NINJADEC_H
#define GAME_SERVER_ENTITIES_NINJADEC_H

class CNinjaDec : public CEntity
{
public:
	CNinjaDec(CGameWorld *pGameWorld, int Owner);

	void Reset();
	void Tick();
	void Snap(int SnappingClient);

private:
	int m_Owner;
};

#endif
