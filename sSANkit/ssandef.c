/**
 *	@file
 *	Contains implementation of functions defined in the corresponding header file.
 *
 *	@author Santhosbaala RS
 *	@copyright 2012 64cloud
 */

#include "ssandef.h"

sskMove * sskCreateBlankMove() {
	sskMove *m = NULL;
	m = malloc(sizeof(sskMove));
	
	if (m == NULL) return NULL;
	
	m->halfmove = 0;
	m->pieceMoved = 0;
	m->fromSquare = 0;
	m->toSquare = 0;
	m->fromSquareExtracted = sskChessSquareInfoNone;
	m->promotedPiece = sskChessPieceNone;
	m->capturedPiece = 0;
	m->castlingType = sskCastlingTypeNone;
	m->selfKingStatus = sskKingStatusNone;
	m->opponentKingStatus = sskKingStatusNone;
    m->didUpdateSelfKingStatus = kFalse;
    m->didUpdateOpponentKingStatus = kFalse;
	strcpy(m->piecePlacementBeforeMove, "1111111111111111111111111111111111111111111111111111111111111111");
	strcpy(m->piecePlacementAfterMove,  "1111111111111111111111111111111111111111111111111111111111111111");
	m->enPassantTarget = 0;
	m->pawnHalfMoves = 0;
	strcpy(m->castlingStatus, "----");
		
	m->next = NULL;
	m->prev = NULL;
	
	return m;
}

kBool sskAppendMove(sskMoveList * list, sskMove ** move) {
	if (*move == NULL) return kFalse;
	
	if (*list == NULL) {
		*list = *move;
		return kTrue;
	}
	
	// Traverse till the last node of the list.
	sskMove * traverser = NULL;
	traverser = *list;
	
	while ( traverser->next != NULL ) {
		traverser = traverser->next;
	}
	
	traverser->next = *move;
	(*move)->prev = traverser;
	traverser = NULL;
	
	return kTrue;
}

void sskFreeMoveList(sskMoveList * list) {
	if (*list == NULL) return;
	
	sskMove * trav = NULL, * temp = NULL;
	trav = *list;
	
	while(trav) {
		temp = trav;
		trav = trav->next;
		
		sskFreeMove(&temp);
		
		if (temp != NULL) {
			printf("\n Move memory not freed!");
		}
	}
	
	temp = NULL;
	trav = NULL;
}

void sskFreeMove(sskMove ** move) {
	(*move)->prev = NULL;
	(*move)->next = NULL;
	
	free(*move);
	*move = NULL;
}

void sskPrintMove(sskMove m) {
	char * squareLabel = sskSquareToLabel(m.toSquare);
	
	switch (m.castlingType) {
		case sskCastlingTypeBKSide:
		case sskCastlingTypeWKSide:
			printf("\n %3d) %c %2d %02d -> %02d %c OO %c", m.halfmove, ((SSK_GET_PIECE_COLOR(m.pieceMoved) == sskChessColorWhite)?'w':'b'), SSK_GET_GENERIC_PIECE_CODE(m.pieceMoved), m.fromSquare, m.toSquare, sskPieceCodeToxFENSymbol(m.pieceMoved), (m.opponentKingStatus == sskKingStatusCheck)?'+':' ');
			break;
			
		case sskCastlingTypeBQSide:
		case sskCastlingTypeWQSide:
			printf("\n %3d) %c %2d %02d -> %02d %c OOO %c", m.halfmove, ((SSK_GET_PIECE_COLOR(m.pieceMoved) == sskChessColorWhite)?'w':'b'), SSK_GET_GENERIC_PIECE_CODE(m.pieceMoved), m.fromSquare, m.toSquare, sskPieceCodeToxFENSymbol(m.pieceMoved), (m.opponentKingStatus == sskKingStatusCheck)?'+':' ');
			break;
			
		default:
			printf("\n %3d) %c %2d %02d -> %02d %c%s %c", m.halfmove, ((SSK_GET_PIECE_COLOR(m.pieceMoved) == sskChessColorWhite)?'w':'b'), SSK_GET_GENERIC_PIECE_CODE(m.pieceMoved), m.fromSquare, m.toSquare, sskPieceCodeToxFENSymbol(m.pieceMoved), squareLabel, (m.opponentKingStatus == sskKingStatusCheck)?'+':' ');
			break;
	}
	
	if (m.capturedPiece != sskChessPieceNone) {
		printf(" Captured: %s %c", (SSK_GET_PIECE_COLOR(m.pieceMoved) == sskChessColorWhite)?"Black":"White", sskPieceCodeToxFENSymbol(m.capturedPiece));
	}
	
	free(squareLabel);
}

void sskPrintPiecePlacement(char piecePlacement[64]) {
	int i, j;
	
	printf("\n");
	// outer loop iterates rank: 8-1
	// inner loop iterates file: a-h
	for (i = 7; i >= 0; i--) {
		for (j = 0; j < 8; j++) {
			if (piecePlacement[(i * 8) + j] == '1') {
				printf("- ");
			} else {
				printf("%c ", piecePlacement[(i * 8) + j]);
			}
		}
		printf("\n");
	}
}