#pragma once
#include"CChessBase.h"
#include<vector>

class CChessEngine
{
public:
	CChessEngine();
	virtual ~CChessEngine();

	virtual void Reset() = 0;
	
	virtual std::vector<CChessBase::PiecePosDesc> GetAvailableSteps(CChessBase::PiecePosDesc& pos);
	virtual void MovePiece(CChessBase::PieceMoveDesc& move) = 0;
	virtual void SelectPiece(CChessBase::PiecePosDesc& pos) = 0;

	virtual void SetSide(bool side_red) = 0;

	//online
	virtual CChessBase::CChessMap GetMap() = 0;
	virtual void SyncMap(CChessBase::CChessMap newMap) = 0;
	

private:
	bool NotMySide(int x, int y, bool my_side);
	CChessBase::ChessPieceType GetPieceType(CChessBase::PiecePosDesc& pos);
	bool PositionInBoard(int x, int y);
	bool PositionPieceNull(int x, int y);
	bool PositionInMandarinArea(int x, int y, bool my_side);
	bool ObeyRule_KingMeetKing(int x, int y);
	bool ObeyRule_KingMeetKing_for_king(int target_x, int target_y);
	//               BLACK
	/*  +---+---+---+---+---+---+---+---+---+
		| r | n | b | a | k | a | b | n | r | 9
		+---+---+---+---+---+---+---+---+---+
		|   |   |   |   |   |   |   |   |   | 8
		+---+---+---+---+---+---+---+---+---+
		|   | c |   |   |   |   |   | c |   | 7
		+---+---+---+---+---+---+---+---+---+
		| p |   | p |   | p |   | p |   | p | 6
		+---+---+---+---+---+---+---+---+---+
		|   |   |   |   |   |   |   |   |   | 5
		+---+---+---+---+---+---+---+---+---+
		|   |   |   |   |   |   |   |   |   | 4
		+---+---+---+---+---+---+---+---+---+
		| P |   | P |   | P |   | P |   | P | 3
		+---+---+---+---+---+---+---+---+---+
		|   | C |   |   |   |   |   | C |   | 2
		+---+---+---+---+---+---+---+---+---+
		|   |   |   |   |   |   |   |   |   | 1
		+---+---+---+---+---+---+---+---+---+
		| R | N | B | A | K | A | B | N | R | 0
		+---+---+---+---+---+---+---+---+---+
		  a   b   c   d   e   f   g   h   i         */
	//                 RED
	CChessBase::CChessMap map;

	bool side_red,current_side_red;
	//todo: add engine adapter member
};
