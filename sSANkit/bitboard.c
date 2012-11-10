/**
 *	@file
 *	Implements the function declared in bitboard.h
 *
 *	@author Santhosbaala RS
 *	@copyright 2012 64cloud
 *	@version 0.1
 */

#include "bitboard.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#pragma mark - Masking Functions

sskBitmap sskBitmapWithFileMask(sskChessSquareFile fileIndex) {
	static const sskBitmap _sskBitmapWithFileMask[8] = {
		0x0101010101010101, 0x0202020202020202, 0x0404040404040404, 0x0808080808080808, 0x1010101010101010, 0x2020202020202020, 0x4040404040404040, 0x8080808080808080,
	};
	
	if (fileIndex > 7) return SSK_EMPTY_BITMAP;	
	return _sskBitmapWithFileMask[fileIndex];
}

sskBitmap sskBitmapWithRankMask(sskChessSquareRank rankIndex) {
	static const sskBitmap _sskBitmapWithRankMask[8] = {
		0x00000000000000ff, 0x000000000000ff00, 0x0000000000ff0000, 0x00000000ff000000, 0x000000ff00000000, 0x0000ff0000000000, 0x00ff000000000000, 0xff00000000000000,
	};
	
	if (rankIndex > 7) return SSK_EMPTY_BITMAP;
	return _sskBitmapWithRankMask[rankIndex];
}

#pragma mark - Bitmap Lookup(Hash) Functions

sskBitmap sskBitmapWithKnightReach(sskChessSquare squareIndex) {
	if (squareIndex > 63) return SSK_EMPTY_BITMAP;
	
	static const sskBitmap _sskBitmapWithKnightReach[64] = {
		0x0000000000020400, 0x0000000000050800, 0x00000000000a1100, 0x0000000000142200, 0x0000000000284400, 0x0000000000508800, 0x0000000000a01000, 0x0000000000402000,
		0x0000000002040004, 0x0000000005080008, 0x000000000a110011, 0x0000000014220022, 0x0000000028440044, 0x0000000050880088, 0x00000000a0100010, 0x0000000040200020,
		0x0000000204000402, 0x0000000508000805, 0x0000000a1100110a, 0x0000001422002214, 0x0000002844004428, 0x0000005088008850, 0x000000a0100010a0, 0x0000004020002040,
		0x0000020400040200, 0x0000050800080500, 0x00000a1100110a00, 0x0000142200221400, 0x0000284400442800, 0x0000508800885000, 0x0000a0100010a000, 0x0000402000204000,
		0x0002040004020000, 0x0005080008050000, 0x000a1100110a0000, 0x0014220022140000, 0x0028440044280000, 0x0050880088500000, 0x00a0100010a00000, 0x0040200020400000,
		0x0204000402000000, 0x0508000805000000, 0x0a1100110a000000, 0x1422002214000000, 0x2844004428000000, 0x5088008850000000, 0xa0100010a0000000, 0x4020002040000000,
		0x0400040200000000, 0x0800080500000000, 0x1100110a00000000, 0x2200221400000000, 0x4400442800000000, 0x8800885000000000, 0x100010a000000000, 0x2000204000000000,
		0x0004020000000000, 0x0008050000000000, 0x00110a0000000000, 0x0022140000000000, 0x0044280000000000, 0x0088500000000000, 0x0010a00000000000, 0x0020400000000000,
	};
	
	return _sskBitmapWithKnightReach[squareIndex];
}

sskBitmap sskBitmapWithKingReach(sskChessSquare squareIndex) {
	if (squareIndex > 63) return SSK_EMPTY_BITMAP;
	
	static const sskBitmap _sskBitmapWithKingReach[64] = {
		0x0000000000000302, 0x0000000000000705, 0x0000000000000e0a, 0x0000000000001c14, 0x0000000000003828, 0x0000000000007050, 0x000000000000e0a0, 0x000000000000c040,
		0x0000000000030203, 0x0000000000070507, 0x00000000000e0a0e, 0x00000000001c141c, 0x0000000000382838, 0x0000000000705070, 0x0000000000e0a0e0, 0x0000000000c040c0,
		0x0000000003020300, 0x0000000007050700, 0x000000000e0a0e00, 0x000000001c141c00, 0x0000000038283800, 0x0000000070507000, 0x00000000e0a0e000, 0x00000000c040c000,
		0x0000000302030000, 0x0000000705070000, 0x0000000e0a0e0000, 0x0000001c141c0000, 0x0000003828380000, 0x0000007050700000, 0x000000e0a0e00000, 0x000000c040c00000,
		0x0000030203000000, 0x0000070507000000, 0x00000e0a0e000000, 0x00001c141c000000, 0x0000382838000000, 0x0000705070000000, 0x0000e0a0e0000000, 0x0000c040c0000000,
		0x0003020300000000, 0x0007050700000000, 0x000e0a0e00000000, 0x001c141c00000000, 0x0038283800000000, 0x0070507000000000, 0x00e0a0e000000000, 0x00c040c000000000,
		0x0302030000000000, 0x0705070000000000, 0x0e0a0e0000000000, 0x1c141c0000000000, 0x3828380000000000, 0x7050700000000000, 0xe0a0e00000000000, 0xc040c00000000000,
		0x0203000000000000, 0x0507000000000000, 0x0a0e000000000000, 0x141c000000000000, 0x2838000000000000, 0x5070000000000000, 0xa0e0000000000000, 0x40c0000000000000,
	};
	
	return _sskBitmapWithKingReach[squareIndex];
}

sskBitmap sskBitmapWithKingCastlePath(sskChessColor kingColor, unsigned short castlingSide, sskChessSquareFile kingFile, sskChessSquareFile rookFile) {
	if (kingColor > 1) return SSK_EMPTY_BITMAP;
	if (castlingSide > 1) return SSK_EMPTY_BITMAP;
	if (kingFile <= 0 || kingFile >= 7) return SSK_EMPTY_BITMAP;
	if (rookFile > 7) return SSK_EMPTY_BITMAP;
		
	if (kingFile == rookFile) return SSK_FULL_BITMAP;
	if (castlingSide == 0 && (rookFile < kingFile)) return SSK_FULL_BITMAP;
	if (castlingSide == 1 && (rookFile > kingFile)) return SSK_FULL_BITMAP;
	
	static const sskBitmap _sskBitmapWithKingCastlePath[2][2][8] = {
		// [0][0] [white][kingSide]
		0x0000000000000000, 0x000000000000007c, 0x0000000000000078, 0x0000000000000070, 0x0000000000000060, 0x0000000000000040, 0x0000000000000020, 0x0000000000000000,
		// [0][1] [white][queenSide]
		0x0000000000000000, 0x000000000000000c, 0x000000000000000a, 0x0000000000000006, 0x000000000000000e, 0x000000000000001e, 0x000000000000003e, 0x0000000000000000,
		// [1][0] [black][kingSide]
		0x0000000000000000, 0x7c00000000000000, 0x7800000000000000, 0x7000000000000000, 0x6000000000000000, 0x4000000000000000, 0x2000000000000000, 0x0000000000000000,
		// [1][1] [black][queenSide]
		0x0000000000000000, 0x0c00000000000000, 0x0a00000000000000, 0x0600000000000000, 0x0e00000000000000, 0x1e00000000000000, 0x3e00000000000000, 0x0000000000000000,
	};
	
	return _sskBitmapWithKingCastlePath[kingColor][castlingSide][kingFile] & SSK_BITMAP_UNSET_SQUARE_IDX(( (kingColor == sskChessColorWhite)?rookFile:(56+rookFile) ));
}

