#include "gungame.h"

CGameControllerGG::CGameControllerGG(class CGameContext *pGameServer) : IGameController(pGameServer)

{
	m_pGameType = "GunGame";
}

void CGameControllerGG::Tick()
{
	IGameController::Tick();
}
