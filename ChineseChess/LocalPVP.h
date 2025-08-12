#pragma once
#include"CChessEngineCore.h"

class CChessLocalPVP :public CChessEngine
{
public:
	CChessLocalPVP();
	~CChessLocalPVP();
	// Í¨¹ý CChessEngine ¼Ì³Ð
	virtual void Reset() override;
	virtual void Update() override;
	virtual void MovePiece(CChessBase::PieceMoveDesc move) override;
	virtual std::vector<CChessBase::PiecePosDesc> SelectPiece(CChessBase::PiecePosDesc pos) override;
	virtual void SetSide(bool side_red) override;
	virtual CChessBase::CChessMap GetMap() override;
	virtual void SyncMap(CChessBase::CChessMap newMap) override;

private:
	std::vector<CChessBase::PiecePosDesc> availablePositions[9][10];

	bool update_check_win, match_over, win_side;
};