/**
 *	@file
 *	Defines data types and functions for offset(array) board manipulation.
 *
 *	@author Santhosbaala RS
 *	@copyright 2012 64cloud
 *	@version 1.0
 */

#ifndef offsetboard_h
#define offsetboard_h

#include "chesspiece.h"

typedef sskChessPiece * sskOffsetPosition; /** Represents an array of chess pieces as an offset position with 64 pieces(a1-h8). */

/**
 *	Utilty function to extract the piece placement information from the given
 *	xFEN string and convert it into an offset position. Memory deallocation is 
 *	the responsbility of the caller.
 *
 *	X-FEN is an extension of FEN for Chess960 support by Reinhard Scharnagl.
 *	http://en.wikipedia.org/wiki/X-FEN
 *
 *	@param xFENString The xFEN string with piece placement.
 *
 *	@return A pointer to the offset position or NULL on failure.
 */
sskOffsetPosition sskxFENtoOffsetPosition(char * xFENstring);

/**
 *	Utility function to create a copy of the given offset position. Memory
 *	deallocation is the responsbility of the caller.
 *
 *	@param offsetPosition The offset position to copy.
 *
 *	@return A pointer to the copy of the given offset position.
 */
sskOffsetPosition sskCopyOffsetPosition(sskOffsetPosition offsetPosition);

/**
 *	Utility function for printing the given offset board(order 8-1 ranks).
 *
 *	@param offsetPosition The offset position to print.
 */
void sskPrintOffsetPosition(sskOffsetPosition offsetPosition);

#endif
