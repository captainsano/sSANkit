/**
 *	@file
 *	Contains implementation of the functions declared in the corresponding header file.
 *
 *	@author Santhosbaala RS
 *	@copyright 2012 64cloud
 *	@version 0.1
 */

#include "semantic_analyzer.h"

sskSemanticAnalyzerError sskSemanticAnalyze(sskMoveList moveList, char * startingPosition, int * ambiguousHalfmoveNumber) {
	// Move List is NULL.
	if (moveList == NULL) { return sskSemanticAnalyzerErrorProvidedMoveListEmpty; }
	
	// Starting FEN not specified
	if (startingPosition == NULL || strlen(startingPosition) == 0) {
		return sskSemanticAnalyzerErrorFirstPositionNotSpecified;
	}
	
	// Bitboard Position for calculating piece movements
	sskBitboardPosition * curBitPos = sskxFEN2BitboardPosition(startingPosition), * temp = NULL;
	
	// Offset Board for easy lookup of piece positions.
	sskOffsetPosition curOffsetPos = sskxFENtoOffsetPosition(startingPosition);
		
	// The list traverser.
	sskMove * trav = NULL;
	trav = moveList;
	kBool ambiguity = kFalse;
	
	while (trav != NULL) {
        // NULL move condition
		if (trav->pieceMoved == sskChessPieceNone) {
			trav = trav->next;
			continue;
		}
        
        /*------------ Update self king status before the move -----------*/
		if (!trav->didUpdateSelfKingStatus) {
			// Check for checks
			int numChecks = sskIsKingUnderCheck(*curBitPos, SSK_GET_PIECE_COLOR(trav->pieceMoved), kFalse, NULL);
            if (numChecks > 0) {
                trav->selfKingStatus = sskKingStatusCheck;
			}
            
            // Check for checkmate or stalemate
			if (trav->selfKingStatus == sskKingStatusCheck) {
				
				if (sskIsKingUnderCheckMate(*curBitPos, SSK_GET_PIECE_COLOR(trav->pieceMoved), numChecks, trav->enPassantTarget)) {
					trav->selfKingStatus = sskKingStatusCheckMate;
				}
				
			} else {
				if (sskIsKingUnderStalemate(*curBitPos, SSK_GET_PIECE_COLOR(trav->pieceMoved), trav->enPassantTarget)) {
					trav->selfKingStatus = sskKingStatusStalemate;
				}
			}
            
            trav->didUpdateSelfKingStatus = kTrue;
        }
				
		/*------ Fill move's piece placement string before the move -----*/
		sskFillPiecePlacementWithOffsetPosition(trav->piecePlacementBeforeMove, curOffsetPos);
		
		/*------------- Before Proceeding to prcess the move, abort if the game has already ended -------------*/
		if (trav->selfKingStatus == sskKingStatusCheckMate || trav->selfKingStatus == sskKingStatusStalemate) {
			return sskSemanticAnalyzerErrorMovesExistAfterGameEnd;
		}
		
		// Insufficient pieces condition - Only 2 Kings
		if (sskCountBits(curBitPos->occupied) == 2) {
			return sskSemanticAnalyzerErrorMovesExistAfterGameEnd;
		}
		
		// Insufficient pieces condition - 2 Kings and one Bishop or Knight
		if (sskCountBits(curBitPos->occupied) == 3) {
			if ((sskCountBits(curBitPos->wBishop) == 1) ||
				(sskCountBits(curBitPos->bBishop) == 1) ||
				(sskCountBits(curBitPos->wKnight) == 1) ||
				(sskCountBits(curBitPos->bKnight) == 1)
				) {
				return sskSemanticAnalyzerErrorMovesExistAfterGameEnd;
			}
		}
		
		/*--------- Verify if the move is pseudo-legal. ---------*/
		if (sskFillFromSquare(*curBitPos, curOffsetPos, trav, &ambiguity) == kFalse) {
			return sskSemanticAnalyzerErrorIllegalMove;
		}
		
		if (ambiguity) {
			if (ambiguousHalfmoveNumber != NULL) {
				*ambiguousHalfmoveNumber = trav->halfmove;
			}
			return sskSemanticAnalyzerErrorAmbiguousMove;
		}
		
		/*------------ Verify if the move is legal -------------*/
		temp = sskCheckLegal(*curBitPos, trav, kTrue);
        if (temp == NULL) {
            return sskSemanticAnalyzerErrorIllegalMove;
        }
        
        // use the updated bitboard from checkLegal() to update current bitboard.
        // NOTE: At this point we don't update the offset board yet!
        free(curBitPos);
        curBitPos = temp;
        temp = NULL;
				
        /*------------ Update current move's status with old offset board -----------*/
        // capture
        if ((curOffsetPos[trav->toSquare] != sskChessPieceNone)) {
            trav->capturedPiece = SSK_GET_GENERIC_PIECE_CODE(curOffsetPos[trav->toSquare]);
        }
        
        // en-passant
        if ((SSK_GET_GENERIC_PIECE_CODE(trav->pieceMoved) == sskChessPiecePawn) && (trav->enPassantTarget == trav->toSquare)) {
            trav->capturedPiece = (sskChessPiecePawn) | (!SSK_GET_PIECE_COLOR(trav->pieceMoved) << 3);
        }
        
        /*-------------------- Update next move's status (if any) -------------------*/
        if (trav->next != NULL) {
            /*---------------- Adjust castling options for next move ---------------*/
            strcpy(trav->next->castlingStatus, trav->castlingStatus);	// Copy the castlingStatus string
			
			// King was castled in the current move
			if (trav->castlingType == sskCastlingTypeWKSide) trav->next->castlingStatus[0] = '-';
			if (trav->castlingType == sskCastlingTypeWQSide) trav->next->castlingStatus[1] = '-';
            if (trav->castlingType == sskCastlingTypeBKSide) trav->next->castlingStatus[2] = '-';
            if (trav->castlingType == sskCastlingTypeBQSide) trav->next->castlingStatus[3] = '-';
			
			// King was moved in the current move
			if (trav->pieceMoved == sskChessPieceWKing) {
				trav->next->castlingStatus[0] = trav->next->castlingStatus[1] = '-';
			}
            if (trav->pieceMoved == sskChessPieceBKing) {
                trav->next->castlingStatus[2] = trav->next->castlingStatus[3] = '-';
            }
			
			// The rook was moved in the current move
			if ((trav->pieceMoved == sskChessPieceWRook) && (trav->castlingStatus[0] != '-')) {
                if ((SSK_CHAR_2_FILE(tolower(trav->castlingStatus[0])) == SSK_GET_FILE_IDX(trav->fromSquare)) && (SSK_GET_RANK_IDX(trav->fromSquare) == 0)) {
                    trav->next->castlingStatus[0] = '-';
                }
            }
            if ((trav->pieceMoved == sskChessPieceWRook) && (trav->castlingStatus[1] != '-')) {
                if ((SSK_CHAR_2_FILE(tolower(trav->castlingStatus[1])) == SSK_GET_FILE_IDX(trav->fromSquare)) && (SSK_GET_RANK_IDX(trav->fromSquare) == 0)) {
                    trav->next->castlingStatus[1] = '-';
                }
            }
            if ((trav->pieceMoved == sskChessPieceBRook) && (trav->castlingStatus[2] != '-')) {
                if ((SSK_CHAR_2_FILE(tolower(trav->castlingStatus[2])) == SSK_GET_FILE_IDX(trav->fromSquare)) && (SSK_GET_RANK_IDX(trav->fromSquare) == 7)) {
                    trav->next->castlingStatus[2] = '-';
                }
            }
            if ((trav->pieceMoved == sskChessPieceBRook) && (trav->castlingStatus[3] != '-')) {
                if ((SSK_CHAR_2_FILE(tolower(trav->castlingStatus[3])) == SSK_GET_FILE_IDX(trav->fromSquare)) && (SSK_GET_RANK_IDX(trav->fromSquare) == 7)) {
                    trav->next->castlingStatus[3] = '-';
                }
            }
            
            // Set enpassant target for next move if the current pawn move was a double move
            if (SSK_GET_GENERIC_PIECE_CODE(trav->pieceMoved) == sskChessPiecePawn) {
                // Look for a double move - White Side
				if ((SSK_GET_RANK_IDX(trav->toSquare) - SSK_GET_RANK_IDX(trav->fromSquare)) == 2) {
                    // Mark the previous square as enpassant target
                    trav->next->enPassantTarget = trav->toSquare - 8;
                }
				
				// Look for a double move - Black Side
                if ((SSK_GET_RANK_IDX(trav->toSquare) - SSK_GET_RANK_IDX(trav->fromSquare)) == -2) {
                    // Mark the previous square as enpassant target
                    trav->next->enPassantTarget = trav->toSquare + 8;
                }
            }
			
			// Update the halfmove clock (pawn plys) - moves since last pawn move/capture for the next move.
			if (SSK_GET_GENERIC_PIECE_CODE(trav->pieceMoved) == sskChessPiecePawn || trav->capturedPiece != sskChessPieceNone) {
				trav->next->pawnHalfMoves = 0;
			} else {
				trav->next->pawnHalfMoves = trav->pawnHalfMoves + 1;
			}
        }
		       
        // Udpate the offset position with the new bitboard position from checkLegal()
        free(curOffsetPos);
		curOffsetPos = sskBitboardPositionToOffsetPosition(*curBitPos);
						
		/*------------ Update the opponent king status after the move -----------*/
		// Check for check
		int numChecks = sskIsKingUnderCheck(*curBitPos, !SSK_GET_PIECE_COLOR(trav->pieceMoved), kFalse, NULL);
		if (numChecks > 0) {
            trav->opponentKingStatus = sskKingStatusCheck;
            trav->didUpdateOpponentKingStatus = kTrue;
            
            if (trav->next != NULL) {
                trav->next->selfKingStatus = sskKingStatusCheck;
                trav->next->didUpdateSelfKingStatus = kTrue;
            }
			
			// Check for Checkmate
			if (sskIsKingUnderCheckMate(*curBitPos, !SSK_GET_PIECE_COLOR(trav->pieceMoved), numChecks, (trav->next != NULL)?trav->next->enPassantTarget:-1)) {
				trav->opponentKingStatus = sskKingStatusCheckMate;
				trav->didUpdateOpponentKingStatus = kTrue;
				
				if (trav->next != NULL) {
					trav->next->selfKingStatus = sskKingStatusCheckMate;
					trav->next->didUpdateSelfKingStatus = kTrue;
				}
			}
        } else if (sskIsKingUnderStalemate(*curBitPos, !SSK_GET_PIECE_COLOR(trav->pieceMoved), (trav->next != NULL)?trav->next->enPassantTarget:-1)) {
			trav->opponentKingStatus = sskKingStatusStalemate;
			trav->didUpdateOpponentKingStatus = kTrue;
			
			if (trav->next != NULL) {
				trav->next->selfKingStatus = sskKingStatusStalemate;
				trav->next->didUpdateSelfKingStatus = kTrue;
			}
		} else {
			if (trav->next != NULL) {
				trav->next->selfKingStatus = sskKingStatusNone;
				trav->next->didUpdateSelfKingStatus = kTrue;
			}
		}
				
		// Fill move's piece placement string after the move
		sskFillPiecePlacementWithOffsetPosition(trav->piecePlacementAfterMove, curOffsetPos);
		
		trav = trav->next;
	}
	
	free(curBitPos);
	free(curOffsetPos);
		
	return 0;
}