sskBitmap sskBitmapWithPawnReach(sskChessSquare squareIndex, sskChessColor pawnColor) {
	if (squareIndex > 63) return SSK_EMPTY_BITMAP;
	if (pawnColor > 1) return SSK_EMPTY_BITMAP;
	
	static const sskBitmap _sskBitmapWithPawnReach[2][64] = {
		// White Pawn
		0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
		0x0000000001030000, 0x0000000002070000, 0x00000000040e0000, 0x00000000081c0000, 0x0000000010380000, 0x0000000020700000, 0x0000000040e00000, 0x0000000080c00000,
		0x0000000003000000, 0x0000000007000000, 0x000000000e000000, 0x000000001c000000, 0x0000000038000000, 0x0000000070000000, 0x00000000e0000000, 0x00000000c0000000,
		0x0000000300000000, 0x0000000700000000, 0x0000000e00000000, 0x0000001c00000000, 0x0000003800000000, 0x0000007000000000, 0x000000e000000000, 0x000000c000000000,
		0x0000030000000000, 0x0000070000000000, 0x00000e0000000000, 0x00001c0000000000, 0x0000380000000000, 0x0000700000000000, 0x0000e00000000000, 0x0000c00000000000,
		0x0003000000000000, 0x0007000000000000, 0x000e000000000000, 0x001c000000000000, 0x0038000000000000, 0x0070000000000000, 0x00e0000000000000, 0x00c0000000000000,
		0x0300000000000000, 0x0700000000000000, 0x0e00000000000000, 0x1c00000000000000, 0x3800000000000000, 0x7000000000000000, 0xe000000000000000, 0xc000000000000000,
		0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
		
		// Black Pawn
		0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
		0x0000000000000003, 0x0000000000000007, 0x000000000000000e, 0x000000000000001c, 0x0000000000000038, 0x0000000000000070, 0x00000000000000e0, 0x00000000000000c0,
		0x0000000000000300, 0x0000000000000700, 0x0000000000000e00, 0x0000000000001c00, 0x0000000000003800, 0x0000000000007000, 0x000000000000e000, 0x000000000000c000,
		0x0000000000030000, 0x0000000000070000, 0x00000000000e0000, 0x00000000001c0000, 0x0000000000380000, 0x0000000000700000, 0x0000000000e00000, 0x0000000000c00000,
		0x0000000003000000, 0x0000000007000000, 0x000000000e000000, 0x000000001c000000, 0x0000000038000000, 0x0000000070000000, 0x00000000e0000000, 0x00000000c0000000,
		0x0000000300000000, 0x0000000700000000, 0x0000000e00000000, 0x0000001c00000000, 0x0000003800000000, 0x0000007000000000, 0x000000e000000000, 0x000000c000000000,
		0x0000030100000000, 0x0000070200000000, 0x00000e0400000000, 0x00001c0800000000, 0x0000381000000000, 0x0000702000000000, 0x0000e04000000000, 0x0000c08000000000,
		0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
	};
	
	return _sskBitmapWithPawnReach[pawnColor][squareIndex];
}

sskBitmap sskBitmapWithNorthEastDiagonal(sskChessSquare squareIndex) {
	if (squareIndex > 63) return SSK_EMPTY_BITMAP;
	
	static const sskBitmap _sskBitmapWithNorthEastDiagonal[64] = {	// a-h horizontal, 1-8 vertical
		0x8040201008040200, 0x0080402010080400, 0x0000804020100800, 0x0000008040201000, 0x0000000080402000, 0x0000000000804000, 0x0000000000008000, 0x0000000000000000,
		0x4020100804020000, 0x8040201008040000, 0x0080402010080000, 0x0000804020100000, 0x0000008040200000, 0x0000000080400000, 0x0000000000800000, 0x0000000000000000,
		0x2010080402000000, 0x4020100804000000, 0x8040201008000000, 0x0080402010000000, 0x0000804020000000, 0x0000008040000000, 0x0000000080000000, 0x0000000000000000,
		0x1008040200000000, 0x2010080400000000, 0x4020100800000000, 0x8040201000000000, 0x0080402000000000, 0x0000804000000000, 0x0000008000000000, 0x0000000000000000,
		0x0804020000000000, 0x1008040000000000, 0x2010080000000000, 0x4020100000000000, 0x8040200000000000, 0x0080400000000000, 0x0000800000000000, 0x0000000000000000,
		0x0402000000000000, 0x0804000000000000, 0x1008000000000000, 0x2010000000000000, 0x4020000000000000, 0x8040000000000000, 0x0080000000000000, 0x0000000000000000,
		0x0200000000000000, 0x0400000000000000, 0x0800000000000000, 0x1000000000000000, 0x2000000000000000, 0x4000000000000000, 0x8000000000000000, 0x0000000000000000,
		0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
	};

	return _sskBitmapWithNorthEastDiagonal[squareIndex];
}

sskBitmap sskBitmapWithNorthWestDiagonal(sskChessSquare squareIndex) {
	if (squareIndex > 63) return SSK_EMPTY_BITMAP;
	
	static const sskBitmap _sskBitmapWithNorthWestDiagonal[64] = {
		0x0000000000000000, 0x0000000000000100, 0x0000000000010200, 0x0000000001020400, 0x0000000102040800, 0x0000010204081000, 0x0001020408102000, 0x0102040810204000,
		0x0000000000000000, 0x0000000000010000, 0x0000000001020000, 0x0000000102040000, 0x0000010204080000, 0x0001020408100000, 0x0102040810200000, 0x0204081020400000,
		0x0000000000000000, 0x0000000001000000, 0x0000000102000000, 0x0000010204000000, 0x0001020408000000, 0x0102040810000000, 0x0204081020000000, 0x0408102040000000,
		0x0000000000000000, 0x0000000100000000, 0x0000010200000000, 0x0001020400000000, 0x0102040800000000, 0x0204081000000000, 0x0408102000000000, 0x0810204000000000,
		0x0000000000000000, 0x0000010000000000, 0x0001020000000000, 0x0102040000000000, 0x0204080000000000, 0x0408100000000000, 0x0810200000000000, 0x1020400000000000,
		0x0000000000000000, 0x0001000000000000, 0x0102000000000000, 0x0204000000000000, 0x0408000000000000, 0x0810000000000000, 0x1020000000000000, 0x2040000000000000,
		0x0000000000000000, 0x0100000000000000, 0x0200000000000000, 0x0400000000000000, 0x0800000000000000, 0x1000000000000000, 0x2000000000000000, 0x4000000000000000,
		0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
	};
	
	return _sskBitmapWithNorthWestDiagonal[squareIndex];
}

