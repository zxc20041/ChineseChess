#include"CChessUI.h"

using namespace debugger;
using namespace CChessBase;


unordered_map<string, PIECE_ATLAS_INFO> PIECE_UI::piece_rect_set;
D2D1_RECT_F PIECE_UI::piece_rect[BOARD_X_MAX + 1][BOARD_Y_MAX + 1];
float PIECE_UI::map_line_x[BOARD_X_MAX + 1], PIECE_UI::map_line_y[BOARD_Y_MAX + 1];

CChessUI::CChessUI()
{
	memset(&map, 0, sizeof map);
	side_red = 1;
	piece_selected = 0;
	selected_piece_x = 0, selected_piece_y = 0;
}

CChessUI::~CChessUI()
{
}

void CChessUI::ClickAt(int line, int column)
{

}


void CChessUI::Reset()
{
	render.Reset();
	return;
}

bool CChessUI::LoadPiecesAtlasInfo()
{
	return render.LoadPiecesAtlasInfo();
}

void CChessUI::Rend()
{
	render.RendBG();
	render.RendPieces();
	return;
}

CChessUI::UIRender::UIRender()
{
	timeScale = 1;
	memset(pieces,0,sizeof(pieces));
	side_red = 1;
	

	map_area_posx1 = 380, map_area_posx2 = 1170, map_area_posy1 = 70, map_area_posy2 = 835;
	block_length_x = (map_area_posx2 - map_area_posx1) / 8;
	block_length_y = (map_area_posy2 - map_area_posy1) / 9;
	mark_lengthx_short = block_length_x * 0.05f, mark_lengthx_long = block_length_x * 0.2f;
	mark_lengthy_short = block_length_y * 0.05f, mark_lengthy_long = block_length_y * 0.2f;
	for (int i = 0; i < 9; i++)
	{
		map_line_x[i] = map_area_posx1 + block_length_x * i;
		PIECE_UI::map_line_x[i] = map_area_posx1 + block_length_x * i;
	}
	for (int i = 0; i < 10; i++)
	{
		map_line_y[i] = map_area_posy1 + block_length_y * i;
		PIECE_UI::map_line_y[i] = map_area_posy1 + block_length_y * i;
	}
	map_rect = D2D1::RectF(map_area_posx1, map_area_posy1, map_area_posx2, map_area_posy2);
	map_rect_extent = D2D1::RectF(map_area_posx1 * 1.02f - map_area_posx2 * 0.02f, map_area_posy1 * 1.02f - map_area_posy2 * 0.02f, map_area_posx2 * 1.02f - map_area_posx1 * 0.02f, map_area_posy2 * 1.02f - map_area_posy1 * 0.02f);
	river_rect= D2D1::RectF(map_area_posx1 * 0.5f + map_area_posx2 * 0.5f - (map_area_posy2 - map_area_posy1) * 3.5f/9, map_area_posy1 * 5 / 9 + map_area_posy2 * 4 / 9, map_area_posx1 * 0.5f + map_area_posx2 * 0.5f + (map_area_posy2 - map_area_posy1) * 3.5f/9, map_area_posy1 * 4 / 9 + map_area_posy2 * 5 / 9);
	board_rect = D2D1::RectF(map_area_posx1 * 1.05f - map_area_posx2 * 0.05f , map_area_posy1 * 1.05f - map_area_posy2 * 0.05f, map_area_posx2 * 1.05f - map_area_posx1 * 0.05f , map_area_posy2 * 1.05f - map_area_posy1 * 0.05f);
	for (int i = 0; i <= BOARD_X_MAX; i++)
	{
		for (int j = 0; j <= BOARD_Y_MAX; j++)
		{
			PIECE_UI::piece_rect[i][j] = D2D1::RectF(map_line_x[i] - block_length_x * 0.5f, map_line_y[j] - block_length_y * 0.5f, map_line_x[i] + block_length_x * 0.5f, map_line_y[j] + block_length_y * 0.5f);
		}
		
	}
}

CChessUI::UIRender::~UIRender()
{
	for (int i = 0; i < PIECE_NUM_MAX; i++)
	{
		if (pieces[i] != nullptr)
		{
			delete pieces[i];
			pieces[i] = nullptr;
		}
	}
}

void CChessUI::UIRender::UpdateAll()
{
}

void CChessUI::UIRender::RendPieces()
{
	for (auto &i : pieces)
	{
		i->rend(side_red);
	}
	return;
}

