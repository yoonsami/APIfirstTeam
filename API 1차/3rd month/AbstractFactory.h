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
		pObj->Set_DirVec(_Dir);
		return pObj;
	}

	static CObject* Create(float _fX, float _fY, float _fCX, float _fCY, Vec2 _dir, float _ratio, FLOAT Mode)
	{
		CObject* pObj = new T;
		pObj->Init();
		pObj->Set_Pos(_fX, _fY);
		pObj->Set_Size(_fCX * _ratio, _fCY * _ratio);
		pObj->Set_VelX(_dir.vX * Mode);
		pObj->Set_VelY(_dir.vY);
		pObj->Get_Stat().m_fMaxHp *= _ratio;
		pObj->Get_Stat().m_fHp *= _ratio;

		return pObj;
	}
};

