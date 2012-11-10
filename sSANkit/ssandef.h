/**
 *	@file
 *	Contains type definitions and utility function definitions for sSAN.
 *
 *	@author Santhosbaala RS
 *	@copyright 2012 64cloud
 */

#ifndef ssandef_h
#define ssandef_h

#include "chesspiece.h"
#include "ChessSquare.h"
#include "bool.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 *	Chess Castling type, 4-bits.
 */
enum {
	sskCastlingTypeNone		=	(0x0),	// 0000
	sskCastlingTypeWKSide	=	(0x1),	// 0001
	sskCastlingTypeWQSide	=	(0x2),	// 0010
	sskCastlingTypeBKSide	=	(0x4),	// 0100
	sskCastlingTypeBQSide 	=	(0x8),	// 1000
};
typedef unsigned short sskCastlingType; /** 4-bit type to represent castling type */


/**
 *	Move Information Type. Solely for the purpose of informing
 *	the semantic analyzer what information was extracted from
 *	the move into the fromSquare variable of Move token. This is
 *	done so as to resolve ambiguity when two or more pieces can
 *	reach the same square. The variable may also be used when sSAN
 *	is being reconstructed.
 */
enum {
	sskChessSquareInfoNone,
	sskChessSquareInfoRankOnly,
	sskChessSquareInfoFileOnly,
	sskChessSquareInfoFileAndRank
};
typedef unsigned short sskChessSquareInfo;

/**
 *  Enum to represent the king's status.
 */
enum {
    sskKingStatusNone = 0,				/** King's status is normal */
	sskKingStatusCalculatedNone = 1,	/** Calculated to be none, so that the status is not calculated again */
    sskKingStatusCheck = 2,				/** King is under check */
    sskKingStatusCheckMate = 3,			/** King has been checkmated */
    sskKingStatusStalemate = 4			/** King has been stalemated */
};
typedef unsigned short sskKingStatus;

/**
 *	Structure to represent a single move token. The node is part of a doubly linked
 *	list, with a start position.
 */
typedef struct _sskMove {
	// Actual move variables
	unsigned int		halfmove;		/** Starts from 0, or as specified */
	sskChessPiece 		pieceMoved;		/**	4 bits. if 0, consider as NULL move. */
	sskChessSquare		fromSquare;		/** Square from which the piece moved. */
	sskChessSquare		toSquare;		/** Square to which the piece moved. */

	// Info for the semantic analyzer (ambiguity resolution)
	sskChessSquareInfo	fromSquareExtracted;	/** The info that was extracted */
	
	// Query variables
	sskChessPiece 		promotedPiece;	/** 3-bits. On promotion, set to Q/R/B/N */
	sskChessPiece 		capturedPiece;	/** 3-bits. On capture, set to piece type */
	sskCastlingType		castlingType;	/** 4-bits. Set any 1 bit. */
	sskKingStatus		selfKingStatus;	/** The status of same side King before this move */
	sskKingStatus		opponentKingStatus; /** The status of the opponent's King after this move. */
    kBool				didUpdateSelfKingStatus;    /** Indicates whether the self kingStatus was already computed */
    kBool				didUpdateOpponentKingStatus;    /** Indicates whether the opponent kingStatus was already computed */
    
	// Status Variables (Same as xFEN) at the instant before the move.
	char				piecePlacementBeforeMove[65];
	/** 
	 *	Null terminated string with piece symbols as in FEN, but square to index mapped.
	 *	a1=0, h8=63 and piecePlacement[64] = '\0'. '1' indicates empty square.
	 *	FEN symbols kqrbnp and KQRBNP are used to represent pieces.
	 */
	sskChessSquare		enPassantTarget;	/** If an enpassant is possible, set to that square */
	unsigned short		pawnHalfMoves;		/** 50-move draw rule pawn half moves */
	char				castlingStatus[5];	/** 4-chars, indicates 'HAha'(xFEN files) or '-' */
	
	// Status Variables at the instant after the move
	char				piecePlacementAfterMove[65];
	/**
	 *	Null terminated string with piece symbols as in FEN, but square to index mapped.
	 *	a1=0, h8=63 and piecePlacement[64] = '\0'. '1' indicates empty square.
	 *	FEN symbols kqrbnp and KQRBNP are used to represent pieces.
	 */

	struct _sskMove *		next; /** Pointer to the next move */
	struct _sskMove *		prev; /** Pointer to the previous move */
} sskMove;

typedef struct _sskMove * sskMoveList;	/** Move token acting a head of a move list. */

/** 
 *	Utility function for creating a blank move node. Memory deallocation is
 *	the responsibility of the caller.
 *
 *	@return A pointer to the new created Move node or NULL on failure.
 */
sskMove * sskCreateBlankMove();

/**
 *	Utility function for appending a move node to a list. If the supplied list
 *	is NULL or empty, the supplied move node is unchanged and assumed to be the
 *	new head of the list by assigning it to the list.
 *
 *	@param list Address of the head node of the move list.
 *	@param move The move node to append to the list, should not be null.
 *
 *	@return kTrue on success, kFalse on failure or if supplied move is NULL.
 */
kBool sskAppendMove(sskMoveList * list, sskMove ** move);

/**
 *	Utility function to deallocate the given move list.
 *
 *	@param list The move list to deallocate.
 */
void sskFreeMoveList(sskMoveList * list);

/**
 *	Utility function to deallocate the given move.
 *
 *	@param move The move to deallocate.
 */
void sskFreeMove(sskMove ** move);

/**
 *	Utility function to print a move node.
 *
 *	@param m The move node to print.
 */
void sskPrintMove(sskMove m);

/**
 *	Utility function to print the given piecePlacement string
 *
 *	@param piecePlacement The null-terminated piecePlacement string to print.
 */
void sskPrintPiecePlacement(char piecePlacement[64]);

#endif