kBool sskFillFromSquare(sskBitboardPosition bitboardPosition, sskOffsetPosition offsetPosition, sskMove * move, kBool * ambiguity) {
	sskChessSquare fromSquare;
	short reachablePieces;
	sskChessSquare reachablePiecesSquaresArray[8], pinnerSquare;	// Maximum of 8 reachable pieces from 8 directions
	kBool underPin;
	sskBitmap singlePieceBitmap, attackMap;
    int isCastlingOrEnpassantTarget;
	*ambiguity = kFalse;
    
    // Initialize Variables
    reachablePieces = 0;
    sskChessColor color = SSK_GET_PIECE_COLOR(move->pieceMoved);
    sskChessPiece pieceWithColor = move->pieceMoved;
    isCastlingOrEnpassantTarget = -1;
    
    // Check for special conditions: Castling and Enpassant
    if ((SSK_GET_GENERIC_PIECE_CODE(pieceWithColor) == sskChessPieceKing)) {
        switch (move->castlingType) {
            case sskCastlingTypeWKSide: {
                if (move->castlingStatus[0] == '-') return kFalse; // Castling not possible
                isCastlingOrEnpassantTarget = SSK_CHAR_2_FILE(tolower(move->castlingStatus[0]));
                move->fromSquare = SSK_SQUARE_IDX_FOR_FILE_RANK_CHAR('e', '1');
                move->toSquare = SSK_SQUARE_IDX_FOR_FILE_RANK_CHAR('g', '1');
                break;
            }
                
            case sskCastlingTypeWQSide: {
                if (move->castlingStatus[1] == '-') return kFalse; // Castling not possible
                isCastlingOrEnpassantTarget = SSK_CHAR_2_FILE(tolower(move->castlingStatus[1]));
                move->fromSquare = SSK_SQUARE_IDX_FOR_FILE_RANK_CHAR('e', '1');
                move->toSquare = SSK_SQUARE_IDX_FOR_FILE_RANK_CHAR('c', '1');
                break;
            }
                
            case sskCastlingTypeBKSide: {
                if (move->castlingStatus[2] == '-') return kFalse; // Castling not possible
                isCastlingOrEnpassantTarget = SSK_CHAR_2_FILE(tolower(move->castlingStatus[2]));
                move->fromSquare = SSK_SQUARE_IDX_FOR_FILE_RANK_CHAR('e', '8');
                move->toSquare = SSK_SQUARE_IDX_FOR_FILE_RANK_CHAR('g', '8');
                break;
            }
                
            case sskCastlingTypeBQSide: {
                if (move->castlingStatus[3] == '-') return kFalse; // Castling not possible
                isCastlingOrEnpassantTarget = SSK_CHAR_2_FILE(tolower(move->castlingStatus[3]));
                move->fromSquare = SSK_SQUARE_IDX_FOR_FILE_RANK_CHAR('e', '8');
                move->toSquare = SSK_SQUARE_IDX_FOR_FILE_RANK_CHAR('c', '8');
                break;
            }
                
            case sskCastlingTypeNone: {
                isCastlingOrEnpassantTarget = -1;
                break;
            }
        }
    } else if (SSK_GET_GENERIC_PIECE_CODE(pieceWithColor) == sskChessPiecePawn) {
        isCastlingOrEnpassantTarget = move->enPassantTarget;
    }
    
    // Obtain bitmap for the piece moved
    singlePieceBitmap = sskBitmapForPieceInBitboardPosition(bitboardPosition, move->pieceMoved);
    // Check if such a piece exists on the board
    if (singlePieceBitmap == SSK_EMPTY_BITMAP) return kFalse;
    
	/*------------ Reducing the search range -------------*/
	switch (SSK_GET_GENERIC_PIECE_CODE(pieceWithColor)) {
		case sskChessPiecePawn: {
			// If the piece is a pawn, we can reduce our search
			// to the pawns in file-1, file and file+1, and ranks
			singlePieceBitmap = singlePieceBitmap & (
				 (sskBitmapWithFileMask(SSK_GET_FILE_IDX(move->toSquare) - 1)) |
				 (sskBitmapWithFileMask(SSK_GET_FILE_IDX(move->toSquare))) |
				 (sskBitmapWithFileMask(SSK_GET_FILE_IDX(move->toSquare) + 1))
			 );
			if (color == sskChessColorWhite) {
				singlePieceBitmap = singlePieceBitmap & (
				 (sskBitmapWithRankMask(SSK_GET_RANK_IDX(move->toSquare) - 1)) |
				 ((SSK_GET_RANK_IDX(move->toSquare) == 3)?sskBitmapWithRankMask(SSK_GET_RANK_IDX(move->toSquare) - 2):SSK_EMPTY_BITMAP)
				 );
			} else {
				singlePieceBitmap = singlePieceBitmap & (
				 (sskBitmapWithRankMask(SSK_GET_RANK_IDX(move->toSquare) + 1)) |
				 ((SSK_GET_RANK_IDX(move->toSquare) == 4)?sskBitmapWithRankMask(SSK_GET_RANK_IDX(move->toSquare) + 2):SSK_EMPTY_BITMAP)
				);
			}
			break;
		}
		
		case sskChessPieceKnight: {
			// If the piece is a knight, we can reduce our search
			// to the 25 squares around toSquare
			singlePieceBitmap = singlePieceBitmap & (
			 (sskBitmapWithFileMask(SSK_GET_FILE_IDX(move->toSquare) - 2)) |
			 (sskBitmapWithFileMask(SSK_GET_FILE_IDX(move->toSquare) - 1)) |
			 (sskBitmapWithFileMask(SSK_GET_FILE_IDX(move->toSquare) + 1)) |
			 (sskBitmapWithFileMask(SSK_GET_FILE_IDX(move->toSquare) + 2)) |
			 (sskBitmapWithRankMask(SSK_GET_RANK_IDX(move->toSquare) - 2)) |
			 (sskBitmapWithRankMask(SSK_GET_RANK_IDX(move->toSquare) - 1)) |
			 (sskBitmapWithRankMask(SSK_GET_RANK_IDX(move->toSquare) + 1)) |
			 (sskBitmapWithRankMask(SSK_GET_RANK_IDX(move->toSquare) + 2))
			 );
			break;
		}
		
		case sskChessPieceRook: {
			// If the piece is a rook, we can reduce our search
			// to same file/rank as that of toSquare
			singlePieceBitmap = singlePieceBitmap & (
			 (sskBitmapWithFileMask(SSK_GET_FILE_IDX(move->toSquare))) |
			 (sskBitmapWithRankMask(SSK_GET_RANK_IDX(move->toSquare)))
			);
			break;
		}
			
		case sskChessPieceBishop: {
			// If the piece is a rook, we can reduce our search
			// to same file/rank as that of toSquare
			singlePieceBitmap = singlePieceBitmap & (
			 sskBitmapWithNorthWestDiagonal(move->toSquare) | sskBitmapWithSouthEastDiagonal(move->toSquare) | sskBitmapWithNorthEastDiagonal(move->toSquare) | sskBitmapWithSouthWestDiagonal(move->toSquare)
			);
			break;
		}
	}
	/*------------ Reducing the search range -------------*/
	
	// If the piece is a pawn, we can reduce our search
	// to the pawns in file-1, file and file+1
	
    // Iterate to find all pieces of the given type and check if it can reach to
    while (singlePieceBitmap) {
        fromSquare = sskFirstOneIndex(singlePieceBitmap);
        singlePieceBitmap = singlePieceBitmap & SSK_BITMAP_UNSET_SQUARE_IDX(fromSquare);
        
        if (sskIsSquareReachable(bitboardPosition, fromSquare, move->toSquare, isCastlingOrEnpassantTarget, &attackMap) == kTrue) {
            // If piece is a king, we can skip checking for pins.
            if (SSK_GET_GENERIC_PIECE_CODE(pieceWithColor) != sskChessPieceKing) {
                underPin = sskIsSquarePinned(bitboardPosition, offsetPosition, fromSquare,
                                    (color == sskChessColorWhite)?sskFirstOneIndex(bitboardPosition.wKing):sskFirstOneIndex(bitboardPosition.bKing),
                                    color, &pinnerSquare);
                // We count the piece on conditions:
                //  1) It is not reachable.
                //  2) It captures the pinnerPiece.
                //  3) It maintains the pin.
                if (!underPin) {
                    reachablePiecesSquaresArray[reachablePieces++] = fromSquare;	// 1
                } else if (underPin) {
                    if (attackMap & SSK_BITMAP_SET_SQUARE_IDX(pinnerSquare)) {
						reachablePiecesSquaresArray[reachablePieces++] = fromSquare; // 2
					} else {
						// 3 - Check if the pinner square can reach the king after move
						sskOffsetPosition afterMovePos = sskCopyOffsetPosition(offsetPosition);
						afterMovePos[fromSquare] = sskChessPieceNone;
						afterMovePos[move->toSquare] = move->pieceMoved;
						sskBitmap tempAttackMap;
						sskBitboardPosition * tempBitboardPosition = sskOffsetPositionToBitboardPosition(afterMovePos);
						if(!sskIsSquareReachable(*tempBitboardPosition, pinnerSquare, (color == sskChessColorWhite)?sskFirstOneIndex(bitboardPosition.wKing):sskFirstOneIndex(bitboardPosition.bKing),-1, &tempAttackMap)) {
							reachablePiecesSquaresArray[reachablePieces++] = fromSquare;
						}
						free(tempBitboardPosition);
						free(afterMovePos);
					}
                }
            } else {
                reachablePiecesSquaresArray[reachablePieces++] = fromSquare;
            }
        }
    }
    
    if (reachablePieces == 0) return kFalse; // None of the pieces could reach the destination
    else {
        *ambiguity = sskCheckMoveAmbiguityAndFillFromSquare(offsetPosition, reachablePiecesSquaresArray, reachablePieces, move);
    }
    
	return kTrue;
}

