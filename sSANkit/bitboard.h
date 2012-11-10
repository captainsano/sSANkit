/**
 *	@file
 *	Defines data types and declares functions for chess bitboard manipulation.
 *
 *	@author Santhosbaala RS
 *	@copyright 2012 64cloud
 *	@version 0.1
 */

#ifndef bitboard_h
#define bitboard_h

#include "ChessSquare.h"
#include "chesscolor.h"
#include "chesspiece.h"

typedef unsigned long long sskBitmap;	/** 64-bit bitmap, mapping to chess squares(LSB=a1 and MSB=h8). */

/** Convenience macro for setting something to an empty bitmap */
#define SSK_EMPTY_BITMAP ((sskBitmap)(0))

/** Convenience macro for setting something to filled bitmap */
#define SSK_FULL_BITMAP ((sskBitmap)(0xffffffffffffffff))

/**
 *	Structure declares the individual bitmaps necessary to store a complete
 *	chess position. Also it declares some conveniece bitmaps for easily
 *	obtaining occupied squares by white, black and both.
 */
typedef struct _sskBitboardPosition {
    sskBitmap wPawn;		/** bitmap stores white pawns. */
	sskBitmap wKing;		/** bitmap stores white king. */
	sskBitmap wQueen;	/** bitmap stores white queens. */
	sskBitmap wRook;		/** bitmap stores white rooks. */
	sskBitmap wBishop;	/** bitmap stores white bishops. */
	sskBitmap wKnight;	/** bitmap stores white knights. */
	
	sskBitmap bPawn;		/** bitmap stores black pawns. */
    sskBitmap bKing;		/** bitmap stores black king. */
	sskBitmap bQueen;	/** bitmap stores black queens. */
	sskBitmap bRook;		/** bitmap stores black rooks. */
	sskBitmap bBishop;	/** bitmap stores black bishops. */
	sskBitmap bKnight;	/** bitmap stores black knights. */
	
	sskBitmap wOccupied;	/** bitmap stores all squares occupied by white pieces */
	sskBitmap bOccupied;	/** bitmap stores all squares occupied by black pieces */
	sskBitmap occupied;	/** bitmap stores all squares occupied by both white and black pieces. */
	
} sskBitboardPosition;

/** Returns a bitmap with 1 at the bit corresponding to the square index (0-63) and the other bits to 0 */
#define SSK_BITMAP_SET_SQUARE_IDX(squareIndex) ((sskBitmap)0x8000000000000000 >> (63 - (squareIndex)))

/** Returns a bitmap with 0 at the bit corresponding to the square index (0-63) and the other bits to 1 */
#define SSK_BITMAP_UNSET_SQUARE_IDX(squareIndex) (~(SSK_BITMAP_SET_SQUARE_IDX(squareIndex)))

#pragma mark - Masking Functions

/**
 *	Function returns a bitmap with all the bits corresponding to given file index(0-7) set to 1.
 *
 *	@return A bitmap with the squares corresponding to the given file set to 1.
 *			Returns an empty bitmap if the given file is invalid.
 */
sskBitmap sskBitmapWithFileMask(sskChessSquareFile fileIndex);

/**
 *	Function returns a bitmap with all the bits corresponding to given rank index(0-7) set to 1.
 *
 *	@return A bitmap with the squares corresponding to the given rank set to 1.
 *			Returns an empty bitmap if the given rank is invalid.
 */
sskBitmap sskBitmapWithRankMask(sskChessSquareRank rankIndex);

#pragma mark - Bitmap Lookup(Hash) Functions

/**
 *	Function returns a bitmap with the bits corresponding to the squares a knight 
 *	can reach from the given square index.
 *
 *	@param squareIndex The square index(0-63).
 *
 *	@return A bitmap with the squares a knight can reach from the given square index.
 *			Returns an empty bitmap if the given square index is invalid.
 */
sskBitmap sskBitmapWithKnightReach(sskChessSquare squareIndex);

/**
 *	Function returns a bitmap with the bits corresponding to the squares a king
 *	can reach from the given square index. NOTE: This function does not consider
 *	castling.
 *
 *	@param squareIndex The square index(0-63).
 *
 *	@return A bitmap with the squares a king can reach from the given square index.
 *			Returns an empty bitmap if the given square index is invalid.
 */
sskBitmap sskBitmapWithKingReach(sskChessSquare squareIndex);

/**
 *	Function returns a bitmap with the bits corresponding to the squares that lie
 *	int the path of king castling. Note that this function supports Chess960.
 *
 *	@param kingColor The color of the castling King.
 *	@param castlingSide The side in which the King is about the castle(0=Kside, 1=Qside).
 *	@param kingFile The file number (1-6 (b-g)) on which the King is placed.
 *	@param rookFile The file number (0-7 (a-h)) of the participating rook.
 *
 *	@return A bitmap with the squares that lie in the path of king castling set to 1.
 *			Returns a bitmap filled with 1 on invalid conditions and an empty bitmap
 *			if the argument values are not in range.
 */
