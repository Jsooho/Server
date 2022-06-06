#pragma once
#include"Object2D.h"

using CollisionFuncion = std::function<void(BaseObject*, DWORD)>;
using SelectFunction = std::function<void(BaseObject*, DWORD)>;

class ObjManager : public Singleton <ObjManager>
{
private:
	int m_iCollisionID;
	int m_iSelectID;
	std::map<int, BaseObject*> m_ObjectList;
	std::map<int, BaseObject*> m_SelectList;
public:
	friend Singleton<ObjManager>;
public:
	typedef std::map<int, CollisionFuncion>::iterator FuncionIterator;
	std::map<int, SelectFunction> m_fnCollision;
	typedef std::map<int, SelectFunction>::iterator FuncionIterator;
	std::map<int, SelectFunction> m_fnSelect;
public:
	void  AddCollisionExecute(BaseObject* owner, CollisionFuncion func);
	void  AddSelectExecute(BaseObject* owner, CollisionFuncion func);
	void  DeleteExecute(BaseObject* owner);
	void  DeleteSelectExecute(BaseObject* owner);
	bool  Init();
	bool  Frame();
	bool  Release();
private:
	ObjManager() { m_iCollisionID = 0; };
public:
	virtual ~ObjManager() {};
	
};
#define I_ObjManager ObjManager::Get()
