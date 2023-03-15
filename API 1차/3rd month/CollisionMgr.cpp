#include "pch.h"
#include "CollisionMgr.h"
#include "Arrow.h"

BOOL CCollisionMgr::IntersectCirRect(RECT* circle, RECT* rect)
{
	FLOAT closestX = (circle->left + circle->right) / 2.f;
	FLOAT closestY = (circle->top + circle->bottom) / 2.f;

	if (closestX < rect->left)
		closestX = static_cast<FLOAT>(rect->left);
	else if (closestX > rect->right)
		closestX = static_cast<FLOAT>(rect->right);

	if (closestY < rect->top)
		closestY = static_cast<FLOAT>(rect->top);
	else if (closestY > rect->bottom)
		closestY = static_cast<FLOAT>(rect->bottom);

	return IntersectCirDot(circle, closestX, closestY);
}

BOOL CCollisionMgr::IntersectWith(RECT& rectA, RECT& rectB, FIGURETYPE figureA, FIGURETYPE figureB)
{
	RECT tmp;

	if (figureA == FIGURETYPE::FT_RECT)
	{
		if (figureB == FIGURETYPE::FT_RECT)
			return IntersectRect(&tmp, &rectA, &rectB);
		if (figureB == FIGURETYPE::FT_CIRCLE)
			return IntersectCirRect(&rectB, &rectA);
		if (figureB == FIGURETYPE::FT_ARROW)
			return IntersectRectDot(&rectA, (rectB.right + rectB.left) * 0.5f, (rectB.bottom + rectB.top) * 0.5f);
	}
	else if (figureA == FIGURETYPE::FT_CIRCLE)
	{
		if (figureB == FIGURETYPE::FT_RECT)
			return IntersectCirRect(&rectA, &rectB);
		if (figureB == FIGURETYPE::FT_CIRCLE)
			return IntersectCircle(&rectA, &rectB);
		if (figureB == FIGURETYPE::FT_ARROW)
			return IntersectCirDot(&rectA, (rectB.right + rectB.left) * 0.5f, (rectB.bottom + rectB.top) * 0.5f);
	}
	else if (figureA == FIGURETYPE::FT_ARROW)
	{
		if (figureB == FIGURETYPE::FT_RECT)
			return IntersectRectDot(&rectB, (rectA.right + rectA.left) * 0.5f, (rectA.bottom + rectA.top) * 0.5f);
		if (figureB == FIGURETYPE::FT_CIRCLE)
			return IntersectCirDot(&rectB, (rectA.right + rectA.left) * 0.5f, (rectA.bottom + rectA.top) * 0.5f);
	}

	return false;
}
	
void CCollisionMgr::CheckCollide(list<CObject*> _Dst, list<CObject*> _Src)
{
	for (auto& i : _Dst)
	{
		for (auto& j : _Src)
		{
			if(!i->Is_Invincible() && !j->Is_Invincible())
			{
				if (IntersectWith(i->Get_Rect(), j->Get_Rect(), i->Get_Figure(), j->Get_Figure()))
				{
					if(i->Get_Stat().m_fHp>0 && j->Get_Stat().m_fHp > 0)
					{
						i->On_Attacked(j);
						j->On_Attacked(i);

						if (i->Get_Figure() == FIGURETYPE::FT_ARROW)
						{
							j->Hit_By_Arrow(dynamic_cast<CArrow*>(i));
						}
						else if (j->Get_Figure() == FIGURETYPE::FT_ARROW)
						{
							i->Hit_By_Arrow(dynamic_cast<CArrow*>(j));
						}
					}

				}
			}
		}
	}
}
