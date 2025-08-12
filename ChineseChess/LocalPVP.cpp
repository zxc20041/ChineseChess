#include"LocalPVP.h"
#include"framework_base.h"
using namespace std;
using namespace CChessBase;
using namespace debugger;

CChessLocalPVP::CChessLocalPVP():CChessEngine()
{
	update_check_win = 0, match_over = 0, win_side = 0;
}

CChessLocalPVP::~CChessLocalPVP()
{
}

void CChessLocalPVP::Reset()
{
	memset(&map, PIECE_NULL, sizeof(map));
	
	map.board[0][0] = PIECE_ROOK;
	map.board[1][0] = PIECE_HORSE;
	map.board[2][0] = PIECE_ELEPHANT;
	map.board[3][0] = PIECE_MANDARIN;
	map.board[4][0] = PIECE_KING;
	map.board[5][0] = PIECE_MANDARIN;
	map.board[6][0] = PIECE_ELEPHANT;
	map.board[7][0] = PIECE_HORSE;
	map.board[8][0] = PIECE_ROOK;
	map.board[1][2] = PIECE_CANNON;
	map.board[7][2] = PIECE_CANNON;
	map.board[0][3] = PIECE_PAWN;
	map.board[2][3] = PIECE_PAWN;
	map.board[4][3] = PIECE_PAWN;
	map.board[6][3] = PIECE_PAWN;
	map.board[8][3] = PIECE_PAWN;

	map.board[0][9] = PIECE_ROOK;
	map.board[1][9] = PIECE_HORSE;
	map.board[2][9] = PIECE_ELEPHANT;
	map.board[3][9] = PIECE_MANDARIN;
	map.board[4][9] = PIECE_KING;
	map.board[5][9] = PIECE_MANDARIN;
	map.board[6][9] = PIECE_ELEPHANT;
	map.board[7][9] = PIECE_HORSE;
	map.board[8][9] = PIECE_ROOK;
	map.board[1][7] = PIECE_CANNON;
	map.board[7][7] = PIECE_CANNON;
	map.board[0][6] = PIECE_PAWN;
	map.board[2][6] = PIECE_PAWN;
	map.board[4][6] = PIECE_PAWN;
	map.board[6][6] = PIECE_PAWN;
	map.board[8][6] = PIECE_PAWN;

	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			if (map.board[i][j] != PIECE_NULL)
			{
				map.piece_side[i][j] = 1;
			}
		}
	}

	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			availablePositions[i][j] = GetAvailableSteps(PiecePosDesc(i, j));
		}
	}

	engineAdapter.Reset();
	return;
}

void CChessLocalPVP::Update()
{
	if (update_check_win)
	{
		bool result;
		if (engineAdapter.GetWin(result))
		{
			if (result)
			{
				 win_side = !current_side_red;
				 match_over = 1;
			}
		}
	}
	return;
}

void CChessLocalPVP::MovePiece(CChessBase::PieceMoveDesc move)
{
	bool valid = 0;
	for (auto &i : availablePositions[move.fromx][move.fromy])	//verify
	{
		if (i.x == move.tox && i.y == move.toy)
		{
			valid = 1;
			break;
		}
	}
	if (current_side_red != map.piece_side[move.fromx][move.fromy])
	{
		valid = 0;
	}
	if (!valid)
	{
		debugger_main.writelog(DWARNNING, "target pos not found in CChessLocalPVP::MovePiece() " + to_string(move.fromx) + "," + to_string(move.fromy) + "->" + to_string(move.tox) + "," + to_string(move.toy), __LINE__);
		//todo: sync map to cui
		return;
	}
	map.board[move.tox][move.toy] = map.board[move.fromx][move.fromy];
	map.piece_side[move.tox][move.toy] = map.piece_side[move.fromx][move.fromy];
	map.board[move.fromx][move.fromy] = PIECE_NULL;

	current_side_red = !current_side_red;

	engineAdapter.MovePiece(move);
	update_check_win = 1;
	//print map
	debugger_main.writelog(DDEBUG, "map info", __LINE__);
	for (int i = BOARD_Y_MAX; i >= 0; i--)
	{
		string line;
		for (int j = 0; j <= BOARD_X_MAX; j++)
		{
			line += " " + to_string(map.board[j][i]);
		}
		debugger_main.writelog(DDEBUG, line);
	}

	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			availablePositions[i][j] = GetAvailableSteps(PiecePosDesc(i, j));
		}
	}
	return;
}

std::vector<CChessBase::PiecePosDesc> CChessLocalPVP::SelectPiece(CChessBase::PiecePosDesc pos)
{
	if (map.board[pos.x][pos.y] == PIECE_NULL)
	{
		debugger_main.add_tagline("WARNNING: Select NULL PIECE");
	}
	return availablePositions[pos.x][pos.y];
}

void CChessLocalPVP::SetSide(bool side_red)
{
	this->side_red = side_red;
	return;
}

CChessBase::CChessMap CChessLocalPVP::GetMap()
{
	return map;
}

void CChessLocalPVP::SyncMap(CChessBase::CChessMap newMap)
{
}
