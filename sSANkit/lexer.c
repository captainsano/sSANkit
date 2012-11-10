/**
 *	@file
 *	Contains implementation of the lexical analyzer.
 *
 *	@author Santhosbaala RS
 *	@copyright 2012 64cloud
 *	@version 0.3
 */

#include "lexer.h"

/*------------------- Internal Methods, not included in the API ------------------*/
sskMoveList lex_movelist(char ** input, unsigned int halfmove, sskChessColor color);
kBool lex_move(char ** input, sskMove ** currentMove);
kBool lex_A(char ** input, sskMove ** currentMove);
kBool lex_B(char ** input, sskMove ** currentMove);
kBool lex_C(char ** input, sskMove ** currentMove);
kBool lex_D(char ** input, sskMove ** currentMove);

kBool lex_file(char ** input);
kBool lex_rank(char ** input);
kBool lex_piece(char ** input);

kBool lex_match(char ** input, char x);
kBool lex_eoi(char ** input);

void lex_consume(char ** input);
/*------------------- Internal Methods, not included in the API ------------------*/

sskMoveList sskLexicalAnalyze(char * input, int * errorIndex, unsigned int halfmove, sskChessColor color) {
	char * inputBase = input;
	
	sskMoveList list = lex_movelist(&input, halfmove, color);
	
	if (list == NULL) {
		*errorIndex = (int)(input - inputBase);
		sskFreeMoveList(&list);
		return NULL;
	}
	
	*errorIndex = -1;
	return list;
}

sskMoveList lex_movelist(char ** input, unsigned int halfmove, sskChessColor color) {
	
	sskMoveList list = NULL;

	while ( !lex_eoi(input) ) {

		sskMove * m = sskCreateBlankMove();
		m->halfmove = halfmove;
		m->pieceMoved |= (color << 3);
		
		if (lex_move(input, &m) == kFalse) {
			sskFreeMove(&m);
			sskFreeMoveList(&list);
			return NULL;
		} else {
			sskAppendMove(&list, &m);
			
			halfmove += 1;
			color = !color;
			m = NULL;
		}
		
		if ( lex_eoi(input) ) return list;
	
		if( lex_match(input, ' ')) {
			lex_consume(input);
		} else {
			printf("\n lex_movelist(): Error, Expected:  (space)");
			sskFreeMoveList(&list);
			return NULL;
		}
	}

	return list;
}

