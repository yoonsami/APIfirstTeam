#include "pch.h"
#include "Mouse.h"

CMouse::CMouse() :CObject(OT_MOUSE)
{

}

CMouse::~CMouse()
{
	Release();

}

void CMouse::Init()
{
	m_tInfo.fCX = 20.f;
	m_tInfo.fCY = 20.f;
	m_eFigure = FIGURETYPE::FT_CIRCLE;
}

int CMouse::Update()
{
	POINT ptMouse{};

	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	m_tInfo.fX = (float)ptMouse.x;
	m_tInfo.fY = (float)ptMouse.y;


	__super::Update_Rect();

	ShowCursor(false);

	return OBJ_NOEVENT;
}

void CMouse::Late_Update()
{

}

void CMouse::Render(HDC hDC)
{
	Draw_Figure(hDC);
}

void CMouse::Release()
{
}