void CChessUI::UIRender::RendBG()
{
	FillRoundedRectangle_1(board_rect.left, board_rect.top, board_rect.right, board_rect.bottom, g_rm.getBrush("bg board"), 1.0f);
	
	//draw lines
	for (int i = 1; i <= 8; i++)
	{
		DrawLine_1(map_area_posx1, map_line_y[i], map_area_posx2, map_line_y[i], 3, g_pBrushBrown, 0.5f);
	}
	for (int i = 1; i <= 7; i++)
	{
		DrawLine_1(map_line_x[i], map_area_posy1, map_line_x[i], map_line_y[4], 3, g_pBrushBrown, 0.5f);
		DrawLine_1(map_line_x[i], map_line_y[5], map_line_x[i], map_area_posy2, 3, g_pBrushBrown, 0.5f);
	}
	for (int i = 0; i <= 8; i += 2)
	{
		if (i == 0)
		{
			DrawLine_1(map_line_x[i] + mark_lengthx_short, map_line_y[3] - mark_lengthy_short, map_line_x[i] + mark_lengthx_long, map_line_y[3] - mark_lengthy_short, 3, g_pBrushBrown, 0.5f);
			DrawLine_1(map_line_x[i] + mark_lengthx_short, map_line_y[3] - mark_lengthy_short, map_line_x[i] + mark_lengthx_short, map_line_y[3] - mark_lengthy_long, 3, g_pBrushBrown, 0.5f);
			DrawLine_1(map_line_x[i] + mark_lengthx_short, map_line_y[3] + mark_lengthy_short, map_line_x[i] + mark_lengthx_short, map_line_y[3] + mark_lengthy_long, 3, g_pBrushBrown, 0.5f);
			DrawLine_1(map_line_x[i] + mark_lengthx_short, map_line_y[3] + mark_lengthy_short, map_line_x[i] + mark_lengthx_long, map_line_y[3] + mark_lengthy_short, 3, g_pBrushBrown, 0.5f);

			DrawLine_1(map_line_x[i] + mark_lengthx_short, map_line_y[6] - mark_lengthy_short, map_line_x[i] + mark_lengthx_long, map_line_y[6] - mark_lengthy_short, 3, g_pBrushBrown, 0.5f);
			DrawLine_1(map_line_x[i] + mark_lengthx_short, map_line_y[6] - mark_lengthy_short, map_line_x[i] + mark_lengthx_short, map_line_y[6] - mark_lengthy_long, 3, g_pBrushBrown, 0.5f);
			DrawLine_1(map_line_x[i] + mark_lengthx_short, map_line_y[6] + mark_lengthy_short, map_line_x[i] + mark_lengthx_short, map_line_y[6] + mark_lengthy_long, 3, g_pBrushBrown, 0.5f);
			DrawLine_1(map_line_x[i] + mark_lengthx_short, map_line_y[6] + mark_lengthy_short, map_line_x[i] + mark_lengthx_long, map_line_y[6] + mark_lengthy_short, 3, g_pBrushBrown, 0.5f);
		}
		else if (i == 8)
		{
			DrawLine_1(map_line_x[i] - mark_lengthx_short, map_line_y[3] - mark_lengthy_short, map_line_x[i] - mark_lengthx_short, map_line_y[3] - mark_lengthy_long, 3, g_pBrushBrown, 0.5f);
			DrawLine_1(map_line_x[i] - mark_lengthx_short, map_line_y[3] - mark_lengthy_short, map_line_x[i] - mark_lengthx_long, map_line_y[3] - mark_lengthy_short, 3, g_pBrushBrown, 0.5f);
			DrawLine_1(map_line_x[i] - mark_lengthx_short, map_line_y[3] + mark_lengthy_short, map_line_x[i] - mark_lengthx_short, map_line_y[3] + mark_lengthy_long, 3, g_pBrushBrown, 0.5f);
			DrawLine_1(map_line_x[i] - mark_lengthx_short, map_line_y[3] + mark_lengthy_short, map_line_x[i] - mark_lengthx_long, map_line_y[3] + mark_lengthy_short, 3, g_pBrushBrown, 0.5f);

			DrawLine_1(map_line_x[i] - mark_lengthx_short, map_line_y[6] - mark_lengthy_short, map_line_x[i] - mark_lengthx_short, map_line_y[6] - mark_lengthy_long, 3, g_pBrushBrown, 0.5f);
			DrawLine_1(map_line_x[i] - mark_lengthx_short, map_line_y[6] - mark_lengthy_short, map_line_x[i] - mark_lengthx_long, map_line_y[6] - mark_lengthy_short, 3, g_pBrushBrown, 0.5f);
			DrawLine_1(map_line_x[i] - mark_lengthx_short, map_line_y[6] + mark_lengthy_short, map_line_x[i] - mark_lengthx_short, map_line_y[6] + mark_lengthy_long, 3, g_pBrushBrown, 0.5f);
			DrawLine_1(map_line_x[i] - mark_lengthx_short, map_line_y[6] + mark_lengthy_short, map_line_x[i] - mark_lengthx_long, map_line_y[6] + mark_lengthy_short, 3, g_pBrushBrown, 0.5f);
		}
		else
		{
			DrawLine_1(map_line_x[i] - mark_lengthx_short, map_line_y[3] - mark_lengthy_short, map_line_x[i] - mark_lengthx_short, map_line_y[3] - mark_lengthy_long, 3, g_pBrushBrown, 0.5f);
			DrawLine_1(map_line_x[i] - mark_lengthx_short, map_line_y[3] - mark_lengthy_short, map_line_x[i] - mark_lengthx_long, map_line_y[3] - mark_lengthy_short, 3, g_pBrushBrown, 0.5f);
			DrawLine_1(map_line_x[i] - mark_lengthx_short, map_line_y[3] + mark_lengthy_short, map_line_x[i] - mark_lengthx_short, map_line_y[3] + mark_lengthy_long, 3, g_pBrushBrown, 0.5f);
			DrawLine_1(map_line_x[i] - mark_lengthx_short, map_line_y[3] + mark_lengthy_short, map_line_x[i] - mark_lengthx_long, map_line_y[3] + mark_lengthy_short, 3, g_pBrushBrown, 0.5f);
			DrawLine_1(map_line_x[i] + mark_lengthx_short, map_line_y[3] - mark_lengthy_short, map_line_x[i] + mark_lengthx_long, map_line_y[3] - mark_lengthy_short, 3, g_pBrushBrown, 0.5f);
			DrawLine_1(map_line_x[i] + mark_lengthx_short, map_line_y[3] - mark_lengthy_short, map_line_x[i] + mark_lengthx_short, map_line_y[3] - mark_lengthy_long, 3, g_pBrushBrown, 0.5f);
			DrawLine_1(map_line_x[i] + mark_lengthx_short, map_line_y[3] + mark_lengthy_short, map_line_x[i] + mark_lengthx_short, map_line_y[3] + mark_lengthy_long, 3, g_pBrushBrown, 0.5f);
			DrawLine_1(map_line_x[i] + mark_lengthx_short, map_line_y[3] + mark_lengthy_short, map_line_x[i] + mark_lengthx_long, map_line_y[3] + mark_lengthy_short, 3, g_pBrushBrown, 0.5f);
			
			DrawLine_1(map_line_x[i] - mark_lengthx_short, map_line_y[6] - mark_lengthy_short, map_line_x[i] - mark_lengthx_short, map_line_y[6] - mark_lengthy_long, 3, g_pBrushBrown, 0.5f);
			DrawLine_1(map_line_x[i] - mark_lengthx_short, map_line_y[6] - mark_lengthy_short, map_line_x[i] - mark_lengthx_long, map_line_y[6] - mark_lengthy_short, 3, g_pBrushBrown, 0.5f);
			DrawLine_1(map_line_x[i] - mark_lengthx_short, map_line_y[6] + mark_lengthy_short, map_line_x[i] - mark_lengthx_short, map_line_y[6] + mark_lengthy_long, 3, g_pBrushBrown, 0.5f);
			DrawLine_1(map_line_x[i] - mark_lengthx_short, map_line_y[6] + mark_lengthy_short, map_line_x[i] - mark_lengthx_long, map_line_y[6] + mark_lengthy_short, 3, g_pBrushBrown, 0.5f);
			DrawLine_1(map_line_x[i] + mark_lengthx_short, map_line_y[6] - mark_lengthy_short, map_line_x[i] + mark_lengthx_long, map_line_y[6] - mark_lengthy_short, 3, g_pBrushBrown, 0.5f);
			DrawLine_1(map_line_x[i] + mark_lengthx_short, map_line_y[6] - mark_lengthy_short, map_line_x[i] + mark_lengthx_short, map_line_y[6] - mark_lengthy_long, 3, g_pBrushBrown, 0.5f);
			DrawLine_1(map_line_x[i] + mark_lengthx_short, map_line_y[6] + mark_lengthy_short, map_line_x[i] + mark_lengthx_short, map_line_y[6] + mark_lengthy_long, 3, g_pBrushBrown, 0.5f);
			DrawLine_1(map_line_x[i] + mark_lengthx_short, map_line_y[6] + mark_lengthy_short, map_line_x[i] + mark_lengthx_long, map_line_y[6] + mark_lengthy_short, 3, g_pBrushBrown, 0.5f);
		}
	}
	for (int i : {1, 7})
	{
		for (int j : {2, 7})
		{
			DrawLine_1(map_line_x[i] - mark_lengthx_short, map_line_y[j] - mark_lengthy_short, map_line_x[i] - mark_lengthx_short, map_line_y[j] - mark_lengthy_long, 3, g_pBrushBrown, 0.5f);
			DrawLine_1(map_line_x[i] - mark_lengthx_short, map_line_y[j] - mark_lengthy_short, map_line_x[i] - mark_lengthx_long, map_line_y[j] - mark_lengthy_short, 3, g_pBrushBrown, 0.5f);
			DrawLine_1(map_line_x[i] - mark_lengthx_short, map_line_y[j] + mark_lengthy_short, map_line_x[i] - mark_lengthx_short, map_line_y[j] + mark_lengthy_long, 3, g_pBrushBrown, 0.5f);
			DrawLine_1(map_line_x[i] - mark_lengthx_short, map_line_y[j] + mark_lengthy_short, map_line_x[i] - mark_lengthx_long, map_line_y[j] + mark_lengthy_short, 3, g_pBrushBrown, 0.5f);
			DrawLine_1(map_line_x[i] + mark_lengthx_short, map_line_y[j] - mark_lengthy_short, map_line_x[i] + mark_lengthx_long, map_line_y[j] - mark_lengthy_short, 3, g_pBrushBrown, 0.5f);
			DrawLine_1(map_line_x[i] + mark_lengthx_short, map_line_y[j] - mark_lengthy_short, map_line_x[i] + mark_lengthx_short, map_line_y[j] - mark_lengthy_long, 3, g_pBrushBrown, 0.5f);
			DrawLine_1(map_line_x[i] + mark_lengthx_short, map_line_y[j] + mark_lengthy_short, map_line_x[i] + mark_lengthx_short, map_line_y[j] + mark_lengthy_long, 3, g_pBrushBrown, 0.5f);
			DrawLine_1(map_line_x[i] + mark_lengthx_short, map_line_y[j] + mark_lengthy_short, map_line_x[i] + mark_lengthx_long, map_line_y[j] + mark_lengthy_short, 3, g_pBrushBrown, 0.5f);
		}
	}

	DrawLine_1(map_line_x[3], map_line_y[0], map_line_x[5], map_line_y[2], 3, g_pBrushBrown, 0.5f);
	DrawLine_1(map_line_x[3], map_line_y[2], map_line_x[5], map_line_y[0], 3, g_pBrushBrown, 0.5f);
	DrawLine_1(map_line_x[3], map_line_y[7], map_line_x[5], map_line_y[9], 3, g_pBrushBrown, 0.5f);
	DrawLine_1(map_line_x[3], map_line_y[9], map_line_x[5], map_line_y[7], 3, g_pBrushBrown, 0.5f);

	DrawRectangle_1(map_rect.left, map_rect.top, map_rect.right, map_rect.bottom, 3, g_pBrushBrown, 0.5f);
	DrawRectangle_1(map_rect_extent.left, map_rect_extent.top, map_rect_extent.right, map_rect_extent.bottom, 6, g_pBrushBrown, 0.5f);
	DrawBitmap_1(g_rm.getTexture("bg river"), river_rect, 0.75f);

	return;
}


