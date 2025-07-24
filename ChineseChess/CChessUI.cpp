#include"CChessUI.h"

using namespace debugger;
using namespace CChessBase;

CChessUI::CChessUI()
{
	memset(&map, 0, sizeof map);
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
	timeScale=1;
	memset(pieces,0,sizeof(pieces));

	side_red = 1;
	piece_selected = 0;
	selected_piece_x = 0, selected_piece_y = 0;

	map_area_posx1 = 350, map_area_posx2 = 1200, map_area_posy1 = 70, map_area_posy2 = 835;
	block_length_x = (map_area_posx2 - map_area_posx1) / 8;
	block_length_y = (map_area_posy2 - map_area_posy1) / 9;
	mark_lengthx_short = block_length_x * 0.05f, mark_lengthx_long = block_length_x * 0.2f;
	mark_lengthy_short = block_length_y * 0.05f, mark_lengthy_long = block_length_y * 0.2f;
	for (int i = 0; i < 9; i++)
	{
		map_line_x[i] = map_area_posx1 + block_length_x * i;
	}
	for (int i = 0; i < 10; i++)
	{
		map_line_y[i] = map_area_posy1 + block_length_y * i;
	}
	map_rect = D2D1::RectF(map_area_posx1, map_area_posy1, map_area_posx2, map_area_posy2);
	map_rect_extent = D2D1::RectF(map_area_posx1 * 1.02f - map_area_posx2 * 0.02f, map_area_posy1 * 1.02f - map_area_posy2 * 0.02f, map_area_posx2 * 1.02f - map_area_posx1 * 0.02f, map_area_posy2 * 1.02f - map_area_posy1 * 0.02f);
	river_rect= D2D1::RectF(map_area_posx1 * 0.5f + map_area_posx2 * 0.5f - (map_area_posy2 - map_area_posy1) * 3.5f/9, map_area_posy1 * 5 / 9 + map_area_posy2 * 4 / 9, map_area_posx1 * 0.5f + map_area_posx2 * 0.5f + (map_area_posy2 - map_area_posy1) * 3.5f/9, map_area_posy1 * 4 / 9 + map_area_posy2 * 5 / 9);
	board_rect = D2D1::RectF(map_area_posx1 * 1.05f - map_area_posx2 * 0.05f , map_area_posy1 * 1.05f - map_area_posy2 * 0.05f, map_area_posx2 * 1.05f - map_area_posx1 * 0.05f , map_area_posy2 * 1.05f - map_area_posy1 * 0.05f);
	for (int i = 0; i <= BOARD_X_MAX; i++)
	{
		for (int j = 0; j <= BOARD_Y_MAX; j++)
		{
			piece_rect[i][j] = D2D1::RectF(map_line_x[i] - block_length_x * 0.5f, map_line_y[j] - block_length_y * 0.5f, map_line_x[i] + block_length_x * 0.5f, map_line_y[j] + block_length_y * 0.5f);
		}
		
	}
}

CChessUI::UIRender::~UIRender()
{
}

void CChessUI::UIRender::UpdateAll()
{
}

void CChessUI::UIRender::RendStaticPiece(PIECE_UI &piece)
{
	if (piece.x > BOARD_X_MAX || piece.y > BOARD_Y_MAX)
	{
		debugger_main.add_output_line("illegal x,y in CChessUI::UIRender::RendStaticPiece() " + to_string(piece.x) + "," + to_string(piece.y));
		return;
	}
	ID2D1Bitmap* texture = g_rm.getTexture("pieces");
	if (piece.side_red)
	{
		switch (piece.type)
		{
		case CChessBase::PIECE_NULL:
			break;
		case CChessBase::PIECE_PAWN:
			DrawBitmap_1(texture, piece_rect[piece.x][piece.y], piece_rect_set["red_pawn"].static_rect,1.0f);
			break;
		case CChessBase::PIECE_ROOK:
			DrawBitmap_1(texture, piece_rect[piece.x][piece.y], piece_rect_set["red_rook"].static_rect, 1.0f);
			break;
		case CChessBase::PIECE_HORSE:
			DrawBitmap_1(texture, piece_rect[piece.x][piece.y], piece_rect_set["red_horse"].static_rect, 1.0f);
			break;
		case CChessBase::PIECE_ELEPHANT:
			DrawBitmap_1(texture, piece_rect[piece.x][piece.y], piece_rect_set["red_elephant"].static_rect, 1.0f);
			break;
		case CChessBase::PIECE_CANNON:
			DrawBitmap_1(texture, piece_rect[piece.x][piece.y], piece_rect_set["red_connon"].static_rect, 1.0f);
			break;
		case CChessBase::PIECE_MANDARIN:
			DrawBitmap_1(texture, piece_rect[piece.x][piece.y], piece_rect_set["red_mandarin"].static_rect, 1.0f);
			break;
		case CChessBase::PIECE_KING:
			DrawBitmap_1(texture, piece_rect[piece.x][piece.y], piece_rect_set["red_king"].static_rect, 1.0f);
			break;
		default:
			break;
		}
	}
	else
	{
		switch (piece.type)
		{
		case CChessBase::PIECE_NULL:
			break;
		case CChessBase::PIECE_PAWN:
			DrawBitmap_1(texture, piece_rect[piece.x][piece.y], piece_rect_set["black_pawn"].static_rect, 1.0f);
			break;
		case CChessBase::PIECE_ROOK:
			DrawBitmap_1(texture, piece_rect[piece.x][piece.y], piece_rect_set["black_rook"].static_rect, 1.0f);
			break;
		case CChessBase::PIECE_HORSE:
			DrawBitmap_1(texture, piece_rect[piece.x][piece.y], piece_rect_set["black_horse"].static_rect, 1.0f);
			break;
		case CChessBase::PIECE_ELEPHANT:
			DrawBitmap_1(texture, piece_rect[piece.x][piece.y], piece_rect_set["black_elephant"].static_rect, 1.0f);
			break;
		case CChessBase::PIECE_CANNON:
			DrawBitmap_1(texture, piece_rect[piece.x][piece.y], piece_rect_set["black_connon"].static_rect, 1.0f);
			break;
		case CChessBase::PIECE_MANDARIN:
			DrawBitmap_1(texture, piece_rect[piece.x][piece.y], piece_rect_set["black_mandarin"].static_rect, 1.0f);
			break;
		case CChessBase::PIECE_KING:
			DrawBitmap_1(texture, piece_rect[piece.x][piece.y], piece_rect_set["black_king"].static_rect, 1.0f);
			break;
		default:
			break;
		}
	}
	return;
}

