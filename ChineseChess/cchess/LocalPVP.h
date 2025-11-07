#pragma once
#include"CChessEngineCore.h"

class CChessLocalPVP :public CChessEngine
{
public:
	CChessLocalPVP();
	virtual ~CChessLocalPVP();
	// Í¨¹ý CChessEngine ¼Ì³Ð
	virtual void Reset() override;
	virtual void Update() override;
	virtual CChessBase::EngineResult GetResult()override;
	virtual void MovePiece(CChessBase::PieceMoveDesc move) override;
	virtual std::vector<CChessBase::PiecePosDesc> SelectPiece(CChessBase::PiecePosDesc pos) override;
	virtual void SetSide(bool side_red) override;
	virtual CChessBase::CChessMap GetMap() override;
	virtual void SyncMap(CChessBase::CChessMap newMap) override;

protected:
	

	bool update_check_win, match_over, win_side, piece_moved;
	CChessBase::EngineResult result;
	std::string recordFilename;

	virtual bool CheckBestMove() override;
	virtual CChessBase::PieceMoveDesc GetBestMove() override;
	virtual void SearchBestMove() override;
	void SearchAvailableSteps();
	virtual void RecordMove(CChessBase::PieceMoveDesc move);
};