sskBitmap sskBitmapWithSouthEastDiagonal(sskChessSquare squareIndex) {
	if (squareIndex > 63) return SSK_EMPTY_BITMAP;
	
	static const sskBitmap _sskBitmapWithSouthEastDiagonal[64] = {
		0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
		0x0000000000000002, 0x0000000000000004, 0x0000000000000008, 0x0000000000000010, 0x0000000000000020, 0x0000000000000040, 0x0000000000000080, 0x0000000000000000,
		0x0000000000000204, 0x0000000000000408, 0x0000000000000810, 0x0000000000001020, 0x0000000000002040, 0x0000000000004080, 0x0000000000008000, 0x0000000000000000,
		0x0000000000020408, 0x0000000000040810, 0x0000000000081020, 0x0000000000102040, 0x0000000000204080, 0x0000000000408000, 0x0000000000800000, 0x0000000000000000,
		0x0000000002040810, 0x0000000004081020, 0x0000000008102040, 0x0000000010204080, 0x0000000020408000, 0x0000000040800000, 0x0000000080000000, 0x0000000000000000,
		0x0000000204081020, 0x0000000408102040, 0x0000000810204080, 0x0000001020408000, 0x0000002040800000, 0x0000004080000000, 0x0000008000000000, 0x0000000000000000,
		0x0000020408102040, 0x0000040810204080, 0x0000081020408000, 0x0000102040800000, 0x0000204080000000, 0x0000408000000000, 0x0000800000000000, 0x0000000000000000,
		0x0002040810204080, 0x0004081020408000, 0x0008102040800000, 0x0010204080000000, 0x0020408000000000, 0x0040800000000000, 0x0080000000000000, 0x0000000000000000,
	};
	
	return _sskBitmapWithSouthEastDiagonal[squareIndex];
}

sskBitmap sskBitmapWithSouthWestDiagonal(sskChessSquare squareIndex) {
	if (squareIndex > 63) return SSK_EMPTY_BITMAP;
	
	static const sskBitmap _sskBitmapWithSouthWestDiagonal[64] = {
		0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
		0x0000000000000000, 0x0000000000000001, 0x0000000000000002, 0x0000000000000004, 0x0000000000000008, 0x0000000000000010, 0x0000000000000020, 0x0000000000000040,
		0x0000000000000000, 0x0000000000000100, 0x0000000000000201, 0x0000000000000402, 0x0000000000000804, 0x0000000000001008, 0x0000000000002010, 0x0000000000004020,
		0x0000000000000000, 0x0000000000010000, 0x0000000000020100, 0x0000000000040201, 0x0000000000080402, 0x0000000000100804, 0x0000000000201008, 0x0000000000402010,
		0x0000000000000000, 0x0000000001000000, 0x0000000002010000, 0x0000000004020100, 0x0000000008040201, 0x0000000010080402, 0x0000000020100804, 0x0000000040201008,
		0x0000000000000000, 0x0000000100000000, 0x0000000201000000, 0x0000000402010000, 0x0000000804020100, 0x0000001008040201, 0x0000002010080402, 0x0000004020100804,
		0x0000000000000000, 0x0000010000000000, 0x0000020100000000, 0x0000040201000000, 0x0000080402010000, 0x0000100804020100, 0x0000201008040201, 0x0000402010080402,
		0x0000000000000000, 0x0001000000000000, 0x0002010000000000, 0x0004020100000000, 0x0008040201000000, 0x0010080402010000, 0x0020100804020100, 0x0040201008040201,
	};
	
	return _sskBitmapWithSouthWestDiagonal[squareIndex];
}

sskBitmap sskBitmapWithNorthFile(sskChessSquare squareIndex) {
	if (squareIndex > 63) return SSK_EMPTY_BITMAP;

	static const sskBitmap _sskBitmapWithNorthFile[64] = {
		0x0101010101010100, 0x0202020202020200, 0x0404040404040400, 0x0808080808080800, 0x1010101010101000, 0x2020202020202000, 0x4040404040404000, 0x8080808080808000,
		0x0101010101010000, 0x0202020202020000, 0x0404040404040000, 0x0808080808080000, 0x1010101010100000, 0x2020202020200000, 0x4040404040400000, 0x8080808080800000,
		0x0101010101000000, 0x0202020202000000, 0x0404040404000000, 0x0808080808000000, 0x1010101010000000, 0x2020202020000000, 0x4040404040000000, 0x8080808080000000,
		0x0101010100000000, 0x0202020200000000, 0x0404040400000000, 0x0808080800000000, 0x1010101000000000, 0x2020202000000000, 0x4040404000000000, 0x8080808000000000,
		0x0101010000000000, 0x0202020000000000, 0x0404040000000000, 0x0808080000000000, 0x1010100000000000, 0x2020200000000000, 0x4040400000000000, 0x8080800000000000,
		0x0101000000000000, 0x0202000000000000, 0x0404000000000000, 0x0808000000000000, 0x1010000000000000, 0x2020000000000000, 0x4040000000000000, 0x8080000000000000,
		0x0100000000000000, 0x0200000000000000, 0x0400000000000000, 0x0800000000000000, 0x1000000000000000, 0x2000000000000000, 0x4000000000000000, 0x8000000000000000,
		0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
	};
	
	return _sskBitmapWithNorthFile[squareIndex];
}

sskBitmap sskBitmapWithSouthFile(sskChessSquare squareIndex) {
	if (squareIndex > 63) return SSK_EMPTY_BITMAP;

	static const sskBitmap _sskBitmapWithSouthFile[64] = {
		0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
		0x0000000000000001, 0x0000000000000002, 0x0000000000000004, 0x0000000000000008, 0x0000000000000010, 0x0000000000000020, 0x0000000000000040, 0x0000000000000080,
		0x0000000000000101, 0x0000000000000202, 0x0000000000000404, 0x0000000000000808, 0x0000000000001010, 0x0000000000002020, 0x0000000000004040, 0x0000000000008080,
		0x0000000000010101, 0x0000000000020202, 0x0000000000040404, 0x0000000000080808, 0x0000000000101010, 0x0000000000202020, 0x0000000000404040, 0x0000000000808080,
		0x0000000001010101, 0x0000000002020202, 0x0000000004040404, 0x0000000008080808, 0x0000000010101010, 0x0000000020202020, 0x0000000040404040, 0x0000000080808080,
		0x0000000101010101, 0x0000000202020202, 0x0000000404040404, 0x0000000808080808, 0x0000001010101010, 0x0000002020202020, 0x0000004040404040, 0x0000008080808080,
		0x0000010101010101, 0x0000020202020202, 0x0000040404040404, 0x0000080808080808, 0x0000101010101010, 0x0000202020202020, 0x0000404040404040, 0x0000808080808080,
		0x0001010101010101, 0x0002020202020202, 0x0004040404040404, 0x0008080808080808, 0x0010101010101010, 0x0020202020202020, 0x0040404040404040, 0x0080808080808080,
	};
	
	return _sskBitmapWithSouthFile[squareIndex];
}

