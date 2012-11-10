/**
 *	@file
 *	Contains implementations of function defined in the corresponding header file.
 *
 *	@author Santhosbaala RS
 *	@copyright 2012 64cloud
 *	@version 1.0
 */

#include "boardformatconvertutil.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

sskBitboardPosition * sskOffsetPositionToBitboardPosition(sskOffsetPosition offsetPosition) {
	int i;
	
	sskBitboardPosition * pos = malloc(sizeof(sskBitboardPosition));
	pos->wKing = (sskBitmap)0x0;
	pos->wQueen = (sskBitmap)0x0;
	pos->wRook = (sskBitmap)0x0;
	pos->wBishop = (sskBitmap)0x0;
	pos->wKnight = (sskBitmap)0x0;
	pos->wPawn = (sskBitmap)0x0;
	pos->bKing = (sskBitmap)0x0;
	pos->bQueen = (sskBitmap)0x0;
	pos->bRook = (sskBitmap)0x0;
	pos->bBishop = (sskBitmap)0x0;
	pos->bKnight = (sskBitmap)0x0;
	pos->bPawn = (sskBitmap)0x0;
	
	for (i = 0; i < 64; i++) {
		switch (offsetPosition[i]) {
			case sskChessPieceWKing: {
				pos->wKing = pos->wKing | SSK_BITMAP_SET_SQUARE_IDX(i);
				break;
			}
			case sskChessPieceWQueen: {
				pos->wQueen = pos->wQueen | SSK_BITMAP_SET_SQUARE_IDX(i);
				break;
			}
			case sskChessPieceWRook: {
				pos->wRook = pos->wRook | SSK_BITMAP_SET_SQUARE_IDX(i);
				break;
			}
			case sskChessPieceWBishop: {
				pos->wBishop = pos->wBishop | SSK_BITMAP_SET_SQUARE_IDX(i);
				break;
			}
			case sskChessPieceWKnight: {
				pos->wKnight = pos->wKnight | SSK_BITMAP_SET_SQUARE_IDX(i);
				break;
			}
			case sskChessPieceWPawn: {
				pos->wPawn = pos->wPawn | SSK_BITMAP_SET_SQUARE_IDX(i);
				break;
			}
			case sskChessPieceBKing: {
				pos->bKing = pos->bKing | SSK_BITMAP_SET_SQUARE_IDX(i);
				break;
			}
			case sskChessPieceBQueen: {
				pos->bQueen = pos->bQueen | SSK_BITMAP_SET_SQUARE_IDX(i);
				break;
			}
			case sskChessPieceBRook: {
				pos->bRook = pos->bRook | SSK_BITMAP_SET_SQUARE_IDX(i);
				break;
			}
			case sskChessPieceBBishop: {
				pos->bBishop = pos->bBishop | SSK_BITMAP_SET_SQUARE_IDX(i);
				break;
			}
			case sskChessPieceBKnight: {
				pos->bKnight = pos->bKnight | SSK_BITMAP_SET_SQUARE_IDX(i);
				break;
			}
			case sskChessPieceBPawn: {
				pos->bPawn = pos->bPawn | SSK_BITMAP_SET_SQUARE_IDX(i);
				break;
			}
				
			case sskChessPieceNone:
			default: break;
		}
	}
	
	pos->wOccupied = pos->wPawn | pos->wKing | pos->wQueen | pos->wRook | pos->wBishop | pos->wKnight;
	pos->bOccupied = pos->bPawn | pos->bKing | pos->bQueen | pos->bRook | pos->bBishop | pos->bKnight;
	pos->occupied = pos->wOccupied | pos->bOccupied;
		
	return pos;
}

