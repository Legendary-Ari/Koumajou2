#pragma once
class CGameObject; 
class CGameObject_Manager
{
	DECLARE_SINGLETON(CGameObject_Manager)
private:
	CGameObject_Manager();
	~CGameObject_Manager();
public:
	void Add_GameObject_Manager(OBJECTINFO::OBJID eID, CGameObject* pObject );
	void Update_GameObject_Manager(); 
	void Render_GameObject_Manager(); 
	void Release_GameObject_Manager(); 
private:
	list<CGameObject*> m_listGameObject[OBJECTINFO::OBJID_END]; 
	list<CGameObject*> m_listGameObjectRender[RENDERID::END];
};

