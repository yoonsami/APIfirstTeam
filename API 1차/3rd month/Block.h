#pragma once
#include "Object.h"

class CBlock : public CObject
{
public:
    CBlock();
    virtual ~CBlock();

public:
    // CObject을(를) 통해 상속됨
    virtual void Init() override;
    virtual int  Update() override;
    virtual void Late_Update() override;
    virtual void Render(HDC hDC) override;
    virtual void Release() override;

private:

};

