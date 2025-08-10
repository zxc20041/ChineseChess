#include"CChessEngineCore.h"
#include"framework_base.h"
using namespace CChessBase;
using namespace std;
using namespace debugger;

CChessEngine::CChessEngine()
{
	memset(&map, 0, sizeof(map));
	side_red = 1, current_side_red = 1;
}

CChessEngine::~CChessEngine()
{
	debugger_main.writelog(DDEBUG, "in ~CChessEngine()", __LINE__);
}

bool CChessEngine::NotMySide(int x, int y, bool my_side)
{
	if (!PositionInBoard(x, y))
	{
		return 0;
	}
	return map.board[x][y] == PIECE_NULL || map.piece_side[x][y] != my_side;
}

ChessPieceType CChessEngine::GetPieceType(CChessBase::PiecePosDesc& pos)
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
			king_limit = i + 1;	//�����ظ�����ͬһPIECE_KING
			continue;
		}
		if (map.board[target_x][i] == PIECE_NULL)
		{
			continue;
		}
		if (start)	//����PIECE_KING֮�����������
		{
			return 1;
		}
	}
	return king_num != 2;
}



//�����������
//˧/�� �ƶ���Χ��ֻ���ھŹ����ƶ��ƶ� ����ÿһ��ֻ����ˮƽ��ֱ�ƶ�һ�� �������˧�ͽ���׼��ͬһֱ����ֱ�Ӷ��棨�м������ӣ�����һ������ռ��λ�ã�����һ������رܣ����������
//��/ʿ �ƶ���Χ��ֻ���ھŹ����ƶ��ƶ� ����ÿһ��ֻ�����ضԽ��߷����ƶ�һ��
//��/�� �ƶ���Χ���ӽ��һ���ƶ� ����ÿһ��ֻ�����ضԽ��߷����ƶ����㣬��ʹ�ú����е���������ر��������ָ�ĶԽ��ߣ��׳��ࣨ�������֡����ࣨ������·���У�����������������ʱ�����ۼ������ǶԷ����ӣ����������߹�ȥ���׳ƣ����ࣨ���ۡ�
//�R �ƶ���Χ���κ�λ���ƶ� ����ÿһ��ֻ����ˮƽ��ֱ�ƶ�һ�㣬�ٰ��Խ��߷�������������ƶ�����ʹ�ú����е�����������������߷�ʽ���׳ƣ��������֣�б�Խ��ߣ������R����ʱ����һ��ֱ�л���д��б�����ӣ����ۼ������ǶԷ����ӣ���ס�������߹�ȥ���׳ƣ������ȡ�
//܇ �ƶ���Χ���κ�λ���ƶ� ���򣺿���ˮƽ��ֱ�����ƶ���������谭�ĵ�
//��/�h �ƶ���Χ���κ�λ���ƶ� �����ƶ������ͳ������ƣ�������������һ���������Ե��Է����ӡ�
//��/�� �ƶ���Χ���κ�λ���ƶ� ���򣺹��ӽ�ǰ��ÿ��ֻ����ǰ�ƶ�һ�㡣���ӽ���������������ƶ��������������䣩����������ƶ���
vector<PiecePosDesc> CChessEngine::GetAvailableSteps(PiecePosDesc pos)
{
	bool this_side = map.piece_side[pos.x][pos.y];
	bool cannon_jump_piece = 0;
	vector<PiecePosDesc> result;

	//fixme: ObeyRule_KingMeetKing always return 1
	if (this_side != current_side_red )
	{
		return vector<PiecePosDesc>();
	}

	bool restrict_Y_move = !ObeyRule_KingMeetKing(pos.x, pos.y);

	switch (GetPieceType(pos))
	{
	case CChessBase::PIECE_NULL:
		return vector<PiecePosDesc>();
		break;
	case CChessBase::PIECE_PAWN:
		if (this_side && pos.y <= 4)	//δ����
		{
			if (NotMySide(pos.x, pos.y + 1, this_side))	//���ܼ����ҷ�����λ��
			{
				result.emplace_back(pos.x, pos.y + 1);
			}
		}
		else if (!this_side && pos.y > 4)
		{
			if (NotMySide(pos.x, pos.y - 1, this_side))
			{
				result.emplace_back(pos.x, pos.y - 1);
			}
		}
		else if(this_side)   //����
		{
			if (pos.y <= 8)
			{
				if (NotMySide(pos.x, pos.y + 1, this_side))
				{
					result.emplace_back(pos.x, pos.y + 1);
				}
			}
			if (pos.x > 0 && !restrict_Y_move)
			{
				if (NotMySide(pos.x - 1, pos.y, this_side))
				{
					result.emplace_back(pos.x - 1, pos.y);
				}
			}
			if (pos.x < 8 && !restrict_Y_move)
			{
				if (NotMySide(pos.x + 1, pos.y, this_side))
				{
					result.emplace_back(pos.x + 1, pos.y);
				}
			}
		}
		else
		{
			if (pos.y > 0)
			{
				if (NotMySide(pos.x, pos.y-1, this_side))
				{
					result.emplace_back(pos.x, pos.y - 1);
				}
			}
			if (pos.x > 0 && !restrict_Y_move)
			{
				if (NotMySide(pos.x - 1, pos.y, this_side))
				{
					result.emplace_back(pos.x - 1, pos.y);
				}
			}
			if (pos.x < 8 && !restrict_Y_move)
			{
				if (NotMySide(pos.x + 1, pos.y, this_side))
				{
					result.emplace_back(pos.x + 1, pos.y);
				}
			}
		}
		break;
	case CChessBase::PIECE_ROOK:
		for (int i = pos.y + 1; i < 10; i++)
		{
			if (map.board[pos.x][i] != PIECE_NULL)
			{
				if (NotMySide(pos.x, i, this_side))
				{
					result.emplace_back(pos.x, i);
				}
				break;
			}
			result.emplace_back(pos.x, i);
		}
		for (int i = pos.y - 1; i >= 0; i--)
		{
			if (map.board[pos.x][i] != PIECE_NULL)
			{
				if (NotMySide(pos.x, i, this_side))
				{
					result.emplace_back(pos.x, i);
				}
				break;
			}
			result.emplace_back(pos.x, i);
		}
		if (restrict_Y_move)
		{
			break;
		}
		for (int i = pos.x - 1; i >= 0; i--)
		{
			if (map.board[i][pos.y] != PIECE_NULL)
			{
				if (NotMySide(i, pos.y, this_side))
				{
					result.emplace_back(i, pos.y);
				}
				break;
			}
			result.emplace_back(i, pos.y);
		}
		for (int i = pos.x + 1; i < 9; i++)
		{
			if (map.board[i][pos.y] != PIECE_NULL)
			{
				if (NotMySide(i, pos.y, this_side))
				{
					result.emplace_back(i, pos.y);
				}
				break;
			}
			result.emplace_back(i, pos.y);
		}
		
		break;
	case CChessBase::PIECE_HORSE:
		if (restrict_Y_move)
		{
			break;
		}
		if (PositionPieceNull(pos.x - 1, pos.y))
		{
			if (NotMySide(pos.x - 2, pos.y + 1, this_side))
			{
				result.emplace_back(pos.x - 2, pos.y + 1);
			}
			if (NotMySide(pos.x - 2, pos.y - 1, this_side))
			{
				result.emplace_back(pos.x - 2, pos.y - 1);
			}
		}
		if (PositionPieceNull(pos.x + 1, pos.y))
		{
			if (NotMySide(pos.x + 2, pos.y + 1, this_side))
			{
				result.emplace_back(pos.x + 2, pos.y + 1);
			}
			if (NotMySide(pos.x + 2, pos.y - 1, this_side))
			{
				result.emplace_back(pos.x + 2, pos.y - 1);
			}
		}
		if (PositionPieceNull(pos.x, pos.y + 1))
		{
			if (NotMySide(pos.x + 1, pos.y + 2, this_side))
			{
				result.emplace_back(pos.x + 1, pos.y + 2);
			}
			if (NotMySide(pos.x - 1, pos.y + 2, this_side))
			{
				result.emplace_back(pos.x - 1, pos.y + 2);
			}
		}
		if (PositionPieceNull(pos.x, pos.y - 1))
		{
			if (NotMySide(pos.x + 1, pos.y - 2, this_side))
			{
				result.emplace_back(pos.x + 1, pos.y - 2);
			}
			if (NotMySide(pos.x - 1, pos.y - 2, this_side))
			{
				result.emplace_back(pos.x - 1, pos.y - 2);
			}
		}
		break;
	case CChessBase::PIECE_ELEPHANT:
		if (restrict_Y_move)
		{
			break;
		}
		if (PositionPieceNull(pos.x - 1, pos.y - 1) && NotMySide(pos.x - 2, pos.y - 2, this_side))
		{
			result.emplace_back(pos.x - 2, pos.y - 2);
		}
		if (PositionPieceNull(pos.x - 1, pos.y + 1) && NotMySide(pos.x - 2, pos.y + 2, this_side))
		{
			result.emplace_back(pos.x - 2, pos.y + 2);
		}
		if (PositionPieceNull(pos.x + 1, pos.y - 1) && NotMySide(pos.x + 2, pos.y - 2, this_side))
		{
			result.emplace_back(pos.x + 2, pos.y - 2);
		}
		if (PositionPieceNull(pos.x + 1, pos.y + 1) && NotMySide(pos.x + 2, pos.y + 2, this_side))
		{
			result.emplace_back(pos.x + 2, pos.y + 2);
		}
		break;
	case CChessBase::PIECE_CANNON:
		cannon_jump_piece = 0;
		for (int i = pos.y + 1; i < 10; i++)
		{
			if (map.board[pos.x][i] != PIECE_NULL)
			{
				if (!cannon_jump_piece)
				{
					cannon_jump_piece = 1;
					continue;
				}
				if (NotMySide(pos.x, i, this_side))
				{
					result.emplace_back(pos.x, i);
				}
				
				break;
			}
			else if (!cannon_jump_piece)
			{
				result.emplace_back(pos.x, i);
			}
		}
		cannon_jump_piece = 0;
		for (int i = pos.y - 1; i >= 0; i--)
		{
			if (map.board[pos.x][i] != PIECE_NULL)
			{
				if (!cannon_jump_piece)
				{
					cannon_jump_piece = 1;
					continue;
				}
				if (NotMySide(pos.x, i, this_side))
				{
					result.emplace_back(pos.x, i);
				}
				
				break;
			}
			else if (!cannon_jump_piece)
			{
				result.emplace_back(pos.x, i);
			}
		}
		if (restrict_Y_move)
		{
			break;
		}
		cannon_jump_piece = 0;
		for (int i = pos.x - 1; i >= 0; i--)
		{
			if (map.board[i][pos.y] != PIECE_NULL)
			{
				if (!cannon_jump_piece)
				{
					cannon_jump_piece = 1;
					continue;
				}
				if (NotMySide(i, pos.y, this_side))
				{
					result.emplace_back(i, pos.y);
				}
				
				break;
			}
			else if (!cannon_jump_piece)
			{
				result.emplace_back(i, pos.y);
			}
		}
		cannon_jump_piece = 0;
		for (int i = pos.x + 1; i < 9; i++)
		{
			if (map.board[i][pos.y] != PIECE_NULL)
			{
				if (!cannon_jump_piece)
				{
					cannon_jump_piece = 1;
					continue;
				}
				if (NotMySide(i, pos.y, this_side))
				{
					result.emplace_back(i, pos.y);
				}
				
				break;
			}
			else if (!cannon_jump_piece)
			{
				result.emplace_back(i, pos.y);
			}
		}
		
		break;
	case CChessBase::PIECE_MANDARIN:
		if (restrict_Y_move)
		{
			break;
		}
		if (PositionInMandarinArea(pos.x + 1, pos.y + 1, this_side) && NotMySide(pos.x + 1, pos.y + 1, this_side))
		{
			result.emplace_back(pos.x + 1, pos.y + 1);
		}
		if (PositionInMandarinArea(pos.x + 1, pos.y - 1, this_side) && NotMySide(pos.x + 1, pos.y - 1, this_side))
		{
			result.emplace_back(pos.x + 1, pos.y - 1);
		}
		if (PositionInMandarinArea(pos.x - 1, pos.y + 1, this_side) && NotMySide(pos.x - 1, pos.y + 1, this_side))
		{
			result.emplace_back(pos.x - 1, pos.y + 1);
		}
		if (PositionInMandarinArea(pos.x - 1, pos.y - 1, this_side) && NotMySide(pos.x - 1, pos.y - 1, this_side))
		{
			result.emplace_back(pos.x - 1, pos.y - 1);
		}
		break;
	case CChessBase::PIECE_KING:
		//fixme: ObeyRule_KingMeetKing_for_king always return 1
		if (NotMySide(pos.x + 1, pos.y, this_side) && ObeyRule_KingMeetKing_for_king(pos.x + 1, pos.y))
		{
			result.emplace_back(pos.x + 1, pos.y);
		}
		if (NotMySide(pos.x - 1, pos.y, this_side) && ObeyRule_KingMeetKing_for_king(pos.x - 1, pos.y))
		{
			result.emplace_back(pos.x - 1, pos.y);
		}
		if (NotMySide(pos.x, pos.y - 1, this_side) && ObeyRule_KingMeetKing_for_king(pos.x, pos.y - 1))
		{
			result.emplace_back(pos.x, pos.y - 1);
		}
		if (NotMySide(pos.x, pos.y + 1, this_side) && ObeyRule_KingMeetKing_for_king(pos.x, pos.y + 1))
		{
			result.emplace_back(pos.x, pos.y + 1);
		}
		break;
	default:
		break;
	}
    return result;
}

bool CChessEngine::GetSide()
{
	return side_red;
}

bool CChessEngine::GetCurrentSide()
{
	return current_side_red;
}
