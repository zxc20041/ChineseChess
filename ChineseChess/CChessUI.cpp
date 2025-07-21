#include"CChessUI.h"

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

void CChessUI::Rend()
{
	render.RendBG();
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
}

CChessUI::UIRender::~UIRender()
{
}

void CChessUI::UIRender::UpdateAll()
{
}

void CChessUI::UIRender::RendPieces()
{
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

void CChessUI::UIRender::SelectPiece(int x, int y)
{
}

void CChessUI::UIRender::MoveTo(int x, int y, bool eat)
{
}

void CChessUI::UIRender::Eat(int x, int y)
{
}
