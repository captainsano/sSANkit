/**
 *	@file
 *	Declares utility functions for converting between board formats.
 *
 *	@author Santhosbaala RS
 *	@copyright 2012 64cloud
 *	@version 1.0
 */

#ifndef boardformatconvertutil_h
#define boardformatconvertutil_h

#include "bitboard.h"
#include "offsetboard.h"

/**
 *	Function converts an offset position to a bitboard position. Memory deallocation
 *	is the responsbility of the caller.
 *
 *	@param offsetPosition The position in offset format.
 *
 *	@return A pointer to the newly created bitboard position or NULL on failure.
 */
sskBitboardPosition * sskOffsetPositionToBitboardPosition(sskOffsetPosition offsetPosition);

/**
 *	Function converts a bitboard position to an offset position. Memory deallocation
 *	is the responsibility of the caller.
 *
 *	@param bitboardPosition The position in bitboard format.
 *
 *	@return A pointer to the newly created offset position or NULL on failure.
 */
sskOffsetPosition sskBitboardPositionToOffsetPosition(sskBitboardPosition bitboardPosition);

/**
 *	Function fills the given piecePlacement string with the information from an offsetPosition.
 *
 *	@param piecePlacement The piecePlacement string to be filled.
 *	@param offsetPosition The position in offset format.
 */
void sskFillPiecePlacementWithOffsetPosition(char piecePlacement[65], sskOffsetPosition offsetPosition);

/**
 *	Function converts the given piece placement string to an offset position. Caller has to
 *	manage memory.
 *
 *	@param piecePlacement The piece placement string.
 *
 *	@return The offset position corresponding to the given piecePlacement string.
 */
sskOffsetPosition sskPiecePlacementStringToOffsetPosition(const char piecePlacement[65]);

/**
 *	Function converts the given piece placment string to a bitboard position. Caller has to
 *	manage memory.
 *
 *	@param piecePlacement The piece placement string.
 *
 *	@return A pointer to the bitboard corresponding to the given piecePlacement string.
 */
sskBitboardPosition * sskPiecePlacementStringToBitboardPosition(const char piecePlacement[65]);


#endif
