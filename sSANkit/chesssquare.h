/**
 *	@file
 *	Definition of data types to represent a square in chess and declaration of the associated utility functions.
 *
 *	@author Santhosbaala RS
 *	@copyright 2012 64cloud
 *	@version 0.1
 */

#ifndef sSANkit_square_h
#define sSANkit_square_h

/**
 *	A square in chess board, indexed with 6 bits (0-63). 
 *	The first 3 bits indicate the rank(0-7) and the last 3 bits
 *	indicate the file(0-7). With the rank and file values
 *	together, the value of a chess square can be from 0 to 63.
 *	a1 = 0 all the way upto h8 = 63.
 *	Example: Square = (RANK)(FILE) (rrr)(fff) (3-bits)(3-bits)
 */
typedef unsigned short sskChessSquare;

/**
 *  A rank on a chessboard(3-bits, 0-7).
 */
typedef sskChessSquare sskChessSquareFile;

/**
 *  A rank on a chessboard(3-bits, 0-7).
 */
typedef sskChessSquare sskChessSquareRank;

/** Convert a file character(a-h) to the file index(0-7). */
#define SSK_CHAR_2_FILE(fileChar) ((int)(fileChar) - (int)'a')

/** Convert a file character(1-8) to the rank index(0-7). */
#define SSK_CHAR_2_RANK(rankChar) ((int)(rankChar) - (int)'1')

/** Obtain the file index(0-7) of the given square index(0-63). */
#define SSK_GET_FILE_IDX(squareIndex) ((squareIndex) & 7)

/** Obtain the file index(0-7) of the given square index(0-63). */
#define SSK_GET_RANK_IDX(squareIndex) (((squareIndex) >> 3) & 7)

/** Obtain the 6-bit square index with 3-bits set to the given file index. */
#define SSK_SQUARE_IDX_FOR_FILE_IDX(fileIndex) ((fileIndex) & 7)

/** Obtain the 6-bit square index with 3-bits set to the given file index. */
#define SSK_SQUARE_IDX_FOR_RANK_IDX(rankIndex) (((rankIndex) << 3) & 56)

/** Obtain the 6-bit square index from the given file and rank indeces respectively. */
#define SSK_SQUARE_IDX_FOR_FILE_RANK_IDX(fileIndex, rankIndex) (SSK_SQUARE_IDX_FOR_RANK_IDX(rankIndex) | SSK_SQUARE_IDX_FOR_FILE_IDX(fileIndex)	)

/** Obtain the 6-bit square index with 3-bits set from the given file character(a-h). */
#define SSK_SQUARE_IDX_FOR_FILE_CHAR(fileChar) SSK_SQUARE_IDX_FOR_FILE_IDX(((fileChar) - 'a'))
/** Obtain the 6-bit square index with 3-bits set from the given file character(1-8). */
#define SSK_SQUARE_IDX_FOR_RANK_CHAR(rankChar) SSK_SQUARE_IDX_FOR_RANK_IDX(((rankChar) - '1'))
/** Obtain the 6-bit square index from the given file and rank characters respectively. */
#define SSK_SQUARE_IDX_FOR_FILE_RANK_CHAR(fileChar, rankChar) (SSK_SQUARE_IDX_FOR_RANK_IDX(((rankChar) - '1')) | SSK_SQUARE_IDX_FOR_FILE_IDX(((fileChar) - 'a')))

/**
 *	Utility function that returns the string square label in SAN format
 *	for the given square index. Memory deallocation is the caller's responsibility.
 *
 *	@param square The square index(0-63).
 *	@return Null terminated string with the square label(length=2). Returns NULL on invalid index.
 */
char * sskSquareToLabel(sskChessSquare aSquareIndex);
  

#endif
