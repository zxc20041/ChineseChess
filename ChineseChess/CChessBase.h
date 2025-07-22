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
	struct CChessMap
	{
		ChessPieceType board[9][10];
		bool piece_side[9][10];
	};
	constexpr int BOARD_Y_MAX = 9, BOARD_X_MAX = 8, BOARD_Y_MIN = 0, BOARD_X_MIN = 0;
	constexpr static int SIDE_BLACK = 0;
	constexpr static int SIDE_RED = 1;
}

