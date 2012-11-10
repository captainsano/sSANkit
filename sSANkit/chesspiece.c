/**
 *	@file
 *	Implementation of the utility functions declared in the corresponding header file.
 *
 *	@author Santhosbaala RS
 *	@copyright 2012 64cloud
 *	@version 0.1
 */

#include "chesspiece.h"

sskChessPiece sskSANsymbolToPieceCode(char aSANsymbol) {
	switch(aSANsymbol) {
		case ' ': return sskChessPiecePawn;
		case 'K': return sskChessPieceKing;
		case 'Q': return sskChessPieceQueen;
		case 'R': return sskChessPieceRook;
		case 'B': return sskChessPieceBishop;
		case 'N': return sskChessPieceKnight;
	}
	
	return sskChessPieceNone;
}

char sskPieceCodeToxFENSymbol(sskChessPiece aPieceCode) {
	switch (aPieceCode) {
		default:
		case sskChessPieceNone:	return ' ';
            
		case sskChessPieceWPawn:	return 'P';
		case sskChessPieceWKing:	return 'K';
		case sskChessPieceWQueen:	return 'Q';
		case sskChessPieceWRook:	return 'R';
		case sskChessPieceWBishop:	return 'B';
		case sskChessPieceWKnight:	return 'N';
			
		case sskChessPieceBPawn:	return 'p';
		case sskChessPieceBKing:	return 'k';
		case sskChessPieceBQueen:	return 'q';
		case sskChessPieceBRook:	return 'r';
		case sskChessPieceBBishop:	return 'b';
		case sskChessPieceBKnight:	return 'n';
	}
}