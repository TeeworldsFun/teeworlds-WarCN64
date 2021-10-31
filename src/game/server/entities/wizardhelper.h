/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/* Copyright © 2013 Neox.                                                                                                */
/* If you are missing that file, acquire a complete release at https://www.teeworlds.com/forum/viewtopic.php?pid=106707  */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef GAME_SERVER_ENTITIES_WIZARDHELPER_H
#define GAME_SERVER_ENTITIES_WIZARDHELPER_H

class CWizardHelper : public CEntity
{
public:
	CWizardHelper(CGameWorld *pGameWorld, int Owner);

	void Reset();
	void Tick();
	void Snap(int SnappingClient);
	void Fire();

private:
	int m_Owner;
	int m_ReloadTimer;
};

#endif
