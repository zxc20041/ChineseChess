#include"CChessEngineCore.h"
#include"..\framework_base.h"
using namespace CChessBase;
using namespace std;
using namespace debugger;

CChessEngine::CChessEngine()
{
	memset(&map, 0, sizeof(map));
	side_red = 1, current_side_red = 1, available_steps_ready = 0;
}

CChessEngine::~CChessEngine()
{
	//debugger_main.writelog(DDEBUG, "in ~CChessEngine()", __LINE__);
}

bool CChessEngine::NotMySide(int x, int y, bool my_side)
{
	if (!PositionInBoard(x, y))
	{
		return 0;
	}
	return map.board[x][y] == PIECE_NULL || map.piece_side[x][y] != my_side;
}

ChessPieceType CChessEngine::GetPieceType(CChessBase::PiecePosDesc& pos) const
{
    return map.board[pos.x][pos.y];
}

bool CChessEngine::PositionInBoard(int x, int y)
{
	return x >= 0 && x <= 8 && y >= 0 && y <= 9;
}

bool CChessEngine::PositionPieceNull(int x, int y)
{
	return PositionInBoard(x, y) && map.board[x][y] == PIECE_NULL;
}

bool CChessEngine::PositionInMandarinArea(int x, int y, bool my_side)
{
	if (!PositionInBoard(x, y))
	{
		return 0;
	}
	if (my_side)
	{
		if (x == 3 && y == 0 || x == 3 && y == 2 || x == 4 && y == 1 || x == 5 && y == 0 || x == 5 && y == 2)
		{
			return 1;
		}
	}
	else
	{
		if (x == 3 && y == 9 || x == 3 && y == 7 || x == 4 && y == 8 || x == 5 && y == 9 || x == 5 && y == 7)
		{
			return 1;
		}
	}
	return 0;
}

bool CChessEngine::ObeyRule_KingMeetKing(int x, int y)
{
	if (map.board[x][y] == PIECE_KING)
	{
		return 1;
	}

	if (!PositionInBoard(x, y)|| map.board[x][y] == PIECE_NULL)
	{
		return 0;
	}

	int piece_between_num = 0, king_num = 0;
	bool start = 0, piece_between_kings = 0;
	for (int i = 0; i < 10; i++)
	{
		if (map.board[x][i] == PIECE_NULL)
		{
			continue;
		}
		if (map.board[x][i] == PIECE_KING)
		{
			king_num++;
			start = !start;
			continue;
		}
		if (start)
		{
			piece_between_num++;
			if (y == i)
			{
				piece_between_kings = 1;
			}
		}
	}
	if (king_num == 2 && piece_between_num == 1 && piece_between_kings)
	{
		return 0;
	}
	return 1;
}

bool CChessEngine::ObeyRule_KingMeetKing_for_king(int target_x, int target_y)
{
	if (!PositionInBoard(target_x, target_y)||target_x < 3 || target_x>5 || target_y > 2 && target_y < 7)
	{
		return 0;
	}
	int king_num = 0, king_limit = -1;
	bool start = 0;
	for (int i = 0; i < 10; i++)
	{
		if ((map.board[target_x][i] == PIECE_KING || i == target_y) && king_limit != i)
		{
			king_num++;
			start = !start;
			king_limit = i + 1;	//避免重复考虑同一PIECE_KING
			continue;
		}
		if (map.board[target_x][i] == PIECE_NULL)
		{
			continue;
		}
		if (start)	//存在PIECE_KING之间的其它棋子
		{
			return 1;
		}
	}
	return king_num != 2;
}

vector<PiecePosDesc> CChessEngine::GetAvailableSteps(PiecePosDesc pos)
{
	while (!available_steps_ready)
	{
		Update();
	}
	return availablePositions[pos.x][pos.y];
}

bool CChessEngine::GetSide()
{
	return side_red;
}

bool CChessEngine::GetCurrentSide() const
{
	return current_side_red;
}