kBool lex_move(char ** input, sskMove ** currentMove) {
	//printf("\n lex_move(): %c\n", **input);	
	if ( lex_file(input) ) {
		lex_consume(input);
		return lex_A(input, currentMove);
	}

	if ( lex_match(input, 'K') ) {
		lex_consume(input);

		if ( lex_file(input) ) {
			lex_consume(input);	
		} else {
			printf("\n lex_move(): Error, Expected <File>");
			return kFalse;
		}
		
		if ( lex_rank(input) ) {
			lex_consume(input);
		} else {
			printf("\n lex_move(): Error, Expected <Rank>");
			return kFalse;
		}

		(*currentMove)->pieceMoved |= sskChessPieceKing;
		(*currentMove)->fromSquare = 0;
		(*currentMove)->toSquare = SSK_SQUARE_IDX_FOR_FILE_RANK_CHAR( *(*input - 2), *(*input - 1) );
		return kTrue;
	}

	if ( lex_piece(input) ) {
		lex_consume(input);
		return lex_C(input, currentMove);
	}

	if ( lex_match(input, '.') ) {
		lex_consume(input);
		
		(*currentMove)->pieceMoved = 0;
		(*currentMove)->fromSquare = 0;
		(*currentMove)->toSquare = 0;
		return kTrue;
	}

	if ( lex_match(input, 'O') ) {
		lex_consume(input);

		if ( lex_match(input, 'O') ){
			lex_consume(input);
		} else {
			printf("\n Error, Expected O (K/Qside Castle)");
			return kFalse;
		}		

		if ( lex_eoi(input) || lex_match(input, ' ') ) {
			(*currentMove)->pieceMoved |= sskChessPieceKing;
			(*currentMove)->castlingType = (SSK_GET_PIECE_COLOR((*currentMove)->pieceMoved) == sskChessColorBlack)?sskCastlingTypeBKSide:sskCastlingTypeWKSide;
			(*currentMove)->fromSquare = (SSK_GET_PIECE_COLOR((*currentMove)->pieceMoved) == sskChessColorBlack)?SSK_SQUARE_IDX_FOR_RANK_IDX(7):SSK_SQUARE_IDX_FOR_RANK_IDX(0);
			(*currentMove)->fromSquareExtracted = sskChessSquareInfoRankOnly;
			return kTrue;
		}

		if ( lex_match(input, 'O') ) {
			lex_consume(input);
		} else {
			printf("\n Error, Expected O (Qside Castle)");
			return kFalse;
		}

		(*currentMove)->pieceMoved |= sskChessPieceKing;
		(*currentMove)->castlingType = (SSK_GET_PIECE_COLOR((*currentMove)->pieceMoved) == sskChessColorBlack)?sskCastlingTypeBQSide:sskCastlingTypeWQSide;
		(*currentMove)->fromSquare = (SSK_GET_PIECE_COLOR((*currentMove)->pieceMoved) == sskChessColorBlack)?SSK_SQUARE_IDX_FOR_RANK_IDX(7):SSK_SQUARE_IDX_FOR_RANK_IDX(0);
		(*currentMove)->fromSquareExtracted = sskChessSquareInfoRankOnly;
		return kTrue;
	}

	printf("\n lex_move() Error, Expected <File>, 'K' or <Piece>");
	return kFalse;
}

kBool lex_A(char ** input, sskMove ** currentMove) {
	//printf("\n lex_A(): %c\n", **input);
	if ( lex_match(input, '2')
		|| lex_match(input, '3')
		|| lex_match(input, '4')
		|| lex_match(input, '5')
		|| lex_match(input, '6')
		|| lex_match(input, '7')) {
		lex_consume(input);

		(*currentMove)->pieceMoved |= sskChessPiecePawn;
		(*currentMove)->fromSquare |= SSK_SQUARE_IDX_FOR_FILE_CHAR( *(*input - 2) );
		(*currentMove)->toSquare = SSK_SQUARE_IDX_FOR_FILE_RANK_CHAR( *(*input - 2) , *(*input - 1) );
		(*currentMove)->fromSquareExtracted = sskChessSquareInfoFileOnly;
		return kTrue;
	}

	if ( lex_match(input, '8') ) {
		lex_consume(input);
		
		if ( lex_piece(input) ) {
			lex_consume(input);
			
			(*currentMove)->pieceMoved |= sskChessPiecePawn;
			(*currentMove)->fromSquare = SSK_SQUARE_IDX_FOR_FILE_RANK_CHAR( *(*input - 3) , '7' );
			(*currentMove)->toSquare = SSK_SQUARE_IDX_FOR_FILE_RANK_CHAR( *(*input - 3) , '8' );
			(*currentMove)->promotedPiece = sskSANsymbolToPieceCode( *(*input - 1) );
			(*currentMove)->fromSquareExtracted = sskChessSquareInfoFileAndRank;
			return kTrue;
		} else {
			printf("\n lex_B(): Error, Expected <Piece>");
			return kFalse;
		}
	}	

	if ( lex_match(input, '1') ) {
		lex_consume(input);
		
		if ( lex_piece(input) ) {
			lex_consume(input);
			(*currentMove)->pieceMoved |= sskChessPiecePawn;
			(*currentMove)->fromSquare = SSK_SQUARE_IDX_FOR_FILE_RANK_CHAR( *(*input - 3) , '2' );
			(*currentMove)->toSquare = SSK_SQUARE_IDX_FOR_FILE_RANK_CHAR( *(*input - 3) , '1' );
			(*currentMove)->promotedPiece = sskSANsymbolToPieceCode( *(*input - 1) );
			(*currentMove)->fromSquareExtracted = sskChessSquareInfoFileAndRank;
			return kTrue;
		} else {
			printf("\n lex_B(): Error, Expected <Piece>");
			return kFalse;
		}
	}

	if ( lex_file(input) ) {
		lex_consume(input);
		return lex_B(input, currentMove);
	}

	printf("\n lex_A(): Error, Expected '8', '1', <'2'-'7'> or <File>");
	return kFalse;
}

