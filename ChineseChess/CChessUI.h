#pragma once
#include"framework_base.h"
#include"CChessBase.h"

//using namespace CChessBase;
class CChessUI
{
public:
	CChessUI();
	~CChessUI();

	void ClickAt(int line, int column);

	void Rend();
private:
	constexpr static int SIDE_BLACK = 0;
	constexpr static int SIDE_RED = 1;
	CChessBase::CChessMap map;
	

	class UIRender
	{
	public:
		UIRender();
		~UIRender();

		void UpdateAll();
		void RendPieces();
		void RendBG();

		void SelectPiece(int x, int y);
		void MoveTo(int x, int y, bool eat);
		void Eat(int x, int y);
	private:
		constexpr static int PIECE_NUM_MAX = 32;
		enum Piece_Move_Status
		{
			PIECE_STATIC = 0,
			PIECE_UP_MOVING = 1,
			PIECE_HANG = 2,
			PIECE_DOWN_MOVING = 3,
			PIECE_MOVING_TO = 4,
			PIECE_EAT_MOVING_TO = 5
		};
		struct PIECE_UI
		{
			int x, y;
			float posx, posy;
			float moving_time;
			bool side_red;
			Piece_Move_Status status;
		};
		float timeScale;
		PIECE_UI pieces[PIECE_NUM_MAX];

		bool side_red;
		bool piece_selected;
		int selected_piece_x, selected_piece_y;

		float map_area_posx1, map_area_posx2, map_area_posy1, map_area_posy2;
		float block_length_x, block_length_y,mark_lengthx_short, mark_lengthx_long, mark_lengthy_short, mark_lengthy_long;

		float map_line_x[9], map_line_y[10];

		D2D1_RECT_F board_rect, river_rect,map_rect, map_rect_extent;

	};

	UIRender render;
};