sskOffsetPosition sskBitboardPositionToOffsetPosition(sskBitboardPosition bitboardPosition) {
	sskOffsetPosition offsetPosition = (sskOffsetPosition)malloc(sizeof(sskChessPiece) * 64);
	sskChessPiece i;
	sskBitmap pieceBitboard;
	sskChessSquare square;
    
    // Clear the board
	memset(offsetPosition, 0, sizeof(sskChessPiece) * 64);

	// White Pieces
	for (i = sskChessPieceWPawn; i <= sskChessPieceWKnight; i++) {
		pieceBitboard = sskBitmapForPieceInBitboardPosition(bitboardPosition, i);
		while (pieceBitboard) {
			square = sskFirstOneIndex(pieceBitboard);
			pieceBitboard = pieceBitboard & SSK_BITMAP_UNSET_SQUARE_IDX(square);
			offsetPosition[square] = i;
		}
	}
	
	// Black Pieces
	for (i = sskChessPieceBPawn; i <= sskChessPieceBKnight; i++) {
		pieceBitboard = sskBitmapForPieceInBitboardPosition(bitboardPosition, i);
		while (pieceBitboard) {
			square = sskFirstOneIndex(pieceBitboard);
			pieceBitboard = pieceBitboard & SSK_BITMAP_UNSET_SQUARE_IDX(square);
			offsetPosition[square] = i;
		}
	}
	
	return offsetPosition;
}

void sskFillPiecePlacementWithOffsetPosition(char piecePlacement[65], sskOffsetPosition offsetPosition) {
	int i;
	
	for (i = 0; i < 64; i++) {
		switch (SSK_GET_GENERIC_PIECE_CODE(offsetPosition[i])) {
			case sskChessPieceKing: {
				piecePlacement[i] = 'K';
				break;
			}
			case sskChessPieceQueen: {
				piecePlacement[i] = 'Q';
				break;
			}
			case sskChessPieceRook: {
				piecePlacement[i] = 'R';
				break;
			}
			case sskChessPieceBishop: {
				piecePlacement[i] = 'B';
				break;
			}
			case sskChessPieceKnight: {
				piecePlacement[i] = 'N';
				break;
			}
			case sskChessPiecePawn: {
				piecePlacement[i] = 'P';
				break;
			}
			case sskChessPieceNone:
			default: {
				piecePlacement[i] = '1';
			}
		}
		
		if (offsetPosition[i] != sskChessPieceNone) {
			switch(SSK_GET_PIECE_COLOR(offsetPosition[i])) {
				case sskChessColorWhite: {
					piecePlacement[i] = toupper(piecePlacement[i]);
					break;
				}
				case sskChessColorBlack: {
					piecePlacement[i] = tolower(piecePlacement[i]);
					break;
				}
			}
		}
	}
	
	piecePlacement[64] = '\0';
}

sskOffsetPosition sskPiecePlacementStringToOffsetPosition(const char piecePlacement[65]) {
	sskOffsetPosition offsetPosition = (sskOffsetPosition)malloc(sizeof(sskChessPiece) * 64);
	int i;
	
	// Clear the board
	memset(offsetPosition, 0, sizeof(sskChessPiece) * 64);
	
	for (i = 0; i < 64; i++) {
		switch (piecePlacement[i]) {
			case 'K': offsetPosition[i] = sskChessPieceWKing; break;
			case 'Q': offsetPosition[i] = sskChessPieceWQueen; break;
			case 'R': offsetPosition[i] = sskChessPieceWRook; break;
			case 'N': offsetPosition[i] = sskChessPieceWKnight; break;
			case 'B': offsetPosition[i] = sskChessPieceWBishop; break;
			case 'P': offsetPosition[i] = sskChessPieceWPawn; break;
			case 'k': offsetPosition[i] = sskChessPieceBKing; break;
			case 'q': offsetPosition[i] = sskChessPieceBQueen; break;
			case 'r': offsetPosition[i] = sskChessPieceBRook; break;
			case 'n': offsetPosition[i] = sskChessPieceBKnight; break;
			case 'b': offsetPosition[i] = sskChessPieceBBishop; break;
			case 'p': offsetPosition[i] = sskChessPieceBPawn; break;
		}
	}
	
	return offsetPosition;
}

sskBitboardPosition * sskPiecePlacementStringToBitboardPosition(const char piecePlacement[65]) {
	sskOffsetPosition offsetPosition = sskPiecePlacementStringToOffsetPosition(piecePlacement);
	sskBitboardPosition * bitboardPosition = sskOffsetPositionToBitboardPosition(offsetPosition);
	free(offsetPosition);
	return bitboardPosition;
}