kBool lex_B(char ** input, sskMove ** currentMove) {
	//printf("\n lex_B(): %c", **input);
	if ( lex_match(input, '2')
		|| lex_match(input, '3')
		|| lex_match(input, '4')
		|| lex_match(input, '5')
		|| lex_match(input, '6')
		|| lex_match(input, '7')) {
		lex_consume(input);

		(*currentMove)->pieceMoved |= sskChessPiecePawn;
		(*currentMove)->fromSquare |= SSK_SQUARE_IDX_FOR_FILE_CHAR( *(*input - 3) );
		(*currentMove)->toSquare = SSK_SQUARE_IDX_FOR_FILE_RANK_CHAR( *(*input - 2) , *(*input - 1) );
		(*currentMove)->fromSquareExtracted = sskChessSquareInfoFileOnly;
		return kTrue;
	}

	if ( lex_match(input, '8') ) {
		lex_consume(input);
		
		if ( lex_piece(input) ) {
			lex_consume(input);
			
			(*currentMove)->pieceMoved |= sskChessPiecePawn;
			(*currentMove)->fromSquare = SSK_SQUARE_IDX_FOR_FILE_RANK_CHAR( *(*input - 4) , '7' );
			(*currentMove)->toSquare = SSK_SQUARE_IDX_FOR_FILE_RANK_CHAR( *(*input - 3) , '8' );
			(*currentMove)->promotedPiece = sskSANsymbolToPieceCode( *(*input - 1) );
			(*currentMove)->fromSquareExtracted = sskChessSquareInfoFileAndRank;
			return kTrue;
		} else {
			printf("\n lex_B(): Error, Expected <Piece>");
			return kFalse;
		}
	}	

	if ( lex_match(input, '1') ) {
		lex_consume(input);
		
		if ( lex_piece(input) ) {
			lex_consume(input);

			(*currentMove)->pieceMoved |= sskChessPiecePawn;
			(*currentMove)->fromSquare = SSK_SQUARE_IDX_FOR_FILE_RANK_CHAR( *(*input - 4) , '2' );
			(*currentMove)->toSquare = SSK_SQUARE_IDX_FOR_FILE_RANK_CHAR( *(*input - 3) , '1' );
			(*currentMove)->promotedPiece = sskSANsymbolToPieceCode( *(*input - 1) );
			(*currentMove)->fromSquareExtracted = sskChessSquareInfoFileAndRank;
			return kTrue;
		} else {
			printf("\n lex_B(): Error, Expected <Piece>");
			return kFalse;
		}
	}

	printf("\n lex_B(): Error, Expected '1', '2' or <'2'-'7'>");
	return kFalse;
}

kBool lex_C(char ** input, sskMove ** currentMove) {
	//printf("\n lex_C(): %c", **input);
	if ( lex_rank(input) ) {
		lex_consume(input);
		
		if ( lex_file(input) ) {
			lex_consume(input);	
		} else {
			printf("\n lex_C(): Error, Expected <File>");
			return kFalse;
		}
		
		if ( lex_rank(input) ) {
			lex_consume(input);
		} else {
			printf("\n lex_C(): Error, Expected <Rank>");
			return kFalse;
		}

		(*currentMove)->pieceMoved |= sskSANsymbolToPieceCode( *(*input - 4) );
		(*currentMove)->fromSquare |= SSK_SQUARE_IDX_FOR_RANK_CHAR( *(*input - 3) );
		(*currentMove)->toSquare = SSK_SQUARE_IDX_FOR_FILE_RANK_CHAR( *(*input - 2) , *(*input - 1) );
		(*currentMove)->fromSquareExtracted = sskChessSquareInfoRankOnly;
		return kTrue;
	}

	if ( lex_file(input) ) {
		lex_consume(input);

		return lex_D(input, currentMove);
	}

	printf("\n lex_C(): Error, Expected <Rank> or <File>");
	return kFalse;
}