sskBitmap sskBitmapWithEastRank(sskChessSquare squareIndex) {
	if (squareIndex > 63) return SSK_EMPTY_BITMAP;

	static const sskBitmap _sskBitmapWithEastRank[64] = {
		0x00000000000000fe, 0x00000000000000fc, 0x00000000000000f8, 0x00000000000000f0, 0x00000000000000e0, 0x00000000000000c0, 0x0000000000000080, 0x0000000000000000,
		0x000000000000fe00, 0x000000000000fc00, 0x000000000000f800, 0x000000000000f000, 0x000000000000e000, 0x000000000000c000, 0x0000000000008000, 0x0000000000000000,
		0x0000000000fe0000, 0x0000000000fc0000, 0x0000000000f80000, 0x0000000000f00000, 0x0000000000e00000, 0x0000000000c00000, 0x0000000000800000, 0x0000000000000000,
		0x00000000fe000000, 0x00000000fc000000, 0x00000000f8000000, 0x00000000f0000000, 0x00000000e0000000, 0x00000000c0000000, 0x0000000080000000, 0x0000000000000000,
		0x000000fe00000000, 0x000000fc00000000, 0x000000f800000000, 0x000000f000000000, 0x000000e000000000, 0x000000c000000000, 0x0000008000000000, 0x0000000000000000,
		0x0000fe0000000000, 0x0000fc0000000000, 0x0000f80000000000, 0x0000f00000000000, 0x0000e00000000000, 0x0000c00000000000, 0x0000800000000000, 0x0000000000000000,
		0x00fe000000000000, 0x00fc000000000000, 0x00f8000000000000, 0x00f0000000000000, 0x00e0000000000000, 0x00c0000000000000, 0x0080000000000000, 0x0000000000000000,
		0xfe00000000000000, 0xfc00000000000000, 0xf800000000000000, 0xf000000000000000, 0xe000000000000000, 0xc000000000000000, 0x8000000000000000, 0x0000000000000000,
	};
	
	return _sskBitmapWithEastRank[squareIndex];
}

sskBitmap sskBitmapWithWestRank(sskChessSquare squareIndex) {
	if (squareIndex > 63) return SSK_EMPTY_BITMAP;

	static const sskBitmap _sskBitmapWithWestRank[64] = {
		0x0000000000000000, 0x0000000000000001, 0x0000000000000003, 0x0000000000000007, 0x000000000000000f, 0x000000000000001f, 0x000000000000003f, 0x000000000000007f,
		0x0000000000000000, 0x0000000000000100, 0x0000000000000300, 0x0000000000000700, 0x0000000000000f00, 0x0000000000001f00, 0x0000000000003f00, 0x0000000000007f00,
		0x0000000000000000, 0x0000000000010000, 0x0000000000030000, 0x0000000000070000, 0x00000000000f0000, 0x00000000001f0000, 0x00000000003f0000, 0x00000000007f0000,
		0x0000000000000000, 0x0000000001000000, 0x0000000003000000, 0x0000000007000000, 0x000000000f000000, 0x000000001f000000, 0x000000003f000000, 0x000000007f000000,
		0x0000000000000000, 0x0000000100000000, 0x0000000300000000, 0x0000000700000000, 0x0000000f00000000, 0x0000001f00000000, 0x0000003f00000000, 0x0000007f00000000,
		0x0000000000000000, 0x0000010000000000, 0x0000030000000000, 0x0000070000000000, 0x00000f0000000000, 0x00001f0000000000, 0x00003f0000000000, 0x00007f0000000000,
		0x0000000000000000, 0x0001000000000000, 0x0003000000000000, 0x0007000000000000, 0x000f000000000000, 0x001f000000000000, 0x003f000000000000, 0x007f000000000000,
		0x0000000000000000, 0x0100000000000000, 0x0300000000000000, 0x0700000000000000, 0x0f00000000000000, 0x1f00000000000000, 0x3f00000000000000, 0x7f00000000000000,
	};
	
	return _sskBitmapWithWestRank[squareIndex];
}

#pragma mark - BitScan Functions

unsigned short sskCountBits(sskBitmap bitmap) {
	/// MIT HAKMEM algorithm, @link http://graphics.stanford.edu/~seander/bithacks.html
	
	static const sskBitmap  M1 = 0x5555555555555555;  // 1 zero,  1 one ...
	static const sskBitmap  M2 = 0x3333333333333333;  // 2 zeros,  2 ones ...
	static const sskBitmap  M4 = 0x0f0f0f0f0f0f0f0f;  // 4 zeros,  4 ones ...
	static const sskBitmap  M8 = 0x00ff00ff00ff00ff;  // 8 zeros,  8 ones ...
	static const sskBitmap M16 = 0x0000ffff0000ffff;  // 16 zeros, 16 ones ...
	static const sskBitmap M32 = 0x00000000ffffffff;  // 32 zeros, 32 ones
	
    bitmap = (bitmap & M1 ) + ((bitmap >>  1) & M1 );   // put count of each  2 bits into those  2 bits
    bitmap = (bitmap & M2 ) + ((bitmap >>  2) & M2 );   // put count of each  4 bits into those  4 bits
    bitmap = (bitmap & M4 ) + ((bitmap >>  4) & M4 );   // put count of each  8 bits into those  8 bits
    bitmap = (bitmap & M8 ) + ((bitmap >>  8) & M8 );   // put count of each 16 bits into those 16 bits
    bitmap = (bitmap & M16) + ((bitmap >> 16) & M16);   // put count of each 32 bits into those 32 bits
    bitmap = (bitmap & M32) + ((bitmap >> 32) & M32);   // put count of each 64 bits into those 64 bits
    
	return (unsigned short)bitmap;
}

unsigned short sskFirstOneIndex(sskBitmap bitmap) {
	/// De Bruijn Multiplication, @link http://chessprogramming.wikispaces.com/BitScan
	// don't use this if bitmap = 0
	
	static const int INDEX64[64] = {
		63,  0, 58,  1, 59, 47, 53,  2,
		60, 39, 48, 27, 54, 33, 42,  3,
		61, 51, 37, 40, 49, 18, 28, 20,
		55, 30, 34, 11, 43, 14, 22,  4,
		62, 57, 46, 52, 38, 26, 32, 41,
		50, 36, 17, 19, 29, 10, 13, 21,
		56, 45, 25, 31, 35, 16,  9, 12,
		44, 24, 15,  8, 23,  7,  6,  5  };
	
	static const sskBitmap DEBRUIJN64 = 0x07EDD5E59A4E28C2;
	
	return INDEX64[((bitmap & -bitmap) * DEBRUIJN64) >> 58];
}

unsigned short sskLastOneIndex(sskBitmap bitmap) {
    // this is Eugene Nalimov's bitScanReverse
	// use sskFirstOneIndex if you can, it is faster than sskLastOneIndex.
	// don't use this if bitmap = 0
	
	// Nalimoves MSB 1 Bit Table.
	static const int MS1BTABLE[256] = {
		0, 0,
		1, 1,
		2, 2, 2, 2,
		3, 3, 3, 3, 3, 3, 3, 3,
		4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
		5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
		5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
		6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
		6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
		6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
		6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
		7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
		7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
		7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
		7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
		7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
		7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
		7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
		7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
	};
	
	int result = 0;
	if (bitmap > 0xFFFFFFFF)
	{
		bitmap >>= 32;
		result = 32;
	}
	if (bitmap > 0xFFFF)
	{
		bitmap >>= 16;
		result += 16;
	}
	if (bitmap > 0xFF)
	{
		bitmap >>= 8;
		result += 8;
	}
	return result + MS1BTABLE[bitmap];
}

#pragma mark - Utility Functions

sskBitmap sskBitmapForPieceInBitboardPosition(sskBitboardPosition bitboardPosition, sskChessPiece pieceCode) {
    switch (pieceCode) {
        case sskChessPieceWPawn: return bitboardPosition.wPawn;
        case sskChessPieceWKing: return bitboardPosition.wKing;
        case sskChessPieceWQueen: return bitboardPosition.wQueen;
        case sskChessPieceWRook: return bitboardPosition.wRook;
        case sskChessPieceWBishop: return bitboardPosition.wBishop;
        case sskChessPieceWKnight: return bitboardPosition.wKnight;
        case sskChessPieceBPawn: return bitboardPosition.bPawn;
        case sskChessPieceBKing: return bitboardPosition.bKing;
        case sskChessPieceBQueen: return bitboardPosition.bQueen;
        case sskChessPieceBRook: return bitboardPosition.bRook;
        case sskChessPieceBBishop: return bitboardPosition.bBishop;
        case sskChessPieceBKnight: return bitboardPosition.bKnight;
    }
    
    return SSK_EMPTY_BITMAP;
}

