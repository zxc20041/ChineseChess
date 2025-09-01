#include"CChessUIPVE.h"

using namespace std;
using namespace debugger;

CChessUIPVE::CChessUIPVE() :CChessUI()
{
	
}

CChessUIPVE::~CChessUIPVE()
{
}

void CChessUIPVE::ClickAt(int x, int y)
{
	debugger_main.add_tagline("ClickAt " + to_string(x) + "," + to_string(y) + " piece type= " + to_string(map.board[x][y]));
	if (piece_selected)
	{
		bool valid = 0;
		for (auto& i : availablePosition)
		{
			if (i.x == x && i.y == y)
			{
				valid = 1;
				engine->MovePiece(PieceMoveDesc{ selected_piece_x, selected_piece_y, x, y });
				engine->SearchBestMove();
				render.MovePiece(PieceMoveDesc{ selected_piece_x, selected_piece_y, x,y }, map.board[x][y] != PIECE_NULL);
				map = engine->GetMap();
				debugger_main.add_tagline("MovePiece " + to_string(selected_piece_x) + "," + to_string(selected_piece_y) + " -> " + to_string(x) + "," + to_string(y));
				break;
			}
		}
		if (!valid)
		{
			render.UnSelectPiece(selected_piece_x, selected_piece_y);
			debugger_main.add_tagline("UnSelectPiece " + to_string(selected_piece_x) + "," + to_string(selected_piece_y) + " -> " + to_string(x) + "," + to_string(y));
		}
		piece_selected = 0;
	}
	else
	{
		if (map.board[x][y] != PIECE_NULL && map.piece_side[x][y] == engine->GetCurrentSide() && side_red == engine->GetCurrentSide())
		{
			availablePosition = engine->SelectPiece(PiecePosDesc(x, y));
			render.SetAvailablePosition(availablePosition);
			render.SelectPiece(x, y);

			selected_piece_x = x, selected_piece_y = y;
			piece_selected = 1;
			debugger_main.add_tagline("SelectPiece " + to_string(selected_piece_x) + "," + to_string(selected_piece_y) + " -> " + to_string(x) + "," + to_string(y));
		}
	}
	return;
}

void CChessUIPVE::Update()
{
	if (side_red == engine->GetCurrentSide())
	{
		
		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				boxes[i][j]->check(1);
				if (boxes[i][j]->getClicked())
				{
					boxes[i][j]->reset_click_status();
					ClickAt(i, j);
				}
			}
		}
	}
	else
	{
		if (engine->CheckBestMove())
		{
			auto bestMove = engine->GetBestMove();
			engine->MovePiece(bestMove);
			render.SelectPiece(bestMove.fromx, bestMove.fromy);
			render.MovePiece(bestMove, map.board[bestMove.tox][bestMove.toy] != PIECE_NULL);
			map = engine->GetMap();
			debugger_main.add_tagline("MovePiece " + to_string(bestMove.fromx) + "," + to_string(bestMove.fromy) + " -> " + to_string(bestMove.tox) + "," + to_string(bestMove.toy));
		}
	}
	engine->Update();

	EngineResult r = engine->GetResult();
	if (r.valid)
	{
		if (r.result == RESULT_MATE)
		{
			render.GameOver(r.win_side);
		}
	}

	render.Update();
	return;
}