void CChessUI::UIRender::Reset()
{
	//0-15(black) at top, 16-31(red) at bottom of board
	for (int i = 0; i < PIECE_NUM_MAX; i++)
	{
		if (pieces[i] != nullptr)
		{
			delete pieces[i];
			pieces[i] = nullptr;
		}
	}

	pieces[0] = new PIECE_UI(BOARD_X_MIN, BOARD_Y_MIN, ChessPieceType::PIECE_ROOK, 0);
	pieces[1] = new PIECE_UI(BOARD_X_MIN + 1, BOARD_Y_MIN, ChessPieceType::PIECE_HORSE, 0);
	pieces[2] = new PIECE_UI(BOARD_X_MIN + 2, BOARD_Y_MIN, ChessPieceType::PIECE_ELEPHANT, 0);
	pieces[3] = new PIECE_UI(BOARD_X_MIN + 3, BOARD_Y_MIN, ChessPieceType::PIECE_MANDARIN, 0);
	pieces[4] = new PIECE_UI(BOARD_X_MIN + 4, BOARD_Y_MIN, ChessPieceType::PIECE_KING, 0);
	pieces[5] = new PIECE_UI(BOARD_X_MAX, BOARD_Y_MIN, ChessPieceType::PIECE_ROOK, 0);
	pieces[6] = new PIECE_UI(BOARD_X_MAX - 1, BOARD_Y_MIN, ChessPieceType::PIECE_HORSE, 0);
	pieces[7] = new PIECE_UI(BOARD_X_MAX - 2, BOARD_Y_MIN, ChessPieceType::PIECE_ELEPHANT, 0);
	pieces[8] = new PIECE_UI(BOARD_X_MAX - 3, BOARD_Y_MIN, ChessPieceType::PIECE_MANDARIN, 0);
	pieces[9] = new PIECE_UI(BOARD_X_MIN + 1, BOARD_Y_MIN+2, ChessPieceType::PIECE_CANNON, 0);
	pieces[10] = new PIECE_UI(BOARD_X_MAX - 1, BOARD_Y_MIN+ 2, ChessPieceType::PIECE_CANNON, 0);
	pieces[11] = new PIECE_UI(BOARD_X_MIN, BOARD_Y_MIN + 3, ChessPieceType::PIECE_PAWN, 0);
	pieces[12] = new PIECE_UI(BOARD_X_MIN + 2, BOARD_Y_MIN + 3, ChessPieceType::PIECE_PAWN, 0);
	pieces[13] = new PIECE_UI(BOARD_X_MIN + 4, BOARD_Y_MIN + 3, ChessPieceType::PIECE_PAWN, 0);
	pieces[14] = new PIECE_UI(BOARD_X_MIN + 6, BOARD_Y_MIN + 3, ChessPieceType::PIECE_PAWN, 0);
	pieces[15] = new PIECE_UI(BOARD_X_MAX, BOARD_Y_MIN + 3, ChessPieceType::PIECE_PAWN, 0);

	pieces[16] = new PIECE_UI(BOARD_X_MIN, BOARD_Y_MAX, ChessPieceType::PIECE_ROOK, 1);
	pieces[17] = new PIECE_UI(BOARD_X_MIN + 1, BOARD_Y_MAX, ChessPieceType::PIECE_HORSE, 1);
	pieces[18] = new PIECE_UI(BOARD_X_MIN + 2, BOARD_Y_MAX, ChessPieceType::PIECE_ELEPHANT, 1);
	pieces[19] = new PIECE_UI(BOARD_X_MIN + 3, BOARD_Y_MAX, ChessPieceType::PIECE_MANDARIN, 1);
	pieces[20] = new PIECE_UI(BOARD_X_MIN + 4, BOARD_Y_MAX, ChessPieceType::PIECE_KING, 1);
	pieces[21] = new PIECE_UI(BOARD_X_MAX, BOARD_Y_MAX, ChessPieceType::PIECE_ROOK, 1);
	pieces[22] = new PIECE_UI(BOARD_X_MAX - 1, BOARD_Y_MAX, ChessPieceType::PIECE_HORSE, 1);
	pieces[23] = new PIECE_UI(BOARD_X_MAX - 2, BOARD_Y_MAX, ChessPieceType::PIECE_ELEPHANT, 1);
	pieces[24] = new PIECE_UI(BOARD_X_MAX - 3, BOARD_Y_MAX, ChessPieceType::PIECE_MANDARIN, 1);
	pieces[25] = new PIECE_UI(BOARD_X_MIN + 1, BOARD_Y_MAX - 2, ChessPieceType::PIECE_CANNON, 1);
	pieces[26] = new PIECE_UI(BOARD_X_MAX - 1, BOARD_Y_MAX - 2, ChessPieceType::PIECE_CANNON, 1);
	pieces[27] = new PIECE_UI(BOARD_X_MIN, BOARD_Y_MAX - 3, ChessPieceType::PIECE_PAWN, 1);
	pieces[28] = new PIECE_UI(BOARD_X_MIN + 2, BOARD_Y_MAX - 3, ChessPieceType::PIECE_PAWN, 1);
	pieces[29] = new PIECE_UI(BOARD_X_MIN + 4, BOARD_Y_MAX - 3, ChessPieceType::PIECE_PAWN, 1);
	pieces[30] = new PIECE_UI(BOARD_X_MIN + 6, BOARD_Y_MAX - 3, ChessPieceType::PIECE_PAWN, 1);
	pieces[31] = new PIECE_UI(BOARD_X_MAX, BOARD_Y_MAX - 3, ChessPieceType::PIECE_PAWN, 1);

	
	return;
}

