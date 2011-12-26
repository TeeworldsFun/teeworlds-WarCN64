#ifndef GAME_SERVER_GAMEMODES_GUNGAME_H
#define GAME_SERVER_GAMEMODES_GUNGAME_h
#include <game/server/gamecontroller.h>

class CGameControllerGG : public IGameController
{
	int aPlayerWeapons[MAX_CLIENTS];
public:
	CGameControllerGG(class CGameContext *pGameServer);
	
	virtual bool WPickupAllowed(int Weapon, int ClientID);

	virtual void Tick();
	virtual void OnCharacterSpawn(class CCharacter *pChr);
	virtual int OnCharacterDeath(class CCharacter *pVictim, class CPlayer *pKiller, int Weapon);
	virtual bool OnEntity(int Index, vec2 Pos);
	virtual bool IsGunGame();
};

#endif
