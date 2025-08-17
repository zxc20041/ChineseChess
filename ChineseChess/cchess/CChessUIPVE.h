#pragma once
#include"CChessUI.h"


class CChessUIPVE : public CChessUI
{
public:
	CChessUIPVE();
	virtual ~CChessUIPVE();
	void ClickAt(int line, int column) override;
	void Update() override;

private:
	
};