sskBitboardPosition * sskCheckLegal(sskBitboardPosition bitboardPosition, sskMove * move, kBool moveWasPseudoLegalChecked) {
    sskOffsetPosition offsetPosition = sskBitboardPositionToOffsetPosition(bitboardPosition);
    sskBitboardPosition * verificationBitboard = NULL;
    unsigned short numChecks = 0;
	
	if (!moveWasPseudoLegalChecked) {
		sskBitmap attackMap;
		kBool underPin, pieceCanReach = kFalse;
		sskChessSquare pinnerSquare;
		
		if (sskIsSquareReachable(bitboardPosition, move->fromSquare, move->toSquare, (move->castlingType != sskCastlingTypeNone)?move->castlingType:(move->enPassantTarget != 0)?move->enPassantTarget:-1, &attackMap) == kTrue) {
            // If piece is a king, we can skip checking for pins.
            if (SSK_GET_GENERIC_PIECE_CODE(move->pieceMoved) != sskChessPieceKing) {
                underPin = sskIsSquarePinned(bitboardPosition, offsetPosition, move->fromSquare,
											 (SSK_GET_PIECE_COLOR(move->pieceMoved) == sskChessColorWhite)?sskFirstOneIndex(bitboardPosition.wKing):sskFirstOneIndex(bitboardPosition.bKing),
											 SSK_GET_PIECE_COLOR(move->pieceMoved), &pinnerSquare);
                // We count the piece on conditions:
                //  1) It is not reachable.
                //  2) It captures the pinnerPiece.
                //  3) It maintains the pin.
                if (!underPin) {
                    pieceCanReach = kTrue;
                } else {
					if (attackMap & SSK_BITMAP_SET_SQUARE_IDX(pinnerSquare)) {
						pieceCanReach = kTrue; // 2
					} else {
						// 3 - Check if the pinner square can reach the king after move
						sskOffsetPosition afterMovePos = sskCopyOffsetPosition(offsetPosition);
						afterMovePos[move->fromSquare] = sskChessPieceNone;
						afterMovePos[move->toSquare] = move->pieceMoved;
						sskBitmap tempAttackMap;
						if(!sskIsSquareReachable(*sskOffsetPositionToBitboardPosition(afterMovePos), pinnerSquare, (SSK_GET_PIECE_COLOR(move->pieceMoved) == sskChessColorWhite)?sskFirstOneIndex(bitboardPosition.wKing):sskFirstOneIndex(bitboardPosition.bKing),-1, &tempAttackMap)) {
							pieceCanReach = kTrue;
							free(afterMovePos);
						}
					}
				}
            } else {
				pieceCanReach = kTrue;
            }
        }
		
		if (!pieceCanReach) {
			free(offsetPosition);
			return NULL;
		}
	}
    
    // If move is castling, verify that the king is not under check and
    // all the squares between king and castling detination are not under check.
    if (move->castlingType != sskCastlingTypeNone) {
        if (move->didUpdateSelfKingStatus && (move->selfKingStatus == sskKingStatusCheck || move->selfKingStatus == sskKingStatusCheckMate)) {
            free(offsetPosition);
            return NULL;
        } else if (sskIsKingUnderCheck(bitboardPosition, SSK_GET_PIECE_COLOR(move->pieceMoved), kFalse, NULL) > 0) {
            free(offsetPosition);
            return NULL;
        }
        
        // verify if each square between king and the castling destination square
        // is not under check by any opponent piece.
        sskOffsetPosition castleEvalOffsetPosition = sskCopyOffsetPosition(offsetPosition);
        sskBitboardPosition * castleEvalBitboardPosition = NULL;
        sskChessSquare i = sskFirstOneIndex(sskBitmapForPieceInBitboardPosition(bitboardPosition, move->pieceMoved)); // Mark current king square
    
        switch (move->castlingType) {
            case sskCastlingTypeWKSide: {
                for (i = i + 1; i <= SSK_SQUARE_IDX_FOR_FILE_RANK_CHAR('g', '1'); i++) {
                    castleEvalOffsetPosition[i - 1] = sskChessPieceNone;   // Vacate the left square and occupy next square
                    castleEvalOffsetPosition[i] = sskChessPieceWKing;      // Occupy current square with White King
                    castleEvalBitboardPosition = sskOffsetPositionToBitboardPosition(castleEvalOffsetPosition);   // Obtain position in bitboard format from adjusted offset position.
                    numChecks = sskIsKingUnderCheck(*castleEvalBitboardPosition, sskChessColorWhite, kTrue, NULL);

                    free(castleEvalBitboardPosition);
                    if (numChecks > 0) break;
                }
                break;
            }
                
            case sskCastlingTypeWQSide: {
                
                for (i = i - 1; i >= SSK_SQUARE_IDX_FOR_FILE_RANK_CHAR('c', '1'); i--) {
                    castleEvalOffsetPosition[i + 1] = sskChessPieceNone;   // Vacate the right square and occupy next square
                    castleEvalOffsetPosition[i] = sskChessPieceWKing;      // Occupy current square with White King
                    castleEvalBitboardPosition = sskOffsetPositionToBitboardPosition(castleEvalOffsetPosition);   // Obtain position in bitboard format from adjusted offset position.
                    numChecks = sskIsKingUnderCheck(*castleEvalBitboardPosition, sskChessColorWhite, kTrue, NULL);
                    
                    free(castleEvalBitboardPosition);
                    if (numChecks > 0) break;
                }
                break;
            }
                
            case sskCastlingTypeBKSide: {
                for (i = i + 1; i <= SSK_SQUARE_IDX_FOR_FILE_RANK_CHAR('g', '8'); i++) {
                    castleEvalOffsetPosition[i - 1] = sskChessPieceNone;   // Vacate the left square and occupy next square
                    castleEvalOffsetPosition[i] = sskChessPieceBKing;      // Occupy current square with Black King
                    castleEvalBitboardPosition = sskOffsetPositionToBitboardPosition(castleEvalOffsetPosition);   // Obtain position in bitboard format from adjusted offset position.
                    numChecks = sskIsKingUnderCheck(*castleEvalBitboardPosition, sskChessColorBlack, kTrue, NULL);
                    
                    free(castleEvalBitboardPosition);
                    if (numChecks > 0) break;
                }
                break;
            }
                
            case sskCastlingTypeBQSide: {
                for (i = i - 1; i >= SSK_SQUARE_IDX_FOR_FILE_RANK_CHAR('c', '8'); i--) {
                    castleEvalOffsetPosition[i + 1] = sskChessPieceNone;   // Vacate the left square and occupy next square
                    castleEvalOffsetPosition[i] = sskChessPieceBKing;      // Occupy current square with Black King
                    castleEvalBitboardPosition = sskOffsetPositionToBitboardPosition(castleEvalOffsetPosition);   // Obtain position in bitboard format from adjusted offset position.
                    numChecks = sskIsKingUnderCheck(*castleEvalBitboardPosition, sskChessColorBlack, kTrue, NULL);
                    
                    free(castleEvalBitboardPosition);
                    if (numChecks > 0) break;
                }
                break;
            }
        }
        
        free(castleEvalOffsetPosition);
        
        if (numChecks > 0) {
            free(offsetPosition);
            return NULL;
        }
    }
    
    // Update the move in offset position.
    offsetPosition[move->fromSquare] = sskChessPieceNone;	// vacate fromSquare
    offsetPosition[move->toSquare] = move->pieceMoved;	// occupy toSquare
    
    // Special Case: Castling
    if (SSK_GET_GENERIC_PIECE_CODE(move->pieceMoved) == sskChessPieceKing) {
        switch (move->castlingType) {
            case sskCastlingTypeWKSide: {
                offsetPosition[ SSK_SQUARE_IDX_FOR_FILE_RANK_CHAR(move->castlingStatus[0], '1') ] = sskChessPieceNone;	// vacate rook square
                offsetPosition[ SSK_SQUARE_IDX_FOR_FILE_RANK_IDX(5, 0) ] = sskChessPieceWRook;								// occupy rook square
                break;
            }
            case sskCastlingTypeWQSide: {
                offsetPosition[ SSK_SQUARE_IDX_FOR_FILE_RANK_CHAR(move->castlingStatus[1], '1') ] = sskChessPieceNone;	// vacate rook square
                offsetPosition[ SSK_SQUARE_IDX_FOR_FILE_RANK_IDX(3, 0) ] = sskChessPieceWRook;								// occupy rook square
                break;
            }
            case sskCastlingTypeBKSide: {
                offsetPosition[ SSK_SQUARE_IDX_FOR_FILE_RANK_CHAR(move->castlingStatus[2], '8') ] = sskChessPieceNone;	// vacate rook square
                offsetPosition[ SSK_SQUARE_IDX_FOR_FILE_RANK_IDX(5, 7) ] = sskChessPieceBRook;								// occupy rook square
                break;
            }
            case sskCastlingTypeBQSide: {
                offsetPosition[ SSK_SQUARE_IDX_FOR_FILE_RANK_CHAR(move->castlingStatus[3], '8') ] = sskChessPieceNone;	// vacate rook square
                offsetPosition[ SSK_SQUARE_IDX_FOR_FILE_RANK_IDX(3, 7) ] = sskChessPieceBRook;								// occupy rook square
                break;
            }
        }
        offsetPosition[move->toSquare] = move->pieceMoved;    // Re-occupy toSquare with king in case it was vacated
    }
    
    // Special Case: En-passant
    if ((move->pieceMoved == sskChessPieceWPawn) && (move->toSquare == move->enPassantTarget)) {
        offsetPosition[move->enPassantTarget - 8] = sskChessPieceNone;	// remove the black pawn on 5th rank
    }
    if ((move->pieceMoved == sskChessPieceBPawn) && (move->toSquare == move->enPassantTarget)) {
        offsetPosition[move->enPassantTarget + 8] = sskChessPieceNone;	// remove the white pawn on 4th rank
    }
	
	// Special Case: Promotion
	if ((SSK_GET_GENERIC_PIECE_CODE(move->pieceMoved) == sskChessPiecePawn) && move->promotedPiece != sskChessPieceNone) {
		offsetPosition[move->toSquare] = (SSK_GET_PIECE_COLOR(move->pieceMoved) << 3) | move->promotedPiece;	// Place promoted piece on toSquare
	}
    
    // Update bitboard
    verificationBitboard = sskOffsetPositionToBitboardPosition(offsetPosition);
    numChecks = sskIsKingUnderCheck(*verificationBitboard, SSK_GET_PIECE_COLOR(move->pieceMoved), kFalse, NULL);
    
    free(offsetPosition);
    
    if (numChecks > 0) {
        free(verificationBitboard);
        return NULL;
    }

    return verificationBitboard;
}

