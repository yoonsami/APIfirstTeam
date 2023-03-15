#pragma once
#include "Include.h"


class CObject;
class CMainGame
{
public:
	CMainGame();
	~CMainGame();

public:
	void Init();
	void Update();
	void Late_Update();
	void Render();
	void Release();

public:
	static FLOAT Gravity;
	static FLOAT GameSpeed;
	static FLOAT COR; // �ݹ߰��
	static INT iStageNum;
	static FLOAT	quad_eqn(FLOAT A, FLOAT B, FLOAT C)
	{
		FLOAT D = B * B - 4.f * A * C;
		
			if (((-B + sqrtf(D)) / (2.f * A)) <= 1.f && ((-B + sqrtf(D)) / (2.f * A)) >= 0.f)
				return (-B + sqrtf(D)) / (2.f * A);
			else if(((-B - sqrtf(D)) / (2.f * A)) <= 1.f && ((-B - sqrtf(D)) / (2.f * A)) >= 0.f)
				return (-B - sqrtf(D)) / (2.f * A);

			return 1;
	}


private:
	HDC				m_DC;
	list<CObject*>	m_objList[OT_END];
	DWORD64			m_dwFPSTime;
	DWORD64			m_dwMonsterCreTime;
	INT				m_iFPS;
	TCHAR			m_szFPS[32];
	INT				m_iStageNum;
};

// �߻� ���丮 ����
// �̻��Ϸ� ���� ���̱�
// �浹ó�� �Լ� (intersectrect)

// 