void CChessUI::UIRender::RendPieces()
{
	for (auto &i : pieces)
	{
		if (i.status == PIECE_STATIC)
		{
			RendStaticPiece(i);
		}
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
	//0-15 at bottom, 16-31 at top of board

	for (int i = 0; i < 32; i++)
	{
		pieces[i].moving_time = 0;
		pieces[i].status = PIECE_STATIC;
	}
	if (side_red)
	{
		for (int i = 0; i < 16; i++)
		{
			pieces[i].side_red = 1;
			pieces[i + 16].side_red = 0;
		}
	}
	else
	{
		for (int i = 0; i < 16; i++)
		{
			pieces[i + 16].side_red = 1;
			pieces[i].side_red = 0;
		}
	}
	for (const int offset : {0, 16})
	{
		pieces[BOARD_X_MIN + offset].type = pieces[BOARD_X_MAX + offset].type = PIECE_ROOK;
		pieces[BOARD_X_MIN + 1 + offset].type = pieces[BOARD_X_MAX - 1 + offset].type = PIECE_HORSE;
		pieces[BOARD_X_MIN + 2 + offset].type = pieces[BOARD_X_MAX - 2 + offset].type = PIECE_ELEPHANT;
		pieces[BOARD_X_MIN + 3 + offset].type = pieces[BOARD_X_MAX - 3 + offset].type = PIECE_MANDARIN;
		pieces[BOARD_X_MIN + 4 + offset].type = PIECE_KING;
		pieces[BOARD_X_MAX + 1 + offset].type = pieces[BOARD_X_MAX + 2 + offset].type = PIECE_CANNON;
		for (int j = 3; j < 8; j++)
		{
			pieces[BOARD_X_MAX + j + offset].type = PIECE_PAWN;
		}
	}
	
	for (int i = 0; i < 9; i++)
	{
		pieces[i].x = BOARD_X_MIN + i, pieces[i].y = BOARD_Y_MAX;
		pieces[i + 16].x = BOARD_X_MIN + i, pieces[i + 16].y = BOARD_Y_MIN;
	}
	
	//PIECE_CANNON
	pieces[9].x = BOARD_X_MIN + 1, pieces[9].y = BOARD_Y_MAX - 2;
	pieces[10].x = BOARD_X_MAX - 1, pieces[10].y = BOARD_Y_MAX - 2;
	pieces[25].x = BOARD_X_MIN + 1, pieces[25].y = BOARD_Y_MIN + 2;
	pieces[26].x = BOARD_X_MAX - 1, pieces[26].y = BOARD_Y_MIN + 2;

	for (int i = 11; i < 16; i++)	//PIECE_PAWN
	{
		pieces[i].x = BOARD_X_MIN + (i - 11) * 2, pieces[i].y = BOARD_Y_MAX - 4;
		pieces[i + 16].x = BOARD_X_MIN + (i - 11) * 2, pieces[i + 16].y = BOARD_Y_MIN + 4;
	}

	for (int i = 0; i < 32; i++)
	{
		pieces[i].posx = map_line_x[pieces[i].x];
		pieces[i].posy = map_line_y[pieces[i].y];
	}
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
	
	piece_rect_set.clear();
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
				piece_rect_set[j] = rect;
				break;
			}
		}
	}
	for (auto& j : piece_names)
	{
		if (piece_rect_set.find(j) == piece_rect_set.end())
		{
			debugger_main.writelog(DWARNNING, "info not complete in CChessUI::UIRender::LoadPiecesAtlasInfo() " + string(j), __LINE__);
			return 0;
		}
	}
	return 1;
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
