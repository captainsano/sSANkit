/**
 *	@file
 *	Contains function declarations for Semantic Analysis of a move list and associated utility functions.
 *
 *	@author Santhosbaala RS
 *	@copyright 2012 64cloud
 *	@version 0.1
 */

#ifndef SEMANTIC_ANALYZER
#define SEMANTIC_ANALYZER

#include "ssandef.h"
#include "bitboard.h"
#include "offsetboard.h"
#include "boardformatconvertutil.h"

#pragma mark - Semantic analysis and legality verification functions

/**
 *	Enum defines the error codes for the semantic_analyze() function.
 */
enum {
	sskSemanticAnalyzerErrorNone = 0,					/** No error */
	sskSemanticAnalyzerErrorProvidedMoveListEmpty,		/** The given move list was empty */
	sskSemanticAnalyzerErrorFirstPositionNotSpecified,	/** The initial position was not supplied */
	sskSemanticAnalyzerErrorIllegalMove,				/** An illegal move was found */
	sskSemanticAnalyzerErrorAmbiguousMove,				/** An ambiguos move was found */
	sskSemanticAnalyzerErrorMovesExistAfterGameEnd		/** Moves exist even if the game had ended */
};
typedef unsigned short sskSemanticAnalyzerError;		/** Custom typedef for semantic analysis error code */

/**
 *	Function analyses the given move list for semantic correctness 
 *	and completes the fromSquare-toSquare pair. If the ambiguity
 *	was encountered, then the ambiguosHalfmoveNumber iVar is filled.
 *
 *	@param moveList The input move list.
 *	@param startingPosition The starting position, specified as an xFEN string.
 *	@param ambiguousHalfmoveNumber Out parameter, filled if a move was found ambigous. (optional, can be NULL)
 *
 *	@return Returns the error code:
 *		0 - No error.
 *		1 - Provided Move list was empty.
 *		2 - Position not specified for first move.
 *		3 - Illegal move.
 *		4 - Piece movement ambiguity.
 *		5 - Movelist continues after logical game end.
 */
sskSemanticAnalyzerError sskSemanticAnalyze(sskMoveList moveList, char * startingPosition, int * ambiguousHalfmoveNumber);

/**
 *	Function verifies if the move is pseudo legal and returns
 *	the fromSquare variable of the given move. Additionaly it fills
 *	fromSquare and other information it discovers during analysis.
 *
 *	@param bitboardPosition The current position in bitboard format.
 *	@param offsetPosition The current position in offset format.
 *	@param move A pointer to the move to be checked.
 *	@param ambiguity Out variable filled with kTrue if ambiguity was found.
 *
 *	@return kTrue on success, kFalse on failure.
 */
kBool sskFillFromSquare(sskBitboardPosition bitboardPosition, sskOffsetPosition offsetPosition, sskMove * move, kBool * ambiguity);

/**
 *  Function verifies if the move is completely legal, i.e) whether the move puts
 *  the king under check. Note that the given move should be complete in the sense that
 *  the fromSquare and toSquare fields should be filled. If the move was not checked for
 *	pseudoLegality, then set the param to kFalse. The caller has to check if the move
 *	is a castle and must set the appropriate field in the move token
 *
 *  @param bitboardPosition The current position in Bitboard format.
 *  @param move A pointer to the move to be verified.
 *	@param moveWasPseudoLegalChecked Set to true if the move's pseudo legallity was checked beforehand.
 *
 *  @return Returns the updated bitboard position if legal or NULL.
 */
sskBitboardPosition * sskCheckLegal(sskBitboardPosition bitboardPosition, sskMove * move, kBool moveWasPseudoLegalChecked);

#pragma mark - Piece and board status query function

/**
 *  Function verifies if a side's king is under check for the given position.
 * 
 *  @param bitboardPosition The current position in bitboard format.
 *  @param kingColor The color of the king that needs to be checked.
 *  @param shouldIncludeKing If set to TRUE, will also consider opponent king issuing check.
 *	@param checkingPieceSquare Out param, can be NULL. Filled with the square of the piece 
 *							   issuing check. In case of multiple pieces, the last piece is filled.
 *
 *  @return The number of pieces putting the king under check or 0 for no check.
 */
unsigned short sskIsKingUnderCheck(sskBitboardPosition bitboardPosition, sskChessColor kingColor, kBool shouldIncludeKing, sskChessSquare * checkingPieceSquare);

/**
 *	Function verifies if a side's king is under checkmate for the given position.
 *	Note that isKingUnderCheck() should be called before calling this function.
 *
 *	@param bitboardPosition The current position in bitboard format.
 *	@param kingColor The color of the king that needs to be checked.
 *  @param numChecks Set this > 0 if the number of checks is known, else set to -1
 *	@param enpassantTarget If any enpassant target is there for kingColor. Set to -1 if inapplicable.
 *
 *	@return kTrue if checkmate else kFalse.
 *
 *	NOTE: We need an enpassant target to handle a rare situation where in a pawn is
 *		  double pushed to issue check. Example: 7k/5p1b/7q/4P3/6K1/r7/8/8. Black
 *		  can play f5+, wKing has no escape squares, but ef6 is possible!
 */
