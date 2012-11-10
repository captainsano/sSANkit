/**
 *	@file
 *	Contains function declaration for the lexical analyzer.
 *
 *	@author Santhosbaala RS
 *	@copyright 2012 64cloud
 *	@version 0.1
 */

#ifndef LEXER
#define LEXER

#include "ssandef.h"
 
/** @todo: Make a silent version of the lexer */

/**
 *	Function analyzes the string move list for validity. Also, it
 *	parses out information and returns a MoveList data structure
 *	with partial information, which is to be filled by the semantic
 *	analyzer.
 *
 *	@param input	The input string.
 *
 *	@return	Returns a pointer to the head node of the MoveList on success. 
 *			On failure, returns NULL, check errorIndex for information.
 */
sskMoveList sskLexicalAnalyze(char * input, int * errorIndex, unsigned int halfmove, sskChessColor color);

#endif