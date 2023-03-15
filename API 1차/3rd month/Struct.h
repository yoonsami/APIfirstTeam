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
	Vec2 Get_DirVec() { if (Get_Size() != 0) return Vec2{ vX / Get_Size(), vY / Get_Size() }; else return Vec2{ 0.f,0.f }; }
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
	FLOAT m_fAttack;
	FLOAT m_fMaxHp;
	FLOAT m_fHp;
	FLOAT Get_HpRatio() { if (m_fMaxHp > 0) return m_fHp / m_fMaxHp; else return 0; }
};