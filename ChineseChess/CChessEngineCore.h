#pragma once
#include"CChessBase.h"

class CChessEngine
{
public:
	CChessEngine();
	virtual ~CChessEngine();

	virtual void Reset();
	virtual bool GetAvailableSteps(PiecePosDesc& pos);
	virtual void MovePiece(PieceMoveDesc&move);
	virtual void SelectPiece(PiecePosDesc& pos);

	void SetSide(bool side_red);

	//online
	virtual CChessMap GetMap() = 0;
	virtual void SyncMap(CChessMap newMap) = 0;
	

private:
	CChessMap map;

	bool side_red, side_is_mine;
	//todo: add engine adapter member
};
