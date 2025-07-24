#pragma once
#include"framework_base.h"
#include"CChessBase.h"

using namespace CChessBase;

//todo: add PIECE_UI class
class PIECE_UI
{
public:
	int x, y;
	float posx, posy;
	float moving_time;
	bool side_red;
	Piece_Move_Status status;
	ChessPieceType type;
};

class CChessUI
{
public:
	CChessUI();
	~CChessUI();

	void ClickAt(int line, int column);

	void Reset();
	bool LoadPiecesAtlasInfo();

	void Rend();
private:
	
	
	CChessBase::CChessMap map;
	

	class UIRender
	{
	public:
		UIRender();
		~UIRender();

		void UpdateAll();
		void RendStaticPiece(PIECE_UI& piece);
		void RendPieces();
		void RendBG();

		void Reset();
		bool LoadPiecesAtlasInfo();

		void SelectPiece(int x, int y);
		void MoveTo(int x, int y, bool eat);
		void Eat(int x, int y);
	private:
		constexpr static int PIECE_NUM_MAX = 32;
		

		struct PIECE_ATLAS_INFO
		{
			D2D1_RECT_F static_rect;
			D2D1_RECT_F up_rect;
			D2D1_RECT_F down_rect;
		};
		float timeScale;
		PIECE_UI pieces[PIECE_NUM_MAX];

		bool side_red;
		bool piece_selected;
		int selected_piece_x, selected_piece_y;

		float map_area_posx1, map_area_posx2, map_area_posy1, map_area_posy2;
		float block_length_x, block_length_y,mark_lengthx_short, mark_lengthx_long, mark_lengthy_short, mark_lengthy_long;

		float map_line_x[BOARD_X_MAX + 1], map_line_y[BOARD_Y_MAX + 1];

		D2D1_RECT_F board_rect, river_rect, map_rect, map_rect_extent, piece_rect[BOARD_X_MAX + 1][BOARD_Y_MAX + 1];

		unordered_map<string, PIECE_ATLAS_INFO> piece_rect_set;
	};

	UIRender render;
};