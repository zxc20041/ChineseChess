#pragma once
#include"localPVP.h"

class CChessLocalPVE :public CChessLocalPVP
{
public:
	CChessLocalPVE();
	~CChessLocalPVE();

	void Update() override;
	bool CheckBestMove() override;
	CChessBase::PieceMoveDesc GetBestMove() override;
	void SearchBestMove() override;
};