bool CChessUI::UIRender::LoadPiecesAtlasInfo()
{
	TEXT_RESOURCE* text = g_rm.getText("pieces atlas info");
	int cnt = 0, index = 0;
	if (text == nullptr)
	{
		debugger_main.writelog(DERROR, "pieces atlas data not found", __LINE__);
		return 0;
	}
	string strbuf[64][4];
	for (int i = 0; i < text->lines; i++)
	{
		while (text->content[i][cnt] != '\0')
		{
			if (text->content[i][cnt] == ' ')
			{
				index++;
				cnt++;
				if (index == 4)
				{
					break;
				}
				continue;
			}
			strbuf[i][index] += text->content[i][cnt];
			cnt++;
		}
		cnt = 0;
		index = 0;
	}
	//find corresponding data
	
	PIECE_UI::piece_rect_set.clear();
	for (int i = 0; i < text->lines; i++)
	{
		for (auto& j : piece_names)
		{
			if (strbuf[i][0] == addFormat(j))
			{
				PIECE_ATLAS_INFO rect;
				try
				{
					rect.static_rect.left = stoi(strbuf[i + 1][0]), rect.static_rect.top = stoi(strbuf[i + 1][1]), rect.static_rect.right = stoi(strbuf[i + 1][2]), rect.static_rect.bottom = stoi(strbuf[i + 1][3]);
					rect.up_rect.left = stoi(strbuf[i + 2][0]), rect.up_rect.top = stoi(strbuf[i + 2][1]), rect.up_rect.right = stoi(strbuf[i + 2][2]), rect.up_rect.bottom = stoi(strbuf[i + 2][3]);
					rect.down_rect.left = stoi(strbuf[i + 3][0]), rect.down_rect.top = stoi(strbuf[i + 3][1]), rect.down_rect.right = stoi(strbuf[i + 3][2]), rect.down_rect.bottom = stoi(strbuf[i + 3][3]);
				}
				catch (exception e)
				{
					debugger_main.writelog(DWARNNING, "exception when stoi in CChessUI::UIRender::LoadPiecesAtlasInfo(): " + string(e.what()),__LINE__);
					return 0;
				}
				PIECE_UI::piece_rect_set[j] = rect;
				break;
			}
		}
	}
	for (auto& j : piece_names)
	{
		if (PIECE_UI::piece_rect_set.find(j) == PIECE_UI::piece_rect_set.end())
		{
			debugger_main.writelog(DWARNNING, "info not complete in CChessUI::UIRender::LoadPiecesAtlasInfo() " + string(j), __LINE__);
			return 0;
		}
	}
	return 1;
}

