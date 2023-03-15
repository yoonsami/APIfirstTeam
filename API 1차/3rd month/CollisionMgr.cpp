#include "pch.h"
#include "CollisionMgr.h"

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
	}
	if (figureA == FIGURETYPE::FT_CIRCLE)
	{
		if (figureB == FIGURETYPE::FT_RECT)
			return IntersectCirRect(&rectA, &rectB);
		if (figureB == FIGURETYPE::FT_CIRCLE)
			return IntersectCircle(&rectA, &rectB);
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
					// 피격판정 TODO
					i->On_Attacked(j);
					j->On_Attacked(i);
				}
			}
		}
	}
}