sskBitmap sskBitmapWithKingCastlePath(sskChessColor kingColor, unsigned short castlingSide, sskChessSquareFile kingFile, sskChessSquareFile rookFile);

/**
 *	Function returns a bitmap with the bits corresponding to the squares a pawn
 *	can reach from the given square index. NOTE: This function returns an empty
 *	bitmap for pawns on 1st rank for white and 8th rank for black.
 *
 *	@param squareIndex The square number(0-63)
 *	@param pawnColor The color of the pawn (0=white, 1=black)
 *
 *	@return A bitmap with the squares attacked by Pawn on the given square.
 *			Returns an empty bitmap if the given square index or condition is invalid.
 */
sskBitmap sskBitmapWithPawnReach(sskChessSquare squareIndex, sskChessColor pawnColor);

/**
 *	Function returns a bitmap with the bits corresponding to the squares in the
 *	North-East diagonal relative to the given square index set to 1.
 *
 *	@param squareIndex The square number(0-63).
 *
 *	@return A bitmap with the North-East diagonal squares relative to the given square index set to 1.
 *			Returns an empty bitmap if the given square index is invalid.
 */
sskBitmap sskBitmapWithNorthEastDiagonal(sskChessSquare squareIndex);

/**
 *	Function returns a bitmap with the bits corresponding to the squares in the
 *	North-West diagonal relative to the given square index set to 1.
 *
 *	@param squareIndex The square number(0-63).
 *
 *	@return A bitmap with the North-East diagonal squares relative to the given square index set to 1.
 *			Returns an empty bitmap if the given square index is invalid.
 */
sskBitmap sskBitmapWithNorthWestDiagonal(sskChessSquare squareIndex);

/**
 *	Function returns a bitmap with the bits corresponding to the squares in the
 *	South-East diagonal relative to the given square index set to 1.
 *
 *	@param squareIndex The square number(0-63).
 *
 *	@return A bitmap with the South-East diagonal squares relative to the given square index set to 1.
 *			Returns an empty bitmap if the given square index is invalid.
 */
sskBitmap sskBitmapWithSouthEastDiagonal(sskChessSquare squareIndex);

/**
 *	Function returns a bitmap with the bits corresponding to the squares in the
 *	South-West diagonal relative to the given square index set to 1.
 *
 *	@param squareIndex The square number(0-63).
 *
 *	@return A bitmap with the South-West diagonal squares relative to the given square index set to 1.
 *			Returns an empty bitmap if the given square index is invalid.
 */
sskBitmap sskBitmapWithSouthWestDiagonal(sskChessSquare squareIndex);

/**
 *	Function returns a bitmap with the bits corresponding to the squares in the
 *	North file relative to the given square index set to 1.
 *
 *	@param squareIndex The square number(0-63).
 *
 *	@return A bitmap with the North file squares relative to the given square index set to 1.
 *			Returns an empty bitmap if the given square index is invalid.
 */
sskBitmap sskBitmapWithNorthFile(sskChessSquare squareIndex);

/**
 *	Function returns a bitmap with the bits corresponding to the squares in the
 *	South file relative to the given square index set to 1.
 *
 *	@param squareIndex The square number(0-63).
 *
 *	@return A bitmap with the South file squares relative to the given square index set to 1.
 *			Returns an empty bitmap if the given square index is invalid.
 */
sskBitmap sskBitmapWithSouthFile(sskChessSquare squareIndex);

/**
 *	Function returns a bitmap with the bits corresponding to the squares in the
 *	East rank relative to the given square index set to 1.
 *
 *	@param squareIndex The square number(0-63).
 *
 *	@return A bitmap with the East rank squares relative to the given square index set to 1.
 *			Returns an empty bitmap if the given square index is invalid.
 */
sskBitmap sskBitmapWithEastRank(sskChessSquare squareIndex);

/**
 *	Function returns a bitmap with the bits corresponding to the squares in the
 *	West rank relative to the given square index set to 1.
 *
 *	@param squareIndex The square number(0-63).
 *
 *	@return A bitmap with the West rank squares relative to the given square index set to 1.
 *			Returns an empty bitmap if the given square index is invalid.
 */
sskBitmap sskBitmapWithWestRank(sskChessSquare squareIndex);

#pragma mark - BitScan Functions

/**
 *	Utility function to count the number of bits set to 1 in a bitmap.
 *
 *	@param bitmap The bitmap in which the number of bits is to be counted.
 *
 *	@return The number of bits set to 1.
 */
unsigned short sskCountBits(sskBitmap bitmap);

/**
 *	Utility function to obtain the index of the first bit in
 *	the given bitmap with value 1, starting from the LSB.
 *	Do not pass 0 to this function!
 *
 *	@param bitmap The bitmap in which the first set bit is to be found.
 *
 *	@return The index of the first bit set to 1, starting from LSB(0-63).
 *			Unpredictable values will be returned when 0 is passed.
 */
unsigned short sskFirstOneIndex(sskBitmap bitmap);