void CChessUI::UIRender::SetSide(bool side_red)
{
	this->side_red = side_red;
	return;
}

void CChessUI::UIRender::SelectPiece(int x, int y)
{
}

void CChessUI::UIRender::MoveTo(int x, int y, bool eat)
{
}

void CChessUI::UIRender::Eat(int x, int y)
{
}

PIECE_UI::PIECE_UI(int x, int y, ChessPieceType type, bool side_red)
{
	this->x = x, this->y = y;
	posx = map_line_x[x];
	posy = map_line_y[y];
	this->type = type;
	this->side_red = side_red;
	status = Piece_Move_Status::PIECE_STATIC;
	if (side_red)
	{
		switch (type)
		{
		case CChessBase::PIECE_NULL:
			break;
		case CChessBase::PIECE_PAWN:
			static_rect = piece_rect_set["red_pawn"].static_rect;
			up_rect = piece_rect_set["red_pawn"].up_rect;
			down_rect = piece_rect_set["red_pawn"].down_rect;
			break;
		case CChessBase::PIECE_ROOK:
			static_rect = piece_rect_set["red_rook"].static_rect;
			up_rect = piece_rect_set["red_rook"].up_rect;
			down_rect = piece_rect_set["red_rook"].down_rect;
			break;
		case CChessBase::PIECE_HORSE:
			static_rect = piece_rect_set["red_horse"].static_rect;
			up_rect = piece_rect_set["red_horse"].up_rect;
			down_rect = piece_rect_set["red_horse"].down_rect;
			break;
		case CChessBase::PIECE_ELEPHANT:
			static_rect = piece_rect_set["red_elephant"].static_rect;
			up_rect = piece_rect_set["red_elephant"].up_rect;
			down_rect = piece_rect_set["red_elephant"].down_rect;
			break;
		case CChessBase::PIECE_CANNON:
			static_rect = piece_rect_set["red_cannon"].static_rect;
			up_rect = piece_rect_set["red_cannon"].up_rect;
			down_rect = piece_rect_set["red_cannon"].down_rect;
			break;
		case CChessBase::PIECE_MANDARIN:
			static_rect = piece_rect_set["red_mandarin"].static_rect;
			up_rect = piece_rect_set["red_mandarin"].up_rect;
			down_rect = piece_rect_set["red_mandarin"].down_rect;
			break;
		case CChessBase::PIECE_KING:
			static_rect = piece_rect_set["red_king"].static_rect;
			up_rect = piece_rect_set["red_king"].up_rect;
			down_rect = piece_rect_set["red_king"].down_rect;
			break;
		default:
			break;
		}
	}
	else
	{
		switch (type)
		{
		case CChessBase::PIECE_NULL:
			break;
		case CChessBase::PIECE_PAWN:
			static_rect = piece_rect_set["black_pawn"].static_rect;
			up_rect = piece_rect_set["black_pawn"].up_rect;
			down_rect = piece_rect_set["black_pawn"].down_rect;
			break;
		case CChessBase::PIECE_ROOK:
			static_rect = piece_rect_set["black_rook"].static_rect;
			up_rect = piece_rect_set["black_rook"].up_rect;
			down_rect = piece_rect_set["black_rook"].down_rect;
			break;
		case CChessBase::PIECE_HORSE:
			static_rect = piece_rect_set["black_horse"].static_rect;
			up_rect = piece_rect_set["black_horse"].up_rect;
			down_rect = piece_rect_set["black_horse"].down_rect;
			break;
		case CChessBase::PIECE_ELEPHANT:
			static_rect = piece_rect_set["black_elephant"].static_rect;
			up_rect = piece_rect_set["black_elephant"].up_rect;
			down_rect = piece_rect_set["black_elephant"].down_rect;
			break;
		case CChessBase::PIECE_CANNON:
			static_rect = piece_rect_set["black_cannon"].static_rect;
			up_rect = piece_rect_set["black_cannon"].up_rect;
			down_rect = piece_rect_set["black_cannon"].down_rect;
			break;
		case CChessBase::PIECE_MANDARIN:
			static_rect = piece_rect_set["black_mandarin"].static_rect;
			up_rect = piece_rect_set["black_mandarin"].up_rect;
			down_rect = piece_rect_set["black_mandarin"].down_rect;
			break;
		case CChessBase::PIECE_KING:
			static_rect = piece_rect_set["black_king"].static_rect;
			up_rect = piece_rect_set["black_king"].up_rect;
			down_rect = piece_rect_set["black_king"].down_rect;
			break;
		default:
			break;
		}
	}
	return;
}

void PIECE_UI::rend(bool board_side_red)
{
	if (x > BOARD_X_MAX || y > BOARD_Y_MAX)
	{
		debugger_main.add_output_line("illegal x,y in CChessUI::UIRender::RendStaticPiece() " + to_string(x) + "," + to_string(y));
		return;
	}
	if (type == PIECE_NULL)
	{
		return;
	}
	ID2D1Bitmap* texture = g_rm.getTexture("pieces");

	if (board_side_red)
	{

		DrawBitmap_1(texture, piece_rect[x][y], static_rect, 1.0f);
	}
	else
	{
		//reflect position y
		DrawBitmap_1(texture, piece_rect[x][BOARD_Y_MAX-y], static_rect, 1.0f);
	}
	return;
}

void PIECE_UI::update()
{
}