unsigned short sskIsKingUnderCheck(sskBitboardPosition bitboardPosition, sskChessColor kingColor, kBool shouldIncludeKing, sskChessSquare * checkingPieceSquare) {
	unsigned short numChecks = 0;
	sskChessPiece i;
	sskChessSquare fromSquare, toSquare;
	sskBitmap pieceBitmap;
	
	// Set the king's square as toSquare.
	toSquare = sskFirstOneIndex(sskBitmapForPieceInBitboardPosition(bitboardPosition, (kingColor == sskChessColorWhite)?sskChessPieceWKing:sskChessPieceBKing));
	
	// Iterate all the opponent pieces(king if needed) and find if they can reach the king.
	for (i = ((kingColor == sskChessColorWhite)?sskChessPieceBPawn:sskChessPieceWPawn);
		 i <= ((kingColor == sskChessColorWhite)?sskChessPieceBKnight:sskChessPieceWKnight);
		 i++) {
		if (SSK_GET_GENERIC_PIECE_CODE(i) == sskChessPieceKing && !shouldIncludeKing) continue;	// Skip King if required.
		
		pieceBitmap = sskBitmapForPieceInBitboardPosition(bitboardPosition, i);
		while (pieceBitmap) {
			fromSquare = sskFirstOneIndex(pieceBitmap);
			pieceBitmap = pieceBitmap & SSK_BITMAP_UNSET_SQUARE_IDX(fromSquare);
			if (sskIsSquareReachable(bitboardPosition, fromSquare, toSquare, -1, NULL)) {
				if (checkingPieceSquare != NULL) *checkingPieceSquare = fromSquare;
				numChecks++;
			}
		}
	}
	
	return numChecks;
}