/**
 *	Utility function to obtain the index of the last bit in
 *	the given bitmap with value 1, starting from the LSB.
 *	Do not pass 0 to this fuction!
 *
 *	@param bitmap The bitmap in which the last set bit is to be found.
 *
 *	@return The index of the last bit set to 1, starting from LSB(0-63).
 *			Unpredictable values will be returned when 0 is passed.
 */
unsigned short sskLastOneIndex(sskBitmap bitmap);

#pragma mark - Utility Functions
/**
 *  Function returns the bitmap from a bitboard position for the given piece code.
 *  This function is exteremely useful for iterating across the members of a BitboardPosition.
 *	
 *	@param bitboardPosition The position as a bitboard.
 *	@param pieceCode The 4-bit piece code for which the bitmap is required.
 *
 *	@return The bitmap for the given piececode or an empty bitmap if piece code is invalid. Empty bitmap on invalid conditions.
 */
sskBitmap sskBitmapForPieceInBitboardPosition(sskBitboardPosition bitboardPosition, sskChessPiece pieceCode);

/**
 *	Function returns a pointer to the bitmap in the given bitboard position. This function
 *	is the same as sskBitmapForPieceInBitboardPosition() except that the bitmap can be updated.
 *
 *	@param bitboardPosition A pointer to the bitboard position structure.
 *	@param pieceCode The 4-bit piece code for which the bitmap is required.
 *
 *	@return A pointer to the bitmap for the given piece code or NULL if the piece code is invalid.  */
sskBitmap * sskUpdateableBitmapForPieceInBitboardPosition(sskBitboardPosition * bitboardPosition, sskChessPiece pieceCode);

/**
 *  Function returns the toSquare specific attack bitmap for a given piece in the given position.
 *  NOTE: This function does not assert if the rook exists on the required square in case of castling.
 *        It is soleley the caller's responsibility.
 *
 *  @param bitboardPosition The bitboard position for attack bitmap calculation.
 *  @param pieceCode The 4-bit piece code of the piece whose attack bitmap is required.
 *  @param fromSquare The fromSquare of the piece.
 *  @param toSquare The toSquare of the piece.
 *  @param isEnpassantOrCastling If castling, set the rook's file(0-7) or in case of enPassant,
 *                               set the square. In normal conditions set to -1.
 *  
 *  @return A bitmap with bits 1 set where the given piece has attacks. Empty bitmap on invalid conditions.
 */
sskBitmap sskBitmapForSpecificPieceAttacksInBitboardPosition(sskBitboardPosition bitboardPosition, sskChessPiece pieceCode, sskChessSquare fromSquare, sskChessSquare toSquare, int isCastlingOrEnpassantTarget);

/**
 *  Function returns the attack bitmap with all reachable squares of the given piece in the given bitboard position.
 *  NOTE: - This function does not consider castling, enpassant moves or pins. Function considers blockers.
 *        - In case the piece does not exist in the given fromSquare, the funciton returns an empty bitboard.
 *
 *  @param bitboardPosition The bitboard position for attack bitmap calculation.
 *  @param pieceCode The 4-bit piece code of the piece whose attack bitmap is required.
 *  @param fromSquare The fromSquare of the piece.
 *
 *  @return A bitmap with 1 set wherever the piece could move. Empty bitmap on invalid conditions.
 */
sskBitmap sskBitmapForAllPieceAttacksInBitboardPosition(sskBitboardPosition bitboardPosition, sskChessPiece pieceCode, sskChessSquare fromSquare);

/**
 *	Utilty function to extract the piece placement information from the given
 *	xFEN string and convert it to a Bitboard position.
 *
 *	X-FEN is an extension of FEN for Chess960 support by Reinhard Scharnagl.
 *	http://en.wikipedia.org/wiki/X-FEN
 *
 *	@param xFENstring The xFEN string with piece placement.
 *
 *	@return A complete bitboard structure for the given xFEN.
 */
sskBitboardPosition * sskxFEN2BitboardPosition(const char * xFENstring);

/**
 *  Utility function to copy the bitboard.
 *
 *  @param bitboardPosition The bitboard position to copy.
 *
 *  @return A pointer to the copy of the given bitboard or NULL if failed to copy.
 */
sskBitboardPosition * sskCopyBitboardPosition(sskBitboardPosition bitboardPosition);

/**
 *	Utility function to clear all the bitmaps in the given bitboard.
 *
 *	@param bitboardPosition The position whose individual bitboards need to be cleared.
 */
void sskClearBitboardPosition(sskBitboardPosition * bitboardPosition);

/**
 *	Utility function for printing the given bitmap(order rank 8->1).
 *
 *	@param bitmap The bitmap to print.
 */
void sskPrintBitmap(sskBitmap bitmap);

/**
 *	Utility function for printing the given position in xFEN format.
 *
 *	@param bitboardPosition The position to print.
 */
void sskPrintBitboardPosition(sskBitboardPosition bitboardPosition);

#endif
