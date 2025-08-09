#pragma once
#include"CChessBase.h"
#include"CChessEngineAdapter.h"
#include<vector>

class CChessEngine
{
public:
	CChessEngine();
	virtual ~CChessEngine();

	virtual void Reset() = 0;
	
	
	virtual void MovePiece(CChessBase::PieceMoveDesc move) = 0;
	virtual std::vector<CChessBase::PiecePosDesc> SelectPiece(CChessBase::PiecePosDesc pos) = 0;

	virtual void SetSide(bool side_red) = 0;
	virtual bool GetSide();

	bool GetCurrentSide();

	//online
	virtual CChessBase::CChessMap GetMap() = 0;
	virtual void SyncMap(CChessBase::CChessMap newMap) = 0;
	
protected:
	virtual std::vector<CChessBase::PiecePosDesc> GetAvailableSteps(CChessBase::PiecePosDesc pos);
	CChessBase::CChessMap map;

	bool side_red, current_side_red;
	//todo: add engine adapter member
	CChessEngineAdapter engineAdapter;
private:
	bool NotMySide(int x, int y, bool my_side);
	CChessBase::ChessPieceType GetPieceType(CChessBase::PiecePosDesc& pos);
	bool PositionInBoard(int x, int y);
	bool PositionPieceNull(int x, int y);
	bool PositionInMandarinArea(int x, int y, bool my_side);

	//合法时返回真
	bool ObeyRule_KingMeetKing(int x, int y);

	//合法时返回真
	bool ObeyRule_KingMeetKing_for_king(int target_x, int target_y);
	


};