kBool sskIsKingUnderCheckMate(sskBitboardPosition bitboardPosition, sskChessColor kingColor, int numChecks, int enpassantTarget);

/**
 *	Function verifies if a side's king is under stalemate for the given position.
 *	note that checkPseudoLegal() and isKingUnderCheck() should have been called
 *	before calling this function.
 *
 *	@param bitboardPosition The current position in bitboard format.
 *	@param kingColor The color of the king that needs to be checked.
 *	@param enpassantTarget If any enpassant target is there for kingColor. Set to -1 if inapplicable.
 *
 *	@return kTrue if stalemate else kFalse.
 */
kBool sskIsKingUnderStalemate(sskBitboardPosition bitboardPosition, sskChessColor kingColor, int enpassantTarget);

/**
 *	Function checks if king can move to adjacent squares without getting into check.
 *	
 *	@param bitboardPosition The current position in bitboard format.
 *	@param kingColor The color of the king that needs to be checked.
 *
 *	@return kTrue if king can escape and kFalse if king does not have any escape square.
 */
kBool sskCanKingEscape(sskBitboardPosition bitboardPosition, sskChessColor kingColor);

/**
 *	Function verifies whether a piece on a square can make A->B move in the given position. (Reachability)
 *	The method supports enpassant and castling verification as well. Additionaly castling is generic for
 *	normal chess as well as chess960.
 *
 *	@param bitboardPosition The position in Bitboard format.
 *	@param offsetPosition The position in Offset format.
 *	@param fromSquare Source square.
 *	@param toSquare Destination square.
 *	@param isCastlingOrEnpassantTarget If the moving piece is a king and the argument is set to 0-7, indicating the rook's file. The function shall
 *									   consider the move as castling. If the moving piece is a pawn and the argument is set to a square, the function
 *									   shall consider enpassant. In normal cases, set this argument to -1.
 *
 *	@return kTrue if the piece can travel fromSquare to toSquare, else kFalse.
 */
kBool sskIsSquareReachable(sskBitboardPosition bitboardPosition, sskChessSquare fromSquare, sskChessSquare toSquare, int isCastlingOrEnpassantTarget, sskBitmap * attackMap);

/**
 *	Function verifies if a particular piece under pin for the blocked piece.
 *
 *	@param bitboardPosition The position in Bitboard format.
 *	@param offsetPosition The position in Offset format.
 *	@param pinnedSquare	The square on which the probably pinned piece is sitting.
 *	@param behindSquare The square to which the pin x-ray is radiated.
 *	@param color The color of the pinned piece.
 *	@param pinCausingPieceSquare If the function returns kTrue, filled with the square number of the pinner's square.
 *
 *	@return kTrue if the given piece is under pin, kFalse if not.
 */
kBool sskIsSquarePinned(sskBitboardPosition bitboardPosition, sskOffsetPosition offsetPosition, sskChessSquare pinnedSquare, sskChessSquare behindSquare, sskChessColor color, sskChessSquare * pinCausingPieceSquare);

#pragma mark - Move ambiguity handling functions

/**
 *	Enum to mark the common attributes among two or more squares.
 *	The enum values can be bitwise ORed if the squares have both
 *	file and rank in common.
 */
enum {
	sskSquareCommonalityNone = 0,	/** Nothing is common between the squares */
	sskSquareCommonalityRank = 1,	/** Squares have rank in common */
	sskSquareCommonalityFile = 2,	/** Squares have file in common */
};

typedef unsigned short sskSquareCommonality;	/** Custom type to represent square commonality */

/**
 *	Function checks the squares of the given set of squares and finds out the commonalities
 *	in them, like same file, same rank, etc., when the feature is shared by atleast two squares
 *	among the provided set.
 *
 */
sskSquareCommonality sskGetSquareCommonality(sskChessSquare squares[], int numSquares);

/**
 *	Function checks if the given move token is ambiguous. Additionaly, the function fills the given
 *	move token with the computed fromSquare value if the move was not ambiguos.
 *
 *	@param offsetPosition The current position in offset format for lookup.
 *	@param reachablePiecesFromSquares An array of calculatedFromSquare.
 *	@param numReachablePieces The number of reachabe pieces found during semantic analysis. Should be > 0.
 *	@param move	The move token representing the move.
 *	@param Out parameter filled with the attack map calculated, can be NULL.
 *
 *	@return kTrue if ambiguous, kFalse if not ambiguous.
 */
kBool sskCheckMoveAmbiguityAndFillFromSquare(sskOffsetPosition offsetPosition, sskChessSquare reachablePiecesFromSquares[], unsigned short numReachablePieces, sskMove * move);

#endif