kBool sskIsKingUnderCheckMate(sskBitboardPosition bitboardPosition, sskChessColor kingColor, int numChecks, int enpassantTarget) {
	sskChessSquare checkingPieceSquare = 0;
	sskBitmap checkPathBitmap = SSK_EMPTY_BITMAP, kingBitmap = SSK_EMPTY_BITMAP, pieceBitmap = SSK_EMPTY_BITMAP, pieceAttackBitmap = SSK_EMPTY_BITMAP;
	kBool pieceCanBlock = kFalse, underPin, kingCanEscape;
	int i, j;
	sskChessSquare fromSquare;
	
	if (numChecks == 0) return kFalse;
	numChecks = sskIsKingUnderCheck(bitboardPosition, kingColor, kTrue, &checkingPieceSquare);
    if (numChecks == 0) return kFalse;
	
	// Check if king has an escape square
	kingCanEscape = sskCanKingEscape(bitboardPosition, kingColor);
	
	if (kingCanEscape) return kFalse;
		
	// When just 1 piece is issuing check,
	// see if any piece can block the check.
	
	
    if (!kingCanEscape && (numChecks == 1)) {
		kingBitmap = sskBitmapForPieceInBitboardPosition(bitboardPosition, (kingColor << 3) | sskChessPieceKing);
        sskIsSquareReachable(bitboardPosition, checkingPieceSquare, sskFirstOneIndex(kingBitmap), -1, &checkPathBitmap);
		
		// modify checkpath bitmap to set the checkingPieceSquare and unset king's square
		checkPathBitmap = checkPathBitmap | SSK_BITMAP_SET_SQUARE_IDX(checkingPieceSquare);
		checkPathBitmap = checkPathBitmap & (~kingBitmap);
		
		pieceCanBlock = kFalse;
		
		// Iterate all kingColor pieces to verify if at least can reach a square in checkpath.
		while (checkPathBitmap) {	// Outer Loop Iterates Squares in check path map
			i = sskFirstOneIndex(checkPathBitmap);
			checkPathBitmap = checkPathBitmap & SSK_BITMAP_UNSET_SQUARE_IDX(i);
			
			for (j = ((kingColor << 3) | sskChessPiecePawn);
				 j <= ((kingColor << 3) | sskChessPieceKnight); j++) {	// Inner Loop Iterates all pieces
				if (SSK_GET_GENERIC_PIECE_CODE(j) == sskChessPieceKing) continue;	// Skip King
				
				pieceBitmap = sskBitmapForPieceInBitboardPosition(bitboardPosition, j);
				while (pieceBitmap) {
					fromSquare = sskFirstOneIndex(pieceBitmap);
					pieceBitmap = pieceBitmap & SSK_BITMAP_UNSET_SQUARE_IDX(fromSquare);
					
					if (sskIsSquareReachable(bitboardPosition, fromSquare, i, enpassantTarget, &pieceAttackBitmap)) {
						// Verify Pin Condition
						sskOffsetPosition offsetPosition = sskBitboardPositionToOffsetPosition(bitboardPosition);
						sskChessSquare pinCausingPieceSquare;
						underPin = sskIsSquarePinned(bitboardPosition, offsetPosition, fromSquare, sskFirstOneIndex(kingBitmap), kingColor, &pinCausingPieceSquare);
						free(offsetPosition);
						if (!underPin) {
							pieceCanBlock = kTrue;
							break;
						}
					}
				}
				
				if (pieceCanBlock) break;
			}
			
			if (pieceCanBlock) break;
		}
    }    
    
	// Else condition: More than 1 piece is issuing
	// check and king can't escape -> Checkmate
	return !pieceCanBlock;
}

kBool sskIsKingUnderStalemate(sskBitboardPosition bitboardPosition, sskChessColor kingColor, int enpassantTarget) {
	if (sskCanKingEscape(bitboardPosition, kingColor)) return kFalse;
	int i;
	kBool legalMoveExists;
	sskBitmap pieceBitmap, pieceAttacksBitmap, pieceAllAttacksBitmap;
	kBool underPin;
	sskChessSquare fromSquare, pinnerSquare, kingSquare, toSquare;
	sskOffsetPosition offsetPosition = NULL;
	
	kingSquare = sskFirstOneIndex(sskBitmapForPieceInBitboardPosition(bitboardPosition, ((kingColor << 3) | sskChessPieceKing)));
	legalMoveExists = kFalse;
	
	// Look for atleast a single legal move of kingColor pieces (except king)
	for (i = ((kingColor << 3) | sskChessPiecePawn); i <= ((kingColor << 3) | sskChessPieceKnight); i++) {	// Iterate pieces
		if (SSK_GET_GENERIC_PIECE_CODE(i) == sskChessPieceKing) continue;	// Skip King
		
		pieceBitmap = sskBitmapForPieceInBitboardPosition(bitboardPosition, i);
		while (pieceBitmap) {
			fromSquare = sskFirstOneIndex(pieceBitmap);
			pieceBitmap = pieceBitmap & SSK_BITMAP_UNSET_SQUARE_IDX(fromSquare);
			
            pieceAllAttacksBitmap = sskBitmapForAllPieceAttacksInBitboardPosition(bitboardPosition, i, fromSquare);
            
			// Compute the squares that the piece can probably reach.
			while (pieceAllAttacksBitmap) {
                toSquare = sskFirstOneIndex(pieceAllAttacksBitmap);
                pieceAllAttacksBitmap = pieceAllAttacksBitmap & SSK_BITMAP_UNSET_SQUARE_IDX(toSquare);
                
                if (sskIsSquareReachable(bitboardPosition, fromSquare, toSquare, enpassantTarget, &pieceAttacksBitmap)) {
					offsetPosition = sskBitboardPositionToOffsetPosition(bitboardPosition);
					underPin = sskIsSquarePinned(bitboardPosition, offsetPosition, fromSquare, kingSquare, kingColor, &pinnerSquare);
					free(offsetPosition);
					if (!underPin) {
						legalMoveExists = kTrue;
						break;
					} else if (underPin && (pieceAttacksBitmap & SSK_BITMAP_SET_SQUARE_IDX(pinnerSquare))) {
						legalMoveExists = kTrue;
						break;
					}
				}
            }
            
			if (legalMoveExists) break;
		}
		
		if (legalMoveExists) break;
	}
	
	return !legalMoveExists;
}

