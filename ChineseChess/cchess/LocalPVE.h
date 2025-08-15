#pragma once
#include"localPVP.h"

class CChessLocalPVE :public CChessLocalPVP
{
public:
	CChessLocalPVE();
	~CChessLocalPVE();

	void Update() override;

};