#ifndef GAME_SERVER_GAMEMODES_GUNGAME_H
#define GAME_SERVER_GAMEMODES_GUNGAME_h
#include <game/server/gamecontroller.h>

class CGameControllerGG : public IGameController
{
public:
	CGameControllerGG(class CGameContext *pGameServer);
	virtual void Tick();
};

#endif