sskBitmap * sskUpdateableBitmapForPieceInBitboardPosition(sskBitboardPosition * bitboardPosition, sskChessPiece pieceCode) {
	switch (pieceCode) {
        case sskChessPieceWPawn: return &(bitboardPosition->wPawn);
        case sskChessPieceWKing: return &(bitboardPosition->wKing);
        case sskChessPieceWQueen: return &(bitboardPosition->wQueen);
        case sskChessPieceWRook: return &(bitboardPosition->wRook);
        case sskChessPieceWBishop: return &(bitboardPosition->wBishop);
        case sskChessPieceWKnight: return &(bitboardPosition->wKnight);
        case sskChessPieceBPawn: return &(bitboardPosition->bPawn);
        case sskChessPieceBKing: return &(bitboardPosition->bKing);
        case sskChessPieceBQueen: return &(bitboardPosition->bQueen);
        case sskChessPieceBRook: return &(bitboardPosition->bRook);
        case sskChessPieceBBishop: return &(bitboardPosition->bBishop);
        case sskChessPieceBKnight: return &(bitboardPosition->bKnight);
    }
	
	return NULL;
}

sskBitmap sskBitmapForSpecificPieceAttacksInBitboardPosition(sskBitboardPosition bitboardPosition, sskChessPiece pieceCode, sskChessSquare fromSquare, sskChessSquare toSquare, int isCastlingOrEnpassantTarget) {
    sskBitmap attacks = SSK_EMPTY_BITMAP, blockers;
    sskChessSquare blockerSquare;
    sskChessColor color = SSK_GET_PIECE_COLOR(pieceCode);
    
    switch (SSK_GET_GENERIC_PIECE_CODE(pieceCode)) {
		case sskChessPiecePawn: {
			if (SSK_GET_FILE_IDX(fromSquare) == SSK_GET_FILE_IDX(toSquare)) {
				attacks = sskBitmapWithPawnReach(fromSquare, color) & sskBitmapWithFileMask(SSK_GET_FILE_IDX(fromSquare));
				blockers = attacks & bitboardPosition.occupied;
				if (blockers) {
					// For white, bitscan from a1, for black bitscan from h8
					blockerSquare = (color == sskChessColorWhite)?sskFirstOneIndex(blockers):sskLastOneIndex(blockers);
					attacks = attacks & SSK_BITMAP_UNSET_SQUARE_IDX(blockerSquare);
					
					// Special blocker checking on 3/6 ranks for double pawn moves.
					if (color == sskChessColorWhite && SSK_GET_RANK_IDX(blockerSquare) == 2) {
						attacks = attacks & SSK_BITMAP_UNSET_SQUARE_IDX(blockerSquare + 8);
					} else if (color == sskChessColorBlack && SSK_GET_RANK_IDX(blockerSquare) == 5) {
						attacks = attacks & SSK_BITMAP_UNSET_SQUARE_IDX(blockerSquare - 8);
					}
				}
			} else {
				attacks = sskBitmapWithPawnReach(fromSquare, color) & sskBitmapWithFileMask(SSK_GET_FILE_IDX(toSquare));
				// If white include black pieces else include white pieces into the attack.
				attacks = attacks & ((color == sskChessColorWhite)?bitboardPosition.bOccupied:bitboardPosition.wOccupied);
				
				// Check for enpassant condition and reset the attacks
				if (SSK_GET_RANK_IDX(fromSquare) == ((color == sskChessColorWhite)?4:3) &&
					((isCastlingOrEnpassantTarget != -1) && (isCastlingOrEnpassantTarget == toSquare))
                    ) {
					attacks = sskBitmapWithPawnReach(fromSquare, color) & sskBitmapWithFileMask(SSK_GET_FILE_IDX(toSquare));
				}
			}
			
			break;
		}
            
		case sskChessPieceKing: {
			if (isCastlingOrEnpassantTarget != -1) {
				short castlingSide = (SSK_GET_FILE_IDX(fromSquare) < isCastlingOrEnpassantTarget)?0:1;
				attacks = sskBitmapWithKingCastlePath(color, castlingSide, SSK_GET_FILE_IDX(fromSquare), isCastlingOrEnpassantTarget);	// argument acts as rook's file
				blockers = attacks & bitboardPosition.occupied;
				
				// if any blockers were found.
				if (blockers != SSK_EMPTY_BITMAP) return SSK_EMPTY_BITMAP;
				else return attacks;
				// !IMPORTANT since castling is a special type of move,
				// we do not follow the normal course of isReachable().
			} else {
				attacks = sskBitmapWithKingReach(fromSquare);
			}
			break;
		}
			
		case sskChessPieceQueen: {
			if (SSK_GET_RANK_IDX(fromSquare) == SSK_GET_RANK_IDX(toSquare)) {
				if (SSK_GET_FILE_IDX(fromSquare) < SSK_GET_FILE_IDX(toSquare)) {	// East
					attacks = sskBitmapWithEastRank(fromSquare);
					blockers = attacks & bitboardPosition.occupied;
					blockerSquare = sskFirstOneIndex(blockers);
					attacks = attacks ^ sskBitmapWithEastRank(blockerSquare);
				} else if (SSK_GET_FILE_IDX(fromSquare) > SSK_GET_FILE_IDX(toSquare)) {	// West
					attacks = sskBitmapWithWestRank(fromSquare);
					blockers = attacks & bitboardPosition.occupied;
					blockerSquare = sskLastOneIndex(blockers);
					attacks = attacks ^ sskBitmapWithWestRank(blockerSquare);
				}
			} else if (SSK_GET_FILE_IDX(fromSquare) == SSK_GET_FILE_IDX(toSquare)) {
				if (SSK_GET_RANK_IDX(fromSquare) < SSK_GET_RANK_IDX(toSquare)) {	// North
					attacks = sskBitmapWithNorthFile(fromSquare);
					blockers = attacks & bitboardPosition.occupied;
					blockerSquare = sskFirstOneIndex(blockers);
					attacks = attacks ^ sskBitmapWithNorthFile(blockerSquare);
				} else if (SSK_GET_RANK_IDX(fromSquare) > SSK_GET_RANK_IDX(toSquare)) {	// South
					attacks = sskBitmapWithSouthFile(fromSquare);
					blockers = attacks & bitboardPosition.occupied;
					blockerSquare = sskLastOneIndex(blockers);
					attacks = attacks ^ sskBitmapWithSouthFile(blockerSquare);
				}
			} else if (SSK_GET_RANK_IDX(fromSquare) < SSK_GET_RANK_IDX(toSquare)) {
				if (SSK_GET_FILE_IDX(fromSquare) < SSK_GET_FILE_IDX(toSquare)) {	// North East
					attacks = sskBitmapWithNorthEastDiagonal(fromSquare);
					blockers = attacks & bitboardPosition.occupied;
					blockerSquare = sskFirstOneIndex(blockers);
					attacks = attacks ^ sskBitmapWithNorthEastDiagonal(blockerSquare);
				} else if (SSK_GET_FILE_IDX(fromSquare) > SSK_GET_FILE_IDX(toSquare)) {	// North West
					attacks = sskBitmapWithNorthWestDiagonal(fromSquare);
					blockers = attacks & bitboardPosition.occupied;
					blockerSquare = sskFirstOneIndex(blockers);
					attacks = attacks ^ sskBitmapWithNorthWestDiagonal(blockerSquare);
				}
			} else if (SSK_GET_RANK_IDX(fromSquare) > SSK_GET_RANK_IDX(toSquare)) {
				if (SSK_GET_FILE_IDX(fromSquare) < SSK_GET_FILE_IDX(toSquare)) {	// South East
					attacks = sskBitmapWithSouthEastDiagonal(fromSquare);
					blockers = attacks & bitboardPosition.occupied;
					blockerSquare = sskLastOneIndex(blockers);
					attacks = attacks ^ sskBitmapWithSouthEastDiagonal(blockerSquare);
				} else if (SSK_GET_FILE_IDX(fromSquare) > SSK_GET_FILE_IDX(toSquare)) {	// South West
					attacks = sskBitmapWithSouthWestDiagonal(fromSquare);
					blockers = attacks & bitboardPosition.occupied;
					blockerSquare = sskLastOneIndex(blockers);
					attacks = attacks ^ sskBitmapWithSouthWestDiagonal(blockerSquare);
				}
			}
            
			break;
		}
			
		case sskChessPieceRook: {
			if (SSK_GET_RANK_IDX(fromSquare) == SSK_GET_RANK_IDX(toSquare)) {
				if (SSK_GET_FILE_IDX(fromSquare) < SSK_GET_FILE_IDX(toSquare)) {	// East
					attacks = sskBitmapWithEastRank(fromSquare);
					blockers = attacks & bitboardPosition.occupied;
					blockerSquare = sskFirstOneIndex(blockers);
					attacks = attacks ^ sskBitmapWithEastRank(blockerSquare);
				} else if (SSK_GET_FILE_IDX(fromSquare) > SSK_GET_FILE_IDX(toSquare)) {	// West
					attacks = sskBitmapWithWestRank(fromSquare);
					blockers = attacks & bitboardPosition.occupied;
					blockerSquare = sskLastOneIndex(blockers);
					attacks = attacks ^ sskBitmapWithWestRank(blockerSquare);
				}
			} else if (SSK_GET_FILE_IDX(fromSquare) == SSK_GET_FILE_IDX(toSquare)) {
				if (SSK_GET_RANK_IDX(fromSquare) < SSK_GET_RANK_IDX(toSquare)) {	// North
					attacks = sskBitmapWithNorthFile(fromSquare);
					blockers = attacks & bitboardPosition.occupied;
					blockerSquare = sskFirstOneIndex(blockers);
					attacks = attacks ^ sskBitmapWithNorthFile(blockerSquare);
				} else if (SSK_GET_RANK_IDX(fromSquare) > SSK_GET_RANK_IDX(toSquare)) {	// South
					attacks = sskBitmapWithSouthFile(fromSquare);
					blockers = attacks & bitboardPosition.occupied;
					blockerSquare = sskLastOneIndex(blockers);
					attacks = attacks ^ sskBitmapWithSouthFile(blockerSquare);
				}
			}
            
			break;
		}
			
		case sskChessPieceBishop: {
			if (SSK_GET_RANK_IDX(fromSquare) < SSK_GET_RANK_IDX(toSquare)) {
				if (SSK_GET_FILE_IDX(fromSquare) < SSK_GET_FILE_IDX(toSquare)) {	// North East
					attacks = sskBitmapWithNorthEastDiagonal(fromSquare);
					blockers = attacks & bitboardPosition.occupied;
					blockerSquare = sskFirstOneIndex(blockers);
					attacks = attacks ^ sskBitmapWithNorthEastDiagonal(blockerSquare);
				} else if (SSK_GET_FILE_IDX(fromSquare) > SSK_GET_FILE_IDX(toSquare)) {	// North West
					attacks = sskBitmapWithNorthWestDiagonal(fromSquare);
					blockers = attacks & bitboardPosition.occupied;
					blockerSquare = sskFirstOneIndex(blockers);
					attacks = attacks ^ sskBitmapWithNorthWestDiagonal(blockerSquare);
				}
			} else if (SSK_GET_RANK_IDX(fromSquare) > SSK_GET_RANK_IDX(toSquare)) {
				if (SSK_GET_FILE_IDX(fromSquare) < SSK_GET_FILE_IDX(toSquare)) {	// South East
					attacks = sskBitmapWithSouthEastDiagonal(fromSquare);
					blockers = attacks & bitboardPosition.occupied;
					blockerSquare = sskLastOneIndex(blockers);
					attacks = attacks ^ sskBitmapWithSouthEastDiagonal(blockerSquare);
				} else if (SSK_GET_FILE_IDX(fromSquare) > SSK_GET_FILE_IDX(toSquare)) {	// South West
					attacks = sskBitmapWithSouthWestDiagonal(fromSquare);
					blockers = attacks & bitboardPosition.occupied;
					blockerSquare = sskLastOneIndex(blockers);
					attacks = attacks ^ sskBitmapWithSouthWestDiagonal(blockerSquare);
				}
			}
			
			break;
		}
			
		case sskChessPieceKnight: {
			attacks = sskBitmapWithKnightReach(fromSquare);
			break;
		}
	}
    
    return attacks;
}