kBool sskCanKingEscape(sskBitboardPosition bitboardPosition, sskChessColor kingColor) {
	// Check to see if king has an escape square.
    sskOffsetPosition checkEvalOffsetPosition = sskBitboardPositionToOffsetPosition(bitboardPosition);
    sskBitboardPosition * checkEvalBitboardPosition = NULL;
    int i, prevSquare = -1;
    sskChessPiece prevPiece = sskChessPieceNone;
	
    // Obtain reachable squares for king in a bitmap.
    sskChessSquare kingSquare = sskFirstOneIndex(sskBitmapForPieceInBitboardPosition(bitboardPosition, (kingColor == sskChessColorWhite)?sskChessPieceWKing:sskChessPieceBKing));
    sskBitmap kingBitmap = sskBitmapWithKingReach(kingSquare);
    kingBitmap = (kingBitmap ^ ((kingColor == sskChessColorWhite)?bitboardPosition.wOccupied:bitboardPosition.bOccupied)) & sskBitmapWithKingReach(kingSquare);
	kingBitmap = kingBitmap & SSK_BITMAP_UNSET_SQUARE_IDX(kingSquare);	// Exclude the kingSquare.
	kBool canEscape = kFalse;
    
    // Put the king on each reachable square and see if its under check
    // before going to the next state, restore.
    for (i = sskFirstOneIndex(kingBitmap); kingBitmap != SSK_EMPTY_BITMAP; i = sskFirstOneIndex(kingBitmap)) {
        if (prevSquare != -1) {
            checkEvalOffsetPosition[prevSquare] = prevPiece;
        }
        
        kingBitmap = kingBitmap & SSK_BITMAP_UNSET_SQUARE_IDX(i);
        
        // Store the current square being set
        // for restoration at next iteration.
        prevPiece = checkEvalOffsetPosition[i];
        prevSquare = i;
		
        checkEvalOffsetPosition[i] = (kingColor << 3) | sskChessPieceKing; // Place king on reachable square.
        checkEvalOffsetPosition[kingSquare] = sskChessPieceNone;
		
        checkEvalBitboardPosition = sskOffsetPositionToBitboardPosition(checkEvalOffsetPosition);
        if (sskIsKingUnderCheck(*checkEvalBitboardPosition, kingColor, kTrue, NULL) == 0) canEscape = kTrue;
        free(checkEvalBitboardPosition);
		
		if (canEscape) break;
	}

	return canEscape;
}

kBool sskIsSquareReachable(sskBitboardPosition bitboardPosition, sskChessSquare fromSquare, sskChessSquare toSquare, int isCastlingOrEnpassantTarget, sskBitmap * attackMap) {
	sskOffsetPosition offsetPosition = sskBitboardPositionToOffsetPosition(bitboardPosition);
	sskChessPiece piece = SSK_GET_GENERIC_PIECE_CODE(offsetPosition[fromSquare]);
	sskChessColor color = SSK_GET_PIECE_COLOR(offsetPosition[fromSquare]);
    sskBitmap attacks = SSK_EMPTY_BITMAP;
	
	// No piece exists on fromSquare
	if (piece == sskChessPieceNone) return kFalse;
    
	// Same color piece exists on toSquare or castling - reject
	if (SSK_GET_GENERIC_PIECE_CODE(offsetPosition[toSquare]) != sskChessPieceNone &&
		SSK_GET_PIECE_COLOR(offsetPosition[toSquare]) == color) {
		// Castling as special case
		if ((fromSquare == toSquare) &&
			(piece == sskChessPieceKing) &&
			(isCastlingOrEnpassantTarget != -1)) {
			// continue execution
		} else if(SSK_GET_FILE_IDX(toSquare) == isCastlingOrEnpassantTarget) {
			// continue execution
		} else {
			free(offsetPosition);
			return kFalse;
		}
	}
	
	attacks = sskBitmapForSpecificPieceAttacksInBitboardPosition(bitboardPosition, offsetPosition[fromSquare], fromSquare, toSquare, isCastlingOrEnpassantTarget);
	
	free(offsetPosition);
    if(attackMap != NULL) *attackMap = attacks;
	if (attacks & SSK_BITMAP_SET_SQUARE_IDX(toSquare)) return kTrue;
	
	return kFalse;
}


