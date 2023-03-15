#pragma once
#include "Bullet.h"

class CRotationBullet : public CBullet
{
public:
    CRotationBullet();
    virtual ~CRotationBullet();

public:

    virtual void Init() override;
    virtual int Update() override;
    virtual void Render(HDC hDC) override;
    virtual void Release() override;

private:
    bool m_bIsFire;
    float m_fRotationSize;
    float m_fBulletCenterX;
    float m_fBulletCenterY;
    float m_fBulletAngle;

};

