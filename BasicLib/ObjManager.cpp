#include "ObjManager.h"
#include "HInput.h"

void  ObjManager::AddCollisionExecute(BaseObject* owner, CollisionFuncion func)
{
	owner->m_iCollisionID = m_iCollisionID++;
	m_ObjectList.insert(std::make_pair(owner->m_iCollisionID, owner));
	m_fnCollision.insert(std::make_pair(owner->m_iCollisionID, func));
}
void ObjManager::AddSelectExecute(BaseObject* owner, CollisionFuncion func)
{
	owner->m_iSelectID = m_iSelectID++;
	m_SelectList.insert(std::make_pair(owner->m_iSelectID, owner));
	m_fnSelect.insert(std::make_pair(owner->m_iSelectID, func));
}
void  ObjManager::DeleteExecute(BaseObject* owner)
{
	std::map<int, BaseObject*>::iterator objiter;
	objiter = m_ObjectList.find(owner->m_iCollisionID);
	if (objiter != m_ObjectList.end())
	{
		m_ObjectList.erase(objiter);
	}

	FuncionIterator colliter = m_fnCollision.find(owner->m_iCollisionID);
	if (colliter != m_fnCollision.end())
	{
		m_fnCollision.erase(colliter);
	}
}
void ObjManager::DeleteSelectExecute(BaseObject* owner)
{
	std::map<int, BaseObject*>::iterator objiter;
	objiter = m_SelectList.find(owner->m_iCollisionID);
	if (objiter != m_SelectList.end())
	{
		m_SelectList.erase(objiter);
	}

	FuncionIterator colliter = m_fnSelect.find(owner->m_iCollisionID);
	if (colliter != m_fnSelect.end())
	{
		m_fnSelect.erase(colliter);
	}
}
bool  ObjManager::Init()
{
	return true;
}
bool  ObjManager::Frame()
{
	for (auto src : m_ObjectList)
	{
		BaseObject* pObjSrc = (BaseObject*)src.second;
		if (pObjSrc->m_dwCollisonType == CollisionType::Ignore) continue;
		DWORD dwState = CollisionType::Overlap;
		for (auto dest : m_ObjectList)
		{
			BaseObject* pObjDest = (BaseObject*)dest.second;
			if (pObjSrc == pObjDest) continue;
			if (Collision::ToRect(pObjSrc->m_rtCollision, pObjDest->m_rtCollision))
			{
				FuncionIterator colliter = m_fnCollision.find(pObjSrc->m_iCollisionID);
				if (colliter != m_fnCollision.end())
				{
					CollisionFuncion call = colliter->second;
					call(pObjDest, dwState);
				}
			}
		}
	}

	for (auto src : m_SelectList)
	{
		BaseObject* pObjSrc = (BaseObject*)src.second;
		if (pObjSrc->m_dwSelectType == CollisionType::Ignore) continue;
		DWORD dwState = CollisionType::Overlap;
		if (Collision::RectToPoint(
			pObjSrc->m_rtCollision, (float)g_ptMouse.x, (float)g_ptMouse.y))
		{
			DWORD dwKeyState = HInput::Get().m_dwMouseState[0];
			pObjSrc->m_dwSelectState = SelectState::S_HOVER;
			if (dwKeyState == KEY_PUSH)
			{
				pObjSrc->m_dwSelectState = SelectState::S_ACTIVE;
			}
			if (dwKeyState == KEY_HOLD)
			{
				pObjSrc->m_dwSelectState = SelectState::S_FOCUS;
			}
			if (dwKeyState == KEY_UP)
			{
				pObjSrc->m_dwSelectState = SelectState::S_SELECTED;
			}


			FuncionIterator colliter = m_fnSelect.find(pObjSrc->m_iSelectID);
			if (colliter != m_fnSelect.end())
			{
				CollisionFuncion call = colliter->second;
				call(pObjSrc, dwState);
			}
		}

	}
	return true;
}
bool  ObjManager::Release()
{
	return true;
}