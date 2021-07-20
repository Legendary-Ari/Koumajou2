#pragma once
#include "Scene.h"
class CLoadingScene :
	public CScene
{
private:
	CLoadingScene();
public:
	virtual ~CLoadingScene();

	static CScene* Create(const CScene_Manager::ID &);
	
	virtual HRESULT Ready_Scene() override;
	virtual void Update_Scene() override;
	virtual void InitUpdate_Scene() override;
	virtual void Render_Scene() override;
	virtual void Release_Scene() override;
	void		Set_LoadingStage(const CScene_Manager::ID& _eId);
	static unsigned	__stdcall LoadActor(LPVOID pVoid);
private:
	CScene_Manager::ID m_eLoadingStage;
	CRITICAL_SECTION m_tCritical_Section;
	HANDLE m_hThread;
	bool	m_bLoaded;
};

