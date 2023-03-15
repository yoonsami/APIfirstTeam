#pragma once
#include "Object.h"
template<typename T>
class CAbstractFactory
{
public:
	static CObject* Create()
	{
		CObject* pObj = new T;
		pObj->Init();
		return pObj;
	}

	static CObject* Create(float _fX, float _fY)
	{
		CObject* pObj = new T;
		pObj->Init();
		pObj->Set_Pos(_fX, _fY);
		return pObj;
	}

	static CObject* Create(float _fX, float _fY, Vec2 _Dir)
	{
		CObject* pObj = new T;
		pObj->Init();
		pObj->Set_Pos(_fX, _fY);
		pObj->Set_Velocity(_Dir);
		return pObj;
	}
};

