#pragma once
#include"..\framework_base.h"
#include"CChessBase.h"
#include"CChessEngineCore.h"

using namespace CChessBase;

struct PIECE_ATLAS_INFO
{
	D2D1_RECT_F static_rect;
	D2D1_RECT_F up_rect;
	D2D1_RECT_F down_rect;
};


class PIECE_UI
{
public:
	PIECE_UI(int x,int y, ChessPieceType type,bool side_red);
	void rend(bool board_side_red);
	void update(float timeScale);
	void Select();
	void UnSelect();
	void MoveTo(PieceMoveDesc move);
	void Die();
	bool MatchPosition(int x, int y);
	static unordered_map<string, PIECE_ATLAS_INFO> piece_rect_set;
	static D2D1_RECT_F piece_rect[BOARD_X_MAX + 1][BOARD_Y_MAX + 1];
	static float map_line_x[BOARD_X_MAX + 1], map_line_y[BOARD_Y_MAX + 1];
private:
	int x, y;
	float posx, posy;
	float moving_time;
	bool side_red;
	Piece_Move_Status status;
	ChessPieceType type;

	D2D1_RECT_F static_rect, up_rect, down_rect;
	PieceMoveDesc currentMove;
};

class CChessUI
{
public:
	CChessUI();
	~CChessUI();

	void ClickAt(int line, int column);

	void Reset();
	bool LoadPiecesAtlasInfo();
	void Update();
	void Rend();
	void SetEngine(shared_ptr<CChessEngine> chessEngine);
private:
	
	CChessBase::CChessMap map;
	bool side_red;
	bool piece_selected;
	int selected_piece_x, selected_piece_y;
	std::vector<CChessBase::PiecePosDesc> availablePosition;
	unique_ptr<Box> boxes[9][10];
	shared_ptr<CChessEngine> engine;

	class UIRender
	{
	public:
		UIRender();
		~UIRender();


		void Update();
		void RendPieces();
		void RendBG();
		void RendMark();
		void Reset();
		bool LoadPiecesAtlasInfo();
		void SetSide(bool side_red);
		void SetAvailablePosition(std::vector<CChessBase::PiecePosDesc> pos);

		void SelectPiece(int x, int y);
		void UnSelectPiece(int x, int y);
		void MovePiece(PieceMoveDesc move, bool eat);
		void GameOver(bool win_side);
		void PostDraw(bool post_side);

		unique_ptr<Box>GetBox(int x, int y);
	private:
		constexpr static int PIECE_NUM_MAX = 32;
		constexpr static float MARK_RADIUS_RATIO_MAX = 0.2F;
		
		float radius_ratio;
		float timeScale;
		unique_ptr<PIECE_UI> pieces[PIECE_NUM_MAX];
		std::vector<CChessBase::PiecePosDesc> availablePosition;
		

		float map_area_posx1, map_area_posx2, map_area_posy1, map_area_posy2;
		float block_length_x, block_length_y,mark_lengthx_short, mark_lengthx_long, mark_lengthy_short, mark_lengthy_long;

		float map_line_x[BOARD_X_MAX + 1], map_line_y[BOARD_Y_MAX + 1];

		D2D1_RECT_F board_rect, river_rect, map_rect, map_rect_extent;

		bool side_red;
	};

	UIRender render;
};