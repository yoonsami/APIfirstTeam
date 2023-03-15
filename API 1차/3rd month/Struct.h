#pragma once
struct Vec2
{
	float vX;
	float vY;
	void operator+= (const Vec2& vec) { vX += vec.vX; vY += vec.vY; }
	void operator-= (const Vec2& vec) { vX -= vec.vX; vY -= vec.vY; }
	Vec2 operator+ (const Vec2& vec) { return Vec2{ vX + vec.vX, vY + vec.vY }; }
	Vec2 operator- (const Vec2& vec) { return Vec2{ vX - vec.vX, vY - vec.vY }; }
	Vec2 operator/ (const FLOAT& a) { return Vec2{ vX / a, vY / a }; }
	Vec2 operator* (const FLOAT& a) { return Vec2{ vX * a, vY * a }; }
	FLOAT Get_Theta()
	{
		return atan2f(vY, vX);
	}
	FLOAT Get_Size() { return sqrtf(powf(vX, 2) + powf(vY, 2)); }
};

struct INFO
{
	float		fX;
	float		fY;
	float		fCX;
	float		fCY;

	INFO operator+ (const Vec2& vec) { return INFO{ fX + vec.vX, fY + vec.vY, fCX,fCY }; }
	INFO operator- (const Vec2& vec) { return INFO{ fX - vec.vX, fY - vec.vY, fCX,fCY }; }
	void operator+= (const Vec2& vec) { fX += vec.vX; fY += vec.vY; }
	void operator-= (const Vec2& vec) { fX -= vec.vX; fY -= vec.vY; }
	Vec2 Get_PosVec() { return Vec2{ fX,fY }; }
};

struct STAT
{
	float m_fAttack;
	float m_fMaxHp;
	float m_fHp;
};