sskBitmap sskBitmapForAllPieceAttacksInBitboardPosition(sskBitboardPosition bitboardPosition, sskChessPiece pieceCode, sskChessSquare fromSquare) {
    sskBitmap pieceBitmap, attacks = SSK_EMPTY_BITMAP, blockers, tempAttacks = SSK_EMPTY_BITMAP;
    sskChessSquare curFromSquare, blockerSquare;
    sskChessColor color = SSK_GET_PIECE_COLOR(pieceCode);
    
    pieceBitmap = sskBitmapForPieceInBitboardPosition(bitboardPosition, pieceCode);
    
    while (pieceBitmap) {
        curFromSquare = sskFirstOneIndex(pieceBitmap);
        pieceBitmap = pieceBitmap & SSK_BITMAP_UNSET_SQUARE_IDX(curFromSquare);
        
        if (curFromSquare != fromSquare) continue;
        
        switch (SSK_GET_GENERIC_PIECE_CODE(pieceCode)) {
            case sskChessPiecePawn: {
                // Same File
                tempAttacks = sskBitmapWithPawnReach(curFromSquare, color) & sskBitmapWithFileMask(SSK_GET_FILE_IDX(curFromSquare));
                blockers = tempAttacks & bitboardPosition.occupied;
                blockerSquare = sskFirstOneIndex(blockers);
                tempAttacks = tempAttacks ^ (sskBitmapWithPawnReach(blockerSquare, color) & sskBitmapWithFileMask(SSK_GET_FILE_IDX(curFromSquare)));
                tempAttacks = tempAttacks & SSK_BITMAP_UNSET_SQUARE_IDX(blockerSquare);
                
                attacks |= tempAttacks;
                
                // Capture Files (Attack exists only if opponent piece exists
                tempAttacks = sskBitmapWithPawnReach(curFromSquare, color) & ~sskBitmapWithFileMask(SSK_GET_FILE_IDX(curFromSquare));
                tempAttacks = tempAttacks & ((color == sskChessColorWhite)?bitboardPosition.bOccupied:bitboardPosition.wOccupied);
                
                attacks |= tempAttacks;
                
                break;
            }
            
            case sskChessPieceKing: {
                attacks = sskBitmapWithKingReach(curFromSquare);
                break;
            }
            case sskChessPieceQueen: {
                
                // East
                tempAttacks = sskBitmapWithEastRank(fromSquare);
                blockers = tempAttacks & bitboardPosition.occupied;
                blockerSquare = sskFirstOneIndex(blockers);
                tempAttacks = tempAttacks ^ sskBitmapWithEastRank(blockerSquare);
                
                attacks |= tempAttacks;
                
                // West
                tempAttacks = sskBitmapWithWestRank(fromSquare);
                blockers = tempAttacks & bitboardPosition.occupied;
                blockerSquare = sskLastOneIndex(blockers);
                tempAttacks = tempAttacks ^ sskBitmapWithWestRank(blockerSquare);
                
                attacks |= tempAttacks;
                
                
                // North
                
                tempAttacks = sskBitmapWithNorthFile(fromSquare);
                blockers = tempAttacks & bitboardPosition.occupied;
                blockerSquare = sskFirstOneIndex(blockers);
                tempAttacks = tempAttacks ^ sskBitmapWithNorthFile(blockerSquare);
                
                attacks |= tempAttacks;
                
                // South
                tempAttacks = sskBitmapWithSouthFile(fromSquare);
                blockers = tempAttacks & bitboardPosition.occupied;
                blockerSquare = sskLastOneIndex(blockers);
                tempAttacks = tempAttacks ^ sskBitmapWithSouthFile(blockerSquare);
                
                attacks |= tempAttacks;
                
                // North East
                tempAttacks = sskBitmapWithNorthEastDiagonal(fromSquare);
                blockers = tempAttacks & bitboardPosition.occupied;
                blockerSquare = sskFirstOneIndex(blockers);
                tempAttacks = tempAttacks ^ sskBitmapWithNorthEastDiagonal(blockerSquare);
                
                attacks |= tempAttacks;
                
                // North West
                tempAttacks = sskBitmapWithNorthWestDiagonal(fromSquare);
                blockers = tempAttacks & bitboardPosition.occupied;
                blockerSquare = sskFirstOneIndex(blockers);
                tempAttacks = tempAttacks ^ sskBitmapWithNorthWestDiagonal(blockerSquare);
                
                attacks |= tempAttacks;
                
                // South East
                tempAttacks = sskBitmapWithSouthEastDiagonal(fromSquare);
                blockers = tempAttacks & bitboardPosition.occupied;
                blockerSquare = sskLastOneIndex(blockers);
                tempAttacks = tempAttacks ^ sskBitmapWithSouthEastDiagonal(blockerSquare);
                
                attacks |= tempAttacks;
                
                // South West
                tempAttacks = sskBitmapWithSouthWestDiagonal(fromSquare);
                blockers = tempAttacks & bitboardPosition.occupied;
                blockerSquare = sskLastOneIndex(blockers);
                tempAttacks = tempAttacks ^ sskBitmapWithSouthWestDiagonal(blockerSquare);
                
                attacks |= tempAttacks;
                break;
            }
                
            case sskChessPieceRook: {
                // Calculate attacks and blockers in each direction and keep appending to attacks.

                // East
                tempAttacks = sskBitmapWithEastRank(fromSquare);
                blockers = tempAttacks & bitboardPosition.occupied;
                blockerSquare = sskFirstOneIndex(blockers);
                tempAttacks = tempAttacks ^ sskBitmapWithEastRank(blockerSquare);
                
                attacks |= tempAttacks;
 
                // West
                tempAttacks = sskBitmapWithWestRank(fromSquare);
                blockers = tempAttacks & bitboardPosition.occupied;
                blockerSquare = sskLastOneIndex(blockers);
                tempAttacks = tempAttacks ^ sskBitmapWithWestRank(blockerSquare);
                
                attacks |= tempAttacks;
                
                
                // North

                tempAttacks = sskBitmapWithNorthFile(fromSquare);
                blockers = tempAttacks & bitboardPosition.occupied;
                blockerSquare = sskFirstOneIndex(blockers);
                tempAttacks = tempAttacks ^ sskBitmapWithNorthFile(blockerSquare);
                
                attacks |= tempAttacks;
                
                // South
                tempAttacks = sskBitmapWithSouthFile(fromSquare);
                blockers = tempAttacks & bitboardPosition.occupied;
                blockerSquare = sskLastOneIndex(blockers);
                tempAttacks = tempAttacks ^ sskBitmapWithSouthFile(blockerSquare);
                
                attacks |= tempAttacks;
                
                break;
            }
                
            case sskChessPieceBishop: {
                // Calculate attacks and blockers in each direction and keep appending to attacks.
                
                // North East
                tempAttacks = sskBitmapWithNorthEastDiagonal(fromSquare);
                blockers = tempAttacks & bitboardPosition.occupied;
                blockerSquare = sskFirstOneIndex(blockers);
                tempAttacks = tempAttacks ^ sskBitmapWithNorthEastDiagonal(blockerSquare);
                
                attacks |= tempAttacks;
            
                // North West
                tempAttacks = sskBitmapWithNorthWestDiagonal(fromSquare);
                blockers = tempAttacks & bitboardPosition.occupied;
                blockerSquare = sskFirstOneIndex(blockers);
                tempAttacks = tempAttacks ^ sskBitmapWithNorthWestDiagonal(blockerSquare);
                
                attacks |= tempAttacks;
            
                // South East
                tempAttacks = sskBitmapWithSouthEastDiagonal(fromSquare);
                blockers = tempAttacks & bitboardPosition.occupied;
                blockerSquare = sskLastOneIndex(blockers);
                tempAttacks = tempAttacks ^ sskBitmapWithSouthEastDiagonal(blockerSquare);
                
                attacks |= tempAttacks;
                
                // South West
                tempAttacks = sskBitmapWithSouthWestDiagonal(fromSquare);
                blockers = tempAttacks & bitboardPosition.occupied;
                blockerSquare = sskLastOneIndex(blockers);
                tempAttacks = tempAttacks ^ sskBitmapWithSouthWestDiagonal(blockerSquare);

                attacks |= tempAttacks;
                break;
            }
                
            case sskChessPieceKnight: {
                attacks = sskBitmapWithKnightReach(curFromSquare) & ~((color == sskChessColorWhite)?bitboardPosition.wOccupied:bitboardPosition.bOccupied);
                break;
            }
        }
        
        if (curFromSquare == fromSquare) break;
    }
    
    if(SSK_GET_GENERIC_PIECE_CODE(pieceCode) != sskChessPiecePawn && SSK_GET_GENERIC_PIECE_CODE(pieceCode) != sskChessPieceKnight) {
        attacks = attacks ^ ((color == sskChessColorWhite)?bitboardPosition.wOccupied:bitboardPosition.bOccupied);
    }
    return attacks;
}

