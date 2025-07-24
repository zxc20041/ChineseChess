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
		PIECE_EAT_MOVING_TO,
		PIECE_DIED
	};
	struct CChessMap
	{
		ChessPieceType board[9][10];
		bool piece_side[9][10];
	};
	constexpr int BOARD_Y_MAX = 9, BOARD_X_MAX = 8, BOARD_Y_MIN = 0, BOARD_X_MIN = 0;
	constexpr static int SIDE_BLACK = 0;
	constexpr static int SIDE_RED = 1;
	constexpr static const char* piece_names[14] = { "black_mandarin","black_king","red_king","black_pawn","red_horse","black_elephant",
	"black_horse","red_pawn","red_elephant","black_rook","red_cannon","red_rook","red_mandarin","black_cannon" };
}

