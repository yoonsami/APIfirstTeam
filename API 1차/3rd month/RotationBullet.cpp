#include "pch.h"
#include "RotationBullet.h"

CRotationBullet::CRotationBullet()
    : m_bIsFire(false),
    m_fRotationSize(25.f),
    m_fBulletCenterX(0.f),
    m_fBulletCenterY(0.f),
    m_fBulletAngle(0.f)
{
}

CRotationBullet::~CRotationBullet()
{

}

void CRotationBullet::Init()
{
    m_tInfo.fCX = 20.f;
    m_tInfo.fCY = 20.f;
    
    m_fSpeed = 3.f;
    m_eFigure = FIGURETYPE::FT_CIRCLE;
}

int CRotationBullet::Update()
{
    if (Is_Dead())
        return OBJ_DEAD;

    if (!m_bIsFire)
    {
        m_fBulletCenterX = m_tInfo.fX;
        m_fBulletCenterY = m_tInfo.fY;

        m_bIsFire = true;
    }
    // m_fAimRadian 이용
    //m_fBulletCenterX += cosf(m_fAimRadian * (PI / 180) * m_fSpeed);
    //m_fBulletCenterY += sinf(m_fAimRadian * (PI / 180) * m_fSpeed);
    // 
    // m_tDirection 넘겨주는거(추상팩토리에 이미 이용하는게 있어서  편하게)
    m_fBulletCenterX += m_tDirection.vX * m_fSpeed;
    m_fBulletCenterY += m_tDirection.vY * m_fSpeed;
    
    m_fBulletAngle += 30.f;

    m_tInfo.fX = m_fBulletCenterX + m_fRotationSize * cosf(m_fBulletAngle * (PI / 180));
    m_tInfo.fY = m_fBulletCenterY + m_fRotationSize * sinf(m_fBulletAngle * (PI / 180));
    
    __super::Update_Rect();


    return OBJ_NOEVENT;
}

void CRotationBullet::Render(HDC hDC)
{
    Draw_Figure(hDC);
}

void CRotationBullet::Release()
{

}
