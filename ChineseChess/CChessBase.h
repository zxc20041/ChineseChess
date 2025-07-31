#pragma once

namespace CChessBase
{
	enum ChessPieceType
	{
		PIECE_NULL,
		PIECE_PAWN,
		PIECE_ROOK,
		PIECE_HORSE,
		PIECE_ELEPHANT,
		PIECE_CANNON,
		PIECE_MANDARIN,
		PIECE_KING
	};
	enum Piece_Move_Status
	{
		PIECE_STATIC,
		PIECE_UP_MOVING,
		PIECE_HANG,
		PIECE_DOWN_MOVING,
		PIECE_MOVING_TO,
		//PIECE_EAT_MOVING_TO,
		PIECE_DIED
	};

	/*               BLACK
	  +---+---+---+---+---+---+---+---+---+
		| r | n | b | a | k | a | b | n | r | 9
		+---+---+---+---+---+---+---+---+---+
		|   |   |   |   |   |   |   |   |   | 8
		+---+---+---+---+---+---+---+---+---+
		|   | c |   |   |   |   |   | c |   | 7
		+---+---+---+---+---+---+---+---+---+
		| p |   | p |   | p |   | p |   | p | 6
		+---+---+---+---+---+---+---+---+---+
		|   |   |   |   |   |   |   |   |   | 5
		+---+---+---+---+---+---+---+---+---+
		|   |   |   |   |   |   |   |   |   | 4
		+---+---+---+---+---+---+---+---+---+
		| P |   | P |   | P |   | P |   | P | 3
		+---+---+---+---+---+---+---+---+---+
		|   | C |   |   |   |   |   | C |   | 2
		+---+---+---+---+---+---+---+---+---+
		|   |   |   |   |   |   |   |   |   | 1
		+---+---+---+---+---+---+---+---+---+
		| R | N | B | A | K | A | B | N | R | 0
		+---+---+---+---+---+---+---+---+---+
		  a   b   c   d   e   f   g   h   i         
		                   RED*/
	struct CChessMap
	{
		ChessPieceType board[9][10];
		bool piece_side[9][10];
	};
	
	struct CChessMapAndPiece
	{
		int selected_x, selected_y;
		CChessMap map;
	};
	struct PiecePosDesc
	{
		PiecePosDesc() { x = 0, y = 0; }
		PiecePosDesc(int _x,int _y) { x = _x, y = _y; }
		int x, y;
	};
	struct PieceMoveDesc
	{
		int fromx, fromy, tox, toy;
	};
	constexpr int BOARD_Y_MAX = 9, BOARD_X_MAX = 8, BOARD_Y_MIN = 0, BOARD_X_MIN = 0;
	constexpr static int SIDE_BLACK = 0;
	constexpr static int SIDE_RED = 1;
	constexpr static const char* piece_names[14] = { "black_mandarin","black_king","red_king","black_pawn","red_horse","black_elephant",
	"black_horse","red_pawn","red_elephant","black_rook","red_cannon","red_rook","red_mandarin","black_cannon" };
}