kBool sskIsSquarePinned(sskBitboardPosition bitboardPosition, sskOffsetPosition offsetPosition, sskChessSquare pinnedSquare, sskChessSquare behindSquare, sskChessColor color, sskChessSquare * pinCausingPieceSquare) {
	if (pinnedSquare == behindSquare) {
		return kFalse;
	}
	
	short pinnedFile = SSK_GET_FILE_IDX(pinnedSquare);
	short pinnedRank = SSK_GET_RANK_IDX(pinnedSquare);
	short behindFile = SSK_GET_FILE_IDX(behindSquare);
	short behindRank = SSK_GET_RANK_IDX(behindSquare);
	
	sskBitmap behindToPinned, pinnedToPinner, blockers;
	sskChessPiece pinner;
	
	// Find the relative position of pieceBehindSquare to pinnedSquare.
	if (pinnedRank == behindRank) { // Same Rank
		if (pinnedFile > behindFile) {	// East
			// Obtain the mask for region: behind<->pinned and identify blockers there.
			behindToPinned = (sskBitmapWithEastRank(pinnedSquare) ^ sskBitmapWithEastRank(behindSquare)) & SSK_BITMAP_UNSET_SQUARE_IDX(pinnedSquare);
			blockers = behindToPinned & bitboardPosition.occupied;
			
			// A piece exists in the region: behind<->pinned, so no pin!
			if (blockers) { return kFalse; }
			
			// Obtain mask for region: pinned<->rest of the rank
			pinnedToPinner = (sskBitmapWithEastRank(pinnedSquare));
			blockers = pinnedToPinner & bitboardPosition.occupied;
			
			// There is no piece to create a pin
			if (blockers == 0) return kFalse;
			
			*pinCausingPieceSquare = sskFirstOneIndex(blockers);
			pinner = offsetPosition[*pinCausingPieceSquare];
			
			// The blocker is the same color as of the pinned piece
			if (SSK_GET_PIECE_COLOR(pinner) == color) {	return kFalse; }
			
			if (SSK_GET_GENERIC_PIECE_CODE(pinner) == sskChessPieceRook || SSK_GET_GENERIC_PIECE_CODE(pinner) == sskChessPieceQueen) {
				return kTrue;
			} else {
				return kFalse;
			}
		} else if (pinnedFile < behindFile) {	// West
			// Obtain mask for region: pinned<->behind and identify blockers there.
			behindToPinned = (sskBitmapWithWestRank(pinnedSquare) ^ sskBitmapWithWestRank(behindSquare) & SSK_BITMAP_UNSET_SQUARE_IDX(pinnedSquare));
			blockers = behindToPinned & bitboardPosition.occupied;
			
			// A piece exists in the region: pinned<->behind, so no pin!
			if (blockers) { return kFalse; }
			
			// Obtain mask for region: rest of rank<->pinned
			pinnedToPinner = (sskBitmapWithWestRank(pinnedSquare));
			blockers = pinnedToPinner & bitboardPosition.occupied;
			
			// There is no piece to create a pin
			if (blockers == 0) { return kFalse; }
			
			*pinCausingPieceSquare = sskLastOneIndex(blockers);
			pinner = offsetPosition[*pinCausingPieceSquare];
			
			// The blocker is the same color as of the pinned piece
			if (SSK_GET_PIECE_COLOR(pinner) == color) { return kFalse; }
			
			if (SSK_GET_GENERIC_PIECE_CODE(pinner) == sskChessPieceRook || SSK_GET_GENERIC_PIECE_CODE(pinner) == sskChessPieceQueen) {
				return kTrue;
			} else {
				return kFalse;
			}
		}		
	} else if (pinnedFile == behindFile) {	// Same File
		if (pinnedRank > behindRank) {	// North
			// Obtain the mask for region: behind<->pinned and identify blockers there.
			behindToPinned = (sskBitmapWithNorthFile(pinnedSquare) ^ sskBitmapWithNorthFile(behindSquare)) & SSK_BITMAP_UNSET_SQUARE_IDX(pinnedSquare);
			blockers = behindToPinned & bitboardPosition.occupied;
			
			// A piece exists in the region: behind<->pinned, so no pin!
			if (blockers) { return kFalse; }
			
			// Obtain mask for region: pinned<->rest of the rank
			pinnedToPinner = (sskBitmapWithNorthFile(pinnedSquare));
			blockers = pinnedToPinner & bitboardPosition.occupied;
			
			// There is no piece to create a pin
			if (blockers == 0) return kFalse;
			
			*pinCausingPieceSquare = sskFirstOneIndex(blockers);
			pinner = offsetPosition[*pinCausingPieceSquare];
			
			// The blocker is the same color as of the pinned piece
			if (SSK_GET_PIECE_COLOR(pinner) == color) {	return kFalse; }
			
			if (SSK_GET_GENERIC_PIECE_CODE(pinner) == sskChessPieceRook || SSK_GET_GENERIC_PIECE_CODE(pinner) == sskChessPieceQueen) {
				return kTrue;
			} else {
				return kFalse;
			}
		} else if (pinnedRank < behindRank) {	// South
			// Obtain mask for region: pinned<->behind and identify blockers there.
			behindToPinned = (sskBitmapWithSouthFile(pinnedSquare) ^ sskBitmapWithSouthFile(behindSquare) & SSK_BITMAP_UNSET_SQUARE_IDX(pinnedSquare));
			blockers = behindToPinned & bitboardPosition.occupied;
			
			// A piece exists in the region: pinned<->behind, so no pin!
			if (blockers) { return kFalse; }
			
			// Obtain mask for region: rest of rank<->pinned
			pinnedToPinner = (sskBitmapWithSouthFile(pinnedSquare));
			blockers = pinnedToPinner & bitboardPosition.occupied;
			
			// There is no piece to create a pin
			if (blockers == 0) { return kFalse; }
			
			*pinCausingPieceSquare = sskLastOneIndex(blockers);
			pinner = offsetPosition[*pinCausingPieceSquare];
			
			// The blocker is the same color as of the pinned piece
			if (SSK_GET_PIECE_COLOR(pinner) == color) { return kFalse; }
			
			if (SSK_GET_GENERIC_PIECE_CODE(pinner) == sskChessPieceRook || SSK_GET_GENERIC_PIECE_CODE(pinner) == sskChessPieceQueen) {
				return kTrue;
			} else {
				return kFalse;
			}
		}
	} else if ((pinnedFile < behindFile && pinnedRank < behindRank)
			   && ((behindFile - pinnedFile) == (behindRank - pinnedRank))
			   ) {	// Behind Square is in North East Diagonal
		//printf("\n Behind Square NE Diagonal");
		// Obtain mask for region: pinned<->behind and identify blockers there.
		behindToPinned = (sskBitmapWithSouthWestDiagonal(pinnedSquare) ^ sskBitmapWithSouthWestDiagonal(behindSquare) & SSK_BITMAP_UNSET_SQUARE_IDX(pinnedSquare));
		blockers = behindToPinned & bitboardPosition.occupied;
		
		// A piece exists in the region: pinned<->behind, so no pin!
		if (blockers) { return kFalse; }
		
		// Obtain mask for region: rest of rank<->pinned
		pinnedToPinner = (sskBitmapWithSouthWestDiagonal(pinnedSquare));
		blockers = pinnedToPinner & bitboardPosition.occupied;
		
		// There is no piece to create a pin
		if (blockers == 0) { return kFalse; }
		
		*pinCausingPieceSquare = sskLastOneIndex(blockers);
		pinner = offsetPosition[*pinCausingPieceSquare];
		
		// The blocker is the same color as of the pinned piece
		if (SSK_GET_PIECE_COLOR(pinner) == color) { return kFalse; }
		
		if (SSK_GET_GENERIC_PIECE_CODE(pinner) == sskChessPieceBishop || SSK_GET_GENERIC_PIECE_CODE(pinner) == sskChessPieceQueen) {
			return kTrue;
		} else {
			return kFalse;
		}
	} else if ((pinnedFile > behindFile && pinnedRank < behindRank)
			   && ((pinnedFile - behindFile) == (behindRank - pinnedRank))
			   ) {	// Behind Square is in North West Diagonal
		//printf("\n Behind Square NW Diagonal");
		// Obtain mask for region: pinned<->behind and identify blockers there.
		behindToPinned = (sskBitmapWithSouthEastDiagonal(pinnedSquare) ^ sskBitmapWithSouthEastDiagonal(behindSquare) & SSK_BITMAP_UNSET_SQUARE_IDX(pinnedSquare));
		blockers = behindToPinned & bitboardPosition.occupied;
		
		// A piece exists in the region: pinned<->behind, so no pin!
		if (blockers) { return kFalse; }
		
		// Obtain mask for region: rest of rank<->pinned
		pinnedToPinner = (sskBitmapWithSouthEastDiagonal(pinnedSquare));
		blockers = pinnedToPinner & bitboardPosition.occupied;
		
		// There is no piece to create a pin
		if (blockers == 0) { return kFalse; }
		
		*pinCausingPieceSquare = sskLastOneIndex(blockers);
		pinner = offsetPosition[*pinCausingPieceSquare];
		
		// The blocker is the same color as of the pinned piece
		if (SSK_GET_PIECE_COLOR(pinner) == color) { return kFalse; }
		
		if (SSK_GET_GENERIC_PIECE_CODE(pinner) == sskChessPieceBishop || SSK_GET_GENERIC_PIECE_CODE(pinner) == sskChessPieceQueen) {
			return kTrue;
		} else {
			return kFalse;
		}
	} else if ((pinnedFile > behindFile && pinnedRank > behindRank)
			   && ((pinnedFile - behindFile) == (pinnedRank  - behindRank))
			   ) {	// Behind Square is in South East Diagonal
		//printf("\n Behind Square SE Diagonal");
		// Obtain mask for region: pinned<->behind and identify blockers there.
		behindToPinned = (sskBitmapWithNorthEastDiagonal(pinnedSquare) ^ sskBitmapWithNorthEastDiagonal(behindSquare) & SSK_BITMAP_UNSET_SQUARE_IDX(pinnedSquare));
		blockers = behindToPinned & bitboardPosition.occupied;
		
		// A piece exists in the region: pinned<->behind, so no pin!
		if (blockers) { return kFalse; }
		
		// Obtain mask for region: rest of rank<->pinned
		pinnedToPinner = (sskBitmapWithNorthEastDiagonal(pinnedSquare));
		blockers = pinnedToPinner & bitboardPosition.occupied;
		
		// There is no piece to create a pin
		if (blockers == 0) { return kFalse; }
		
		*pinCausingPieceSquare = sskFirstOneIndex(blockers);
		pinner = offsetPosition[*pinCausingPieceSquare];
		
		// The blocker is the same color as of the pinned piece
		if (SSK_GET_PIECE_COLOR(pinner) == color) { return kFalse; }
		
		if (SSK_GET_GENERIC_PIECE_CODE(pinner) == sskChessPieceBishop || SSK_GET_GENERIC_PIECE_CODE(pinner) == sskChessPieceQueen) {
			return kTrue;
		} else {
			return kFalse;
		}		
	} else if ((pinnedFile < behindFile && pinnedRank > behindRank)
			   && ((behindFile - pinnedFile) == (pinnedRank - behindRank))
			   ) {	// Behind Square is in South West Diagonal
		//printf("\n Behind Square SW Diagonal");
		// Obtain mask for region: pinned<->behind and identify blockers there.
		behindToPinned = (sskBitmapWithNorthWestDiagonal(pinnedSquare) ^ sskBitmapWithNorthWestDiagonal(behindSquare) & SSK_BITMAP_UNSET_SQUARE_IDX(pinnedSquare));
		blockers = behindToPinned & bitboardPosition.occupied;
		
		// A piece exists in the region: pinned<->behind, so no pin!
		if (blockers) { return kFalse; }
		
		// Obtain mask for region: rest of rank<->pinned
		pinnedToPinner = (sskBitmapWithNorthWestDiagonal(pinnedSquare));
		blockers = pinnedToPinner & bitboardPosition.occupied;
		
		// There is no piece to create a pin
		if (blockers == 0) { return kFalse; }
		
		*pinCausingPieceSquare = sskFirstOneIndex(blockers);
		pinner = offsetPosition[*pinCausingPieceSquare];
		
		// The blocker is the same color as of the pinned piece
		if (SSK_GET_PIECE_COLOR(pinner) == color) { return kFalse; }
		
		if (SSK_GET_GENERIC_PIECE_CODE(pinner) == sskChessPieceBishop || SSK_GET_GENERIC_PIECE_CODE(pinner) == sskChessPieceQueen) {
			return kTrue;
		} else {
			return kFalse;
		}
	}
	
	return kFalse;
}