sskBitboardPosition * sskxFEN2BitboardPosition(const char * xFENstring) {
	sskBitboardPosition * pos = NULL;
	char * ptr = (char *)xFENstring;
	int i, j;
	
	pos = malloc(sizeof(sskBitboardPosition));
	
	// clear the position
	memset(pos, 0, sizeof(sskBitboardPosition));
	
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
					
				case 'p': pos->bPawn |= SSK_BITMAP_SET_SQUARE_IDX( (i * 8) + j); break;
				case 'k': pos->bKing |= SSK_BITMAP_SET_SQUARE_IDX( (i * 8) + j); break;
				case 'q': pos->bQueen |= SSK_BITMAP_SET_SQUARE_IDX( (i * 8) + j); break;
				case 'r': pos->bRook |= SSK_BITMAP_SET_SQUARE_IDX( (i * 8) + j); break;
				case 'b': pos->bBishop |= SSK_BITMAP_SET_SQUARE_IDX( (i * 8) + j); break;
				case 'n': pos->bKnight |= SSK_BITMAP_SET_SQUARE_IDX( (i * 8) + j); break;
					
				case 'P': pos->wPawn |= SSK_BITMAP_SET_SQUARE_IDX( (i * 8) + j); break;
				case 'K': pos->wKing |= SSK_BITMAP_SET_SQUARE_IDX( (i * 8) + j); break;
				case 'Q': pos->wQueen |= SSK_BITMAP_SET_SQUARE_IDX( (i * 8) + j); break;
				case 'R': pos->wRook |= SSK_BITMAP_SET_SQUARE_IDX( (i * 8) + j); break;
				case 'B': pos->wBishop |= SSK_BITMAP_SET_SQUARE_IDX( (i * 8) + j); break;
				case 'N': pos->wKnight |= SSK_BITMAP_SET_SQUARE_IDX( (i * 8) + j); break;
			}
			
			ptr += 1;
		}
	}
	
	pos->wOccupied = pos->wPawn | pos->wKing | pos->wQueen | pos->wRook | pos->wBishop | pos->wKnight;
	pos->bOccupied = pos->bPawn | pos->bKing | pos->bQueen | pos->bRook | pos->bBishop | pos->bKnight;
	pos->occupied = pos->wOccupied | pos->bOccupied;
	
	return pos;
}

