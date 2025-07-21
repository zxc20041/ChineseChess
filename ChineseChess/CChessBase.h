#pragma once

namespace CChessBase
{
	enum ChessPiece
	{
		PIECE_NULL = 0,
		PIECE_PAWN = 1,
		PIECE_ROOK = 2,
		PIECE_HORSE = 3,
		PIECE_ELEPHANT = 4,
		PIECE_CANNON = 5,
		PIECE_MANDARIN = 6,
		PIECE_KING = 7
	};
	struct CChessMap
	{
		ChessPiece board[9][10];
		bool piece_side[9][10];
	};
}

