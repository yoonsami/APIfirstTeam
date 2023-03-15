#pragma once
#include "Include.h"
#include "Object.h"

class CCollisionMgr
{
public:

	static BOOL IntersectCircle(RECT* circleA, RECT* circleB)
	{
		Vec2 cenA = { ((circleA->left + circleA->right) / 2.f) , ((circleA->top + circleA->bottom) / 2.f) };
		Vec2 cenB = { ((circleB->left + circleB->right) / 2.f) , ((circleB->top + circleB->bottom) / 2.f) };

		LONG sizeA = abs(circleA->right - circleA->left);
		LONG sizeB = abs(circleB->right - circleB->left);

		return (cenA - cenB).Get_Size() < (sizeA + sizeB) / 2.f;
	}

	static BOOL IntersectCirDot(RECT* circle, FLOAT fX, FLOAT fY)
	{
		Vec2 cen = { ((circle->left + circle->right) / 2.f) , ((circle->top + circle->bottom) / 2.f) };
		Vec2 dot = { fX,fY };

		return (cen - dot).Get_Size() < abs(circle->right - circle->left) * 0.5f;

	}

	static BOOL IntersectRectDot(RECT* rect, FLOAT fX, FLOAT fY){return (fX< rect->right&& fX> rect->left && fY > rect->top && fY < rect->bottom);}

	static BOOL IntersectCirRect(RECT* circle, RECT* rect);

	static BOOL IntersectWith(RECT& rectA, RECT& rectB, FIGURETYPE figureA, FIGURETYPE figureB);
	

	static void CheckCollide(list<CObject*> _Dst, list<CObject*> _Src);
};