kBool lex_D(char ** input, sskMove ** currentMove) {
	//printf("\n lex_D(): %c", **input);
	if ( lex_file(input) ) {
		lex_consume(input);

		if ( lex_rank(input) ) {
			lex_consume(input);
		} else {
			printf("\n lex_D(): Error, Expected <Rank>");
			return kFalse;
		}
		
		(*currentMove)->pieceMoved |= sskSANsymbolToPieceCode( *(*input - 4) );
		(*currentMove)->fromSquare |= SSK_SQUARE_IDX_FOR_FILE_CHAR( *(*input - 3) );
		(*currentMove)->toSquare = SSK_SQUARE_IDX_FOR_FILE_RANK_CHAR( *(*input - 2) , *(*input - 1) );
		(*currentMove)->fromSquareExtracted = sskChessSquareInfoFileOnly;
		return kTrue;
	}

	if ( lex_rank(input) ) {
		lex_consume(input);

		if ( lex_eoi(input) || lex_match(input, ' ') ) {
			(*currentMove)->pieceMoved |= sskSANsymbolToPieceCode( *(*input - 3) );
			(*currentMove)->fromSquare = 0;
			(*currentMove)->toSquare = SSK_SQUARE_IDX_FOR_FILE_RANK_CHAR( *(*input - 2) , *(*input - 1) );
			return kTrue;
		}

		if ( lex_file(input) ) {
			lex_consume(input);

			if ( lex_rank(input) ) {
				lex_consume(input);

				(*currentMove)->pieceMoved |= sskSANsymbolToPieceCode( *(*input - 5) );
				(*currentMove)->fromSquare  = SSK_SQUARE_IDX_FOR_FILE_RANK_CHAR( *(*input - 4), *(*input - 3) );
				(*currentMove)->toSquare = SSK_SQUARE_IDX_FOR_FILE_RANK_CHAR( *(*input - 2) , *(*input - 1) );
				(*currentMove)->fromSquareExtracted = sskChessSquareInfoFileAndRank;
				return kTrue;
			}
			
			printf("\n lex_D(): Error, Expected <Rank>");
			return kFalse;
		}
	}

	printf("\n lex_D(): Error, Expected <File> or <Rank> ");
	printf("Got: %c(%d)", **input, **input);
	return kFalse;
}

kBool lex_file(char ** input) {
	kBool r = (lex_match(input, 'a')
			|| lex_match(input, 'b')
			|| lex_match(input, 'c')
			|| lex_match(input, 'd')
			|| lex_match(input, 'e')
			|| lex_match(input, 'f')
			|| lex_match(input, 'g')
			|| lex_match(input, 'h'));
	return r;
}

kBool lex_rank(char ** input) {
	kBool r = (lex_match(input, '1')
			|| lex_match(input, '2')
			|| lex_match(input, '3')
			|| lex_match(input, '4')
			|| lex_match(input, '5')
			|| lex_match(input, '6')
			|| lex_match(input, '7')
			|| lex_match(input, '8'));
	return r;	
}

kBool lex_piece(char ** input) {
	kBool r = (lex_match(input, 'Q')
			|| lex_match(input, 'R')
			|| lex_match(input, 'B')
			|| lex_match(input, 'N'));
	return r;
}

kBool lex_match(char ** input, char x) {
	// printf("\n lex_match(): %c with %c \n", **input, x);
	if (**input == x) return kTrue;
	return kFalse;
}

kBool lex_eoi(char ** input) {
	if (*input == NULL || **input == '\0' || **input == EOF || **input == 0) return kTrue;
	return kFalse;
}

void lex_consume(char ** input) {
	//printf("%c", **input);
	*input += 1;
}