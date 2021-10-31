/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* Copyright � 2021 FlowerFell-Sans.                                                                                     */
/* If you are missing that file, acquire a complete release at github.com/teeworldsCNFun/teeworlds-war                   */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef GAME_SERVER_ENTITIES_WTF_H
#define GAME_SERVER_ENTITIES_WTF_H

class CWTF : public CEntity
{
public:
	CWTF(CGameWorld *pGameWorld, vec2 Pos);

	void Reset();
	void Tick();
	void Snap(int SnappingClient);
	void GiveHealth();
	void SearchChar();
	bool TestBox(vec2 Pos, vec2 Size);
	void MoveBox(vec2* pInoutPos, vec2* pInoutVel, vec2 Size, float Elasticity);

private:
	CCharacter* m_pCarryingChar;
	bool m_IsDropped;
	bool m_IsTaken;
	int m_DropTick;

	vec2 m_StartPos;

	vec2 m_Vel;

    vec2 m_aFrom[5];
    vec2 m_aTo[5];

    int m_aIDs[5];
};

#endif
