/**
 *	@file
 *	Implementation of the utility functions declared in the corresponding header file.
 *
 *	@author Santhosbaala RS
 *	@copyright 2012 64cloud
 *	@version 0.1
 */

#include "ChessSquare.h"
#include <stdlib.h>

char * sskSquareToLabel(sskChessSquare aSquareIndex) {
	char * label = NULL;
	
	if (aSquareIndex <= 63) {
		label = (char *)malloc(sizeof(char) * 3);
		label[0] = 'a' + SSK_GET_FILE_IDX(aSquareIndex);
		label[1] = '1' + SSK_GET_RANK_IDX(aSquareIndex);
		label[2] = '\0';
	}
	
	return label;
}