sskSquareCommonality sskGetSquareCommonality(sskChessSquare squares[], int numSquares) {
	if (numSquares <= 1) return sskSquareCommonalityNone;
	
	int i, j;
	sskSquareCommonality commonality = sskSquareCommonalityNone;
	
	for (i = 0; i < numSquares - 1; i++) {
		for (j = i + 1; j < numSquares; j++) {
			if (SSK_GET_FILE_IDX(squares[i]) == SSK_GET_FILE_IDX(squares[j])) {
				commonality |= sskSquareCommonalityFile;
			}
			if (SSK_GET_RANK_IDX(squares[i]) == SSK_GET_RANK_IDX(squares[j])) {
				commonality |= sskSquareCommonalityRank;
			}
		}
	}
	
	return commonality;
}

kBool sskCheckMoveAmbiguityAndFillFromSquare(sskOffsetPosition offsetPosition, sskChessSquare reachablePiecesFromSquares[], unsigned short numReachablePieces, sskMove * move) {
	if (numReachablePieces == 0) return kFalse;
	
	kBool ambiguity = kFalse;
	int i;
    
	if (numReachablePieces == 1) { // If any extra information was extracted, make sure it is relevant.
		switch (move->fromSquareExtracted) {
			case sskChessSquareInfoFileOnly: {
				if (SSK_GET_FILE_IDX(move->fromSquare) != SSK_GET_FILE_IDX(reachablePiecesFromSquares[0])) {
					ambiguity = kTrue; // Check if the extracted file is indeed the calculated fromSquare
				}
				break;
			}
			case sskChessSquareInfoRankOnly: {
				if (SSK_GET_RANK_IDX(move->fromSquare) != SSK_GET_RANK_IDX(reachablePiecesFromSquares[0])) {
					ambiguity = kTrue; // Check if the extracted file is indeed the calculated fromSquare
				}
				break;
			}
			case sskChessSquareInfoFileAndRank: {
				if (move->fromSquare != reachablePiecesFromSquares[0]) {
					ambiguity = kTrue; // Check if the extracted fromSquare is same as the calculated fromSquare
				}
				break;
			}
			case sskChessSquareInfoNone:
			default:
				break;
		}
		move->fromSquare = reachablePiecesFromSquares[0];
	} else if (numReachablePieces >= 2) {
		sskChessSquareInfo squareCommonality = sskGetSquareCommonality(reachablePiecesFromSquares, numReachablePieces);
		
		if(squareCommonality == sskSquareCommonalityNone) {
			// Check for information extracted by the lexical analyzer to resolve ambiguity.
			switch (move->fromSquareExtracted) {
				case sskChessSquareInfoNone: {
					ambiguity = kTrue;
					break;
				}
					
				case sskChessSquareInfoFileOnly: {
					// Find the piece on the particular file.
					for (i = 0; i < numReachablePieces; i++) {
						if (SSK_GET_FILE_IDX(reachablePiecesFromSquares[i]) == SSK_GET_FILE_IDX(move->fromSquare)) {
							move->fromSquare = reachablePiecesFromSquares[i];
							break;
						}
					}
					// No piece was found!
					if (i == numReachablePieces) { ambiguity = kTrue; }
					break;
				}
					
				case sskChessSquareInfoRankOnly: {
					// Find the piece on the particular rank.
					for (i = 0; i < numReachablePieces; i++) {
						if (SSK_GET_RANK_IDX(reachablePiecesFromSquares[i]) == SSK_GET_RANK_IDX(move->fromSquare)) {
							move->fromSquare = reachablePiecesFromSquares[i];
							break;
						}
					}
					// No piece was found!
					if (i == numReachablePieces) { ambiguity = kTrue; }
					break;
				}
					
				case sskChessSquareInfoFileAndRank: {
					// Lookup to find if the piece exists on the given square
					if (offsetPosition[move->fromSquare] != move->pieceMoved) {
						ambiguity = kTrue;
					}
					break;
				}
			}
		} else if((squareCommonality & sskSquareCommonalityFile) && (squareCommonality & sskSquareCommonalityRank)) {
			if (move->fromSquareExtracted == sskChessSquareInfoFileAndRank) {
				if (offsetPosition[move->fromSquare] != move->pieceMoved) {
					ambiguity = kTrue;
				}
			} else {
				ambiguity = kTrue;
			}
		} else if(squareCommonality & sskSquareCommonalityFile) {
			// Check for information extracted by the lexical analyzer to resolve ambiguity.
			switch (move->fromSquareExtracted) {
				case sskChessSquareInfoNone:
				case sskChessSquareInfoFileOnly: {
					ambiguity = kTrue;
					break;
				}
				case sskChessSquareInfoRankOnly: {
					// Find the piece on the particular rank.
					for (i = 0; i < numReachablePieces; i++) {
						if (SSK_GET_RANK_IDX(reachablePiecesFromSquares[i]) == SSK_GET_RANK_IDX(move->fromSquare)) {
							move->fromSquare = reachablePiecesFromSquares[i];
							break;
						}
					}
					// No piece was found!
					if (i == numReachablePieces) { ambiguity = kTrue; }
					break;
				}
					
				case sskChessSquareInfoFileAndRank: {
					// Lookup to find if the piece exists on the given square
					if (offsetPosition[move->fromSquare] != move->pieceMoved) {
						ambiguity = kTrue;
					}
					break;
				}
			}
		} else if(squareCommonality & sskSquareCommonalityRank) {
			// Check for information extracted by the lexical analyzer to resolve ambiguity.
			switch (move->fromSquareExtracted) {
				case sskChessSquareInfoNone:
				case sskChessSquareInfoRankOnly: {
					ambiguity = kTrue;
					break;
				}
				case sskChessSquareInfoFileOnly: {
					// Find the piece on the particular file.
					for (i = 0; i < numReachablePieces; i++) {
						if (SSK_GET_FILE_IDX(reachablePiecesFromSquares[i]) == SSK_GET_FILE_IDX(move->fromSquare)) {
							move->fromSquare = reachablePiecesFromSquares[i];
							break;
						}
					}
					// No piece was found!
					if (i == numReachablePieces) { ambiguity = kTrue; }
					break;
				}
					
				case sskChessSquareInfoFileAndRank: {
					// Lookup to find if the piece exists on the given square
					if (offsetPosition[move->fromSquare] != move->pieceMoved) {
						ambiguity = kTrue;
					}
					break;
				}
			}
		}
	}
	
	return ambiguity;
}