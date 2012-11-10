/**
 *	@file
 *	Definition of the chess piece data type and declartions of the associated utility functions.
 *
 *	@author Santhosbaala RS
 *	@copyright 2012 64cloud
 *	@version 0.1
 */
#ifndef sSANkit_piece_h
#define sSANkit_piece_h

#include "chesscolor.h"

/**
 *	Color classified chess pieces. The type of pieces are identifed
 *	by 4-bits. MSB indicates color and the rest 3 bits indicate 
 *	the piece type. White pieces have MSB = 0, Black pieces have MSB = 1.
 */
enum {
	sskChessPieceNone		=	(0x0),	/** None = 0000 */
     
	sskChessPieceWPawn		=	(0x1),	/** White Pawn		0001 */
	sskChessPieceWKing		=	(0x2),	/** White King		0010 */
	sskChessPieceWQueen     =	(0x3),	/** White Queen		0011 */
	sskChessPieceWRook		=	(0x4),	/** White Rook		0100 */
	sskChessPieceWBishop	=	(0x5),	/** White Bishop	0101 */
	sskChessPieceWKnight	=	(0x6),	/** White Knight	0110 */
	
	sskChessPieceBPawn		=	(0x9),	/** Black Pawn		1001 */
	sskChessPieceBKing		=	(0xA),	/** Black King		1010 */
	sskChessPieceBQueen     =	(0xB),	/** Black Queen		1011 */
	sskChessPieceBRook		=	(0xC),	/** Black Rook		1100 */
	sskChessPieceBBishop	=	(0xD),	/** Black Bishop	1101 */
	sskChessPieceBKnight	=	(0xE)	/** Black Knight	1110 */
};

/** 
 *	Generic Chess Pieces, 3 bits - without MSB.
 *	Note that they have the same values as White pieces with color (MSB).
 */
enum {
	sskChessPiecePawn 		= (1),	/** Pawn	001 */
	sskChessPieceKing 		= (2),	/** King	010 */
	sskChessPieceQueen 	= (3),	/** Queen	011 */
	sskChessPieceRook 		= (4),	/** Rook	100 */
	sskChessPieceBishop 	= (5),	/** Bishop	101 */
	sskChessPieceKnight 	= (6)	/** Knight	110 */
};

typedef unsigned short sskChessPiece;	/** Color classified chess piece type (4-bits). */

/**
 *	Utility function returns the generic 3-bit piece code for the given SAN symbol in english.
 *	@link http://en.wikipedia.org/wiki/Algebraic_notation_(chess)#Naming_the_pieces
 *
 *	@param s symbol of the piece ' ', 'K', 'Q', 'R', 'B', 'N'
 *	@return 3 bit piece code for the given symbol or 0 if invalid.
 */
sskChessPiece sskSANsymbolToPieceCode(char aSANsymbol);

/**
 *	Utility function returns FEN symbol from the given piece code.
 *	@link http://en.wikipedia.org/wiki/X-FEN
 *
 *	@param piece The 4-bit piece code.
 *	@return xFEN Symbol for the given piece code.
 */
char sskPieceCodeToxFENSymbol(sskChessPiece aPieceCode);

/** Get the color of a piece, uses (LSB-3)rd bit */
#define SSK_GET_PIECE_COLOR(pieceCode) (((pieceCode) >> 3) & 1)
/** Get the type of a piece without color, uses last 3 bits */
#define SSK_GET_GENERIC_PIECE_CODE(pieceCode) ((pieceCode) & 7)

#endif
