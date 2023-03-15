#pragma once
#include "Bullet.h"

class CGuidedBullet :public CBullet
{
public:
	CGuidedBullet();
	virtual ~CGuidedBullet();


public:
	virtual void Init() override;
	virtual int Update() override;

public:
	void Set_TargetList(list<CObject*>* TargetList) { m_pTargetList = TargetList; }
	RECT& Get_DetectRect() { return m_tDetect_Rect; }
	
private:
	void Find_ClosestTarget();

private:
	RECT			m_tDetect_Rect;
	list<CObject*>*	m_pTargetList;
	FLOAT			m_fDetectRange;
	CObject*		m_pTarget;

};

