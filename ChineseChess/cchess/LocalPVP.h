#pragma once
#include"CChessEngineCore.h"

class CChessLocalPVP :public CChessEngine
{
public:
	CChessLocalPVP();
	virtual ~CChessLocalPVP();
	// ͨ�� CChessEngine �̳�
	virtual void Reset() override;
	virtual void Update() override;
	virtual CChessBase::EngineResult GetResult()override;
	virtual void MovePiece(CChessBase::PieceMoveDesc move) override;
	virtual std::vector<CChessBase::PiecePosDesc> SelectPiece(CChessBase::PiecePosDesc pos) override;
	virtual void SetSide(bool side_red) override;
	virtual CChessBase::CChessMap GetMap() override;
	virtual void SyncMap(CChessBase::CChessMap newMap) override;

protected:
	std::vector<CChessBase::PiecePosDesc> availablePositions[9][10];

	bool update_check_win, match_over, win_side;
	CChessBase::EngineResult result;

	virtual bool CheckBestMove() override;
	virtual CChessBase::PieceMoveDesc GetBestMove() override;
	virtual void SearchBestMove() override;
};