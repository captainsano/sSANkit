#include "ssankit.h"

#include <time.h>

int main(int argc, const char * argv[]) {
	
	clock_t begin, end;
	
	char * input = NULL;
	
	if (argv[1] != NULL) {
		input = (char *)argv[1];
	} else {
		/*
		input = "e4 e5 Nf3 Nc6 Bb5 a6 Ba4 Nf6 OO Be7 Re1 b5 Bb3 d6 c3 OO h3 Nb8 d4 Nbd7 c4 b4 a3 c5 d5 a5 Nbd2 Nb6 Bc2 g6 ab4 ab4 Ra8 Na8 Nf1 Nb6 b3 Nh5 Bh6 Ng7 g4 Bd7 Ng3 Qc7 Kh2 Ra8 Rg1 Qd8 Nf5 Bf5 ef5 e4 Be4 Bf6 Re1 Nd7 Bf4 Bc3 Re2 Ne8 Qc2 Ne5 Be5 Be5 Kg2 Ra7 fg6 hg6 Bg6 fg6 Qg6 Rg7 Qe6 Rf7 Ne5 de5 Re5 Ng7 Qh6 Qf8 Qe3 Rf4 Re4 Rf6 h4 Ra6 h5 Kh7 Re7 Rf6 f3 Ra6 Qd3 Kh8 Re2 Ne8 Qe3 Nf6 Qe5 Kg8 Kg3 Nd7 Qg5 Kh8 f4 Nf6 Qe5 Qg7 g5 Nh5 Kf3 Qe5 fe5 Ng7 Ke4 Kg8 d6 Ra1 Kd5 Rd1 Kc5 Ne6 Kb4 Kf7 Rf2 Ke8 g6 Rg1 Rf6 Kd7 c5 Nd4 Kc4 Nc6 e6 Ke8 e7 Ne5 Kb5 Nd7 Re6 Rg5 b4";
		 */
		input = "d4 Nf6 c4 g6 Nc3 d5 cd5 Nd5 e4 Nc3 bc3 Bg7 Bc4 c5 Ne2 Nc6 Be3 OO OO b6 dc5 Qc7 f4 bc5 Rb1 Rd8 Qa4 Na5 Bd5 Bd7 Qa3 Rac8 f5 e6 Bf4 Be5 fe6 fe6 Bb3 Nb3 ab3 Rf8 Qc1 c4 b4 Qb6 Kh1 Bg7 e5 Bc6 Nd4 Bd5 Ra1 Qb7 Qc2 Rc7 Bg3 Rcf7 Rf7 Qf7 Qe2 g5 h3 h5 Kg1 h4 Bh2 Qg6 Rd1 g4 hg4 Qg5 Nf5 ef5 Rd5 fg4 Kh1 h3 gh3 gh3 Rd4 Qc1 Rd1 Qc3 e6 Qb2 Qe4 Qg2 Qg2 hg2 Kg2 Re8 Rd6 Bf8 Rc6 Bb4 Rc4 a5 Rc6 Kg7 Kf3 Kf6 Ke4 Re6 Be5 Kf7";
	}
	
	int errorIndex = 0;
	
	begin = clock();
	sskMoveList list = sskLexicalAnalyze(input, &errorIndex , 0, sskChessColorWhite);
	end = clock();
	
	if (list == NULL) {
		printf("\n Lexical analysis failed! Error at: %d", errorIndex);
		return 0;
	}
	
	printf("\n Lexical analysis took %f second(s)", (float)(end-begin)/CLOCKS_PER_SEC);
	

	char * startFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
	strcpy(list->castlingStatus, "HAha");
	list->enPassantTarget = 0;
	list->pawnHalfMoves = 0;
	
	/*
	char * startFEN = "7k/6q1/r4n2/2pPQ2P/1pP2PP1/1P4K1/4R3/8";
	strcpy(list->castlingStatus, "----");
	list->enPassantTarget = 0;
	list->pawnHalfMoves = 0;
	 */
	
	//sskPrintBitboardPosition(*sskxFEN2BitboardPosition(startFEN));
	
	int ambiguousHalfmoveNumber = -1;
	
	begin = clock();
	unsigned int error = sskSemanticAnalyze(list, startFEN, &ambiguousHalfmoveNumber);
	end = clock();
	printf("\n Semantic analysis took %f second(s)", (float)(end-begin)/CLOCKS_PER_SEC);
	
	switch (error) {
		case sskSemanticAnalyzerErrorNone: {
			printf("\n No error!");
			break;
		}
			
		case sskSemanticAnalyzerErrorProvidedMoveListEmpty: {
			printf("\n Provided move list was empty!");
			break;
		}
			
		case sskSemanticAnalyzerErrorFirstPositionNotSpecified: {
			printf("\n First Position not specified");
			break;
		}
			
		case sskSemanticAnalyzerErrorIllegalMove: {
			printf("\n Illegal Move!");
			break;
		}
			
		case sskSemanticAnalyzerErrorAmbiguousMove: {
			printf("\n Ambiguous Move! %d", ambiguousHalfmoveNumber);
			break;
		}
			
		case sskSemanticAnalyzerErrorMovesExistAfterGameEnd: {
			printf("\n Moves exist after game end!");
			break;
		}
			
		default: {
			printf("\n Some Error Occured!");
			break;
		}
	}
	
	printf("\n");
	
	sskMove * trav = list;
	while (trav) {
		sskPrintMove(*trav);
		sskPrintPiecePlacement(trav->piecePlacementAfterMove);
		trav = trav->next;
	}
	
	sskFreeMoveList(&list);
	
	printf("\n");
	return 0;
}
