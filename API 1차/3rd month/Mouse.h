#pragma once
#include "Object.h"

class CMouse : public CObject
{
public:
	virtual ~CMouse();
public:
	void	Init() override;
	int		Update() override;
	void	Late_Update() override;
	void	Render(HDC hDC) override;
	void	Release() override;

};