sskBitboardPosition * sskCopyBitboardPosition(sskBitboardPosition bitboardPosition) {
    sskBitboardPosition * copyBitboardPosition = malloc(sizeof(sskBitboardPosition));

    copyBitboardPosition->wPawn = bitboardPosition.wPawn;
    copyBitboardPosition->wKing = bitboardPosition.wKing;
    copyBitboardPosition->wQueen = bitboardPosition.wQueen;
    copyBitboardPosition->wRook = bitboardPosition.wRook;
    copyBitboardPosition->wBishop = bitboardPosition.wBishop;
    copyBitboardPosition->wKnight = bitboardPosition.wKnight;
    
    copyBitboardPosition->bPawn = bitboardPosition.bPawn;
    copyBitboardPosition->bKing = bitboardPosition.bKing;
    copyBitboardPosition->bQueen = bitboardPosition.bQueen;
    copyBitboardPosition->bRook = bitboardPosition.bRook;
    copyBitboardPosition->bBishop = bitboardPosition.bBishop;
    copyBitboardPosition->bKnight = bitboardPosition.bKnight;
    
    copyBitboardPosition->wOccupied = bitboardPosition.wOccupied;
    copyBitboardPosition->bOccupied = bitboardPosition.bOccupied;
    copyBitboardPosition->occupied = bitboardPosition.occupied;
    
	// Do not replace this function with memcpy(), it takes longer!
	
    return copyBitboardPosition;
}

void sskClearBitboardPosition(sskBitboardPosition * bitboardPosition) {
	bitboardPosition->wPawn = SSK_EMPTY_BITMAP;
	bitboardPosition->wKing = SSK_EMPTY_BITMAP;
	bitboardPosition->wQueen = SSK_EMPTY_BITMAP;
	bitboardPosition->wRook = SSK_EMPTY_BITMAP;
	bitboardPosition->wBishop = SSK_EMPTY_BITMAP;
	bitboardPosition->wKnight = SSK_EMPTY_BITMAP;
	
	bitboardPosition->bPawn = SSK_EMPTY_BITMAP;
	bitboardPosition->bKing = SSK_EMPTY_BITMAP;
	bitboardPosition->bQueen = SSK_EMPTY_BITMAP;
	bitboardPosition->bRook = SSK_EMPTY_BITMAP;
	bitboardPosition->bBishop = SSK_EMPTY_BITMAP;
	bitboardPosition->bKnight = SSK_EMPTY_BITMAP;
	
	bitboardPosition->wOccupied = SSK_EMPTY_BITMAP;
	bitboardPosition->bOccupied = SSK_EMPTY_BITMAP;
	bitboardPosition->occupied = SSK_EMPTY_BITMAP;
}

void sskPrintBitmap(sskBitmap bitmap) {
	int i, j;
	sskBitmap rank;
	
	printf("\n");
	// outer loop takes a specif rank and shifts to first byte.
	// inner loop prints the first byte from LSB.
	for (i = 7; i >= 0; i--) {
		rank = (bitmap >> (i * 8));
		for (j = 0; j < 8; j++) {
			printf("%c ", ((rank >> j) & 1)?'1':'-');
		}
		printf("\n");
	}
}

void sskPrintBitboardPosition(sskBitboardPosition bitboardPosition) {
	char posStr[65];
	int i, j;
	
	for (i = 0; i < 64; i++) posStr[i] = '-';
	posStr[64] = '\0';
	
	for (i = 0; i < 64; i++) posStr[i] = ((bitboardPosition.wPawn >> i) & 1)?'P':posStr[i];
	for (i = 0; i < 64; i++) posStr[i] = ((bitboardPosition.wKing >> i) & 1)?'K':posStr[i];
	for (i = 0; i < 64; i++) posStr[i] = ((bitboardPosition.wQueen >> i) & 1)?'Q':posStr[i];
	for (i = 0; i < 64; i++) posStr[i] = ((bitboardPosition.wRook >> i) & 1)?'R':posStr[i];
	for (i = 0; i < 64; i++) posStr[i] = ((bitboardPosition.wBishop >> i) & 1)?'B':posStr[i];
	for (i = 0; i < 64; i++) posStr[i] = ((bitboardPosition.wKnight >> i) & 1)?'N':posStr[i];
	
	for (i = 0; i < 64; i++) posStr[i] = ((bitboardPosition.bPawn >> i) & 1)?'p':posStr[i];
	for (i = 0; i < 64; i++) posStr[i] = ((bitboardPosition.bKing >> i) & 1)?'k':posStr[i];
	for (i = 0; i < 64; i++) posStr[i] = ((bitboardPosition.bQueen >> i) & 1)?'q':posStr[i];
	for (i = 0; i < 64; i++) posStr[i] = ((bitboardPosition.bRook >> i) & 1)?'r':posStr[i];
	for (i = 0; i < 64; i++) posStr[i] = ((bitboardPosition.bBishop >> i) & 1)?'b':posStr[i];
	for (i = 0; i < 64; i++) posStr[i] = ((bitboardPosition.bKnight >> i) & 1)?'n':posStr[i];
	
	printf("\n");
	for (i = 7; i >= 0; i--) {
		for (j = 0; j < 8; j++) {
			printf("%c ", posStr[ (8 * i) + j ]);
		}
		printf("\n");
	}
}