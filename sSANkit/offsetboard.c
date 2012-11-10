/**
 *	@file
 *	Implements function declared in offsetboard.h
 *
 *	@author Santhosbaala RS
 *	@copyright 2012 64cloud
 *	@version 1.0
 */

#include "offsetboard.h"
#include <stdio.h>	// For NULL
#include <stdlib.h>	// For malloc() and free()
#include <string.h>	// For memeset
#include <ctype.h>	// For toupper() and tolower()

sskOffsetPosition sskxFENtoOffsetPosition(char * xFENstring) {
	sskOffsetPosition pos = NULL;
	char * ptr = (char *)xFENstring;
	int i, j;
	
	pos = malloc(sizeof(sskChessPiece) * 64);
	
	// Clear the position
	memset(pos, 0, sizeof(sskChessPiece) * 64);
	
	for (i = 7; i >= 0; i--) {
		for (j = 0; j < 8; j++) {
			switch (*ptr) {
				case '/': j--; break;
					
				default:
				case ' ': return pos;
					
				case '1': j += 0; break;
				case '2': j += 1; break;
				case '3': j += 2; break;
				case '4': j += 3; break;
				case '5': j += 4; break;
				case '6': j += 5; break;
				case '7': j += 6; break;
				case '8': j += 7; break;
					
				case 'p': pos[ (i * 8) + j] = sskChessPieceBPawn; break;
				case 'k': pos[ (i * 8) + j] = sskChessPieceBKing; break;
				case 'q': pos[ (i * 8) + j] = sskChessPieceBQueen; break;
				case 'r': pos[ (i * 8) + j] = sskChessPieceBRook; break;
				case 'b': pos[ (i * 8) + j] = sskChessPieceBBishop; break;
				case 'n': pos[ (i * 8) + j] = sskChessPieceBKnight; break;
					
				case 'P': pos[ (i * 8) + j] = sskChessPieceWPawn; break;
				case 'K': pos[ (i * 8) + j] = sskChessPieceWKing; break;
				case 'Q': pos[ (i * 8) + j] = sskChessPieceWQueen; break;
				case 'R': pos[ (i * 8) + j] = sskChessPieceWRook; break;
				case 'B': pos[ (i * 8) + j] = sskChessPieceWBishop; break;
				case 'N': pos[ (i * 8) + j] = sskChessPieceWKnight; break;
			}
			
			ptr += 1;
		}
	}
	
	return pos;
}

sskOffsetPosition sskCopyOffsetPosition(sskOffsetPosition offsetPosition) {
	sskOffsetPosition copy = malloc(sizeof(sskChessPiece) * 64);
	memcpy((void *)copy, (const void *)offsetPosition, sizeof(sskChessPiece) * 64);
	return copy;
}

void sskPrintOffsetPosition(sskOffsetPosition offsetPosition) {
	int i, j;
	
	printf("\n");
	// outer loop iterates rank: 8-1
	// inner loop iterates file: a-h
	for (i = 7; i >= 0; i--) {
		for (j = 0; j < 8; j++) {
			char fenPiece = '-';
			
			switch (SSK_GET_GENERIC_PIECE_CODE((offsetPosition[(i * 8) + j]))) {
				case sskChessPiecePawn:	fenPiece = 'p'; break;
				case sskChessPieceKing:	fenPiece = 'k'; break;
				case sskChessPieceQueen:	fenPiece = 'q'; break;
				case sskChessPieceRook:	fenPiece = 'r'; break;
				case sskChessPieceBishop:	fenPiece = 'b'; break;
				case sskChessPieceKnight:	fenPiece = 'n'; break;
				case sskChessPieceNone:
				default:
					fenPiece = '-';
					break;
			}
			
			if( SSK_GET_PIECE_COLOR( (offsetPosition[(i * 8) + j]) ) == sskChessColorWhite ) {
				fenPiece = toupper(fenPiece);
			}
			
			printf("%c ", fenPiece);
		}
		printf("\n");
	}
}