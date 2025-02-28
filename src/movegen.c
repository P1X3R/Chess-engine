#pragma once

#include "bitboard.c"
#include <stdbool.h>
#include <stdint.h>

#define STARTER_RANK_WHITES 1  // Rank where white pawns start
#define STARTER_RANK_BLACKS 6  // Rank where black pawns start
#define POSSIBLE_HORSE_MOVES 8 // Number of possible knight moves
#define BISHOP_DIRECTIONS 4    // Number of diagonal directions for a bishop

/**
 * Generates legal pawn moves.
 *
 * @param isWhite     True if the pawn is white, false if black.
 * @param position    The current position of the pawn.
 * @param otherPieces BitBoard representing positions of other pieces (friendly
 * and opponent).
 * @return            BitBoard representing legal pawn moves.
 */
BitBoard getPawnMoves(const bool isWhite, const Position position,
                      const BitBoard *otherPieces) {
  BitBoard legalMoves = 0;
  const bool inStarterRank = isWhite ? position.rank == STARTER_RANK_WHITES
                                     : position.rank == STARTER_RANK_BLACKS;
  Position forward = {
      .rank = isWhite ? position.rank + 1 : position.rank - 1,
      .file = position.file,
  };

  const bool isNotInEdge =
      forward.rank < BOARD_LENGTH && forward.file < BOARD_LENGTH;
  bool areBlockingPieces = getSquare(otherPieces, forward);

  if (isNotInEdge && !areBlockingPieces) {
    setPiece(&legalMoves, forward, false); // Set the forward move as legal
    forward.rank += isWhite ? 1 : -1;
    areBlockingPieces =
        getSquare(otherPieces, forward); // Check for double move block

    if (inStarterRank && forward.rank < BOARD_LENGTH && !areBlockingPieces) {
      setPiece(&legalMoves, forward, false);
    }
  }

  return legalMoves;
}

/**
 * Generates pawn capture moves.
 * @todo en passant
 *
 * @param isWhite  True if the pawn is white, false if black.
 * @param position The current position of the pawn.
 * @param rival    BitBoard representing the positions of opponent pieces.
 * @return         BitBoard representing pawn capture moves.
 */
BitBoard getPawnKills(const bool isWhite, const Position position,
                      const BitBoard *rival) {
  BitBoard kills = 0;
  const Position leftKill = {
      .rank = isWhite ? position.rank + 1 : position.rank - 1,
      .file = position.file - 1,
  };
  const Position rightKill = {
      .rank = leftKill.rank,
      .file = position.file + 1,
  };

  if (leftKill.rank >= 0 && leftKill.rank < BOARD_LENGTH) {
    if (leftKill.file >= 0 && leftKill.file < BOARD_LENGTH)
      setPiece(&kills, leftKill, false);

    if (rightKill.file >= 0 && rightKill.file < BOARD_LENGTH)
      setPiece(&kills, rightKill, false);
  }

  return kills;
}

/**
 * Generates legal knight moves.
 *
 * @param position The current position of the knight.
 * @return         BitBoard representing legal knight moves.
 */
BitBoard getKnightMoves(const Position position) {
  BitBoard legalMoves = 0;
  const Position possibleMoves[POSSIBLE_HORSE_MOVES] = {
      // Top Left
      (Position){.rank = position.rank + 2, .file = position.file - 1},

      // Top right
      (Position){.rank = position.rank + 2, .file = position.file + 1},

      // Bottom left
      (Position){.rank = position.rank - 2, .file = position.file - 1},

      // Bottom right
      (Position){.rank = position.rank - 2, .file = position.file + 1},

      // Left top
      (Position){.rank = position.rank + 1, .file = position.file - 2},

      // Left bottom
      (Position){.rank = position.rank - 1, .file = position.file - 2},

      // Right top
      (Position){.rank = position.rank + 1, .file = position.file + 2},

      // Right bottom
      (Position){.rank = position.rank - 1, .file = position.file + 2},
  };

  for (uint8_t i = 0; i < POSSIBLE_HORSE_MOVES; i++) {
    const bool isOutsideOfBoard =
        possibleMoves[i].rank < 0 || possibleMoves[i].rank > 7 ||
        possibleMoves[i].file < 0 || possibleMoves[i].file > 7;
    if (isOutsideOfBoard)
      continue;

    setPiece(&legalMoves, possibleMoves[i], false);
  }

  return legalMoves;
}

/**
 * Generates a BitBoard representing the legal moves for a bishop.
 *
 * @param position       The current position of the bishop.
 * @param friendlyPieces A BitBoard representing the positions of friendly
 * pieces.
 * @param opponentPieces A BitBoard representing the positions of opponent
 * pieces.
 * @return               A BitBoard representing the legal moves of the bishop.
 */
BitBoard generateBishopMoves(const Position bishopPosition,
                             const BitBoard *friendlyPieces,
                             const BitBoard *opponentPieces) {
  BitBoard legalMoves = 0;

  // Iterate through possible move distances
  for (uint8_t distance = 1; distance < BOARD_LENGTH; distance++) {
    // Define possible move offsets for each diagonal direction
    const Position moveOffsets[BISHOP_DIRECTIONS] = {
        {-distance, -distance}, // Up-left
        {distance, -distance},  // Down-left
        {-distance, distance},  // Up-right
        {distance, distance}    // Down-right
    };

    // Track if a direction is blocked
    bool directionBlocked[BISHOP_DIRECTIONS] = {false, false, false, false};

    // Iterate through each diagonal direction
    for (uint8_t direction = 0; direction < BISHOP_DIRECTIONS; direction++) {
      // Calculate the target position
      const Position targetPosition = {
          bishopPosition.rank + moveOffsets[direction].rank,
          bishopPosition.file + moveOffsets[direction].file};

      // Check if the target position is within the board boundaries
      const bool isOutsideBoard = targetPosition.rank >= BOARD_LENGTH ||
                                  targetPosition.file >= BOARD_LENGTH ||
                                  targetPosition.rank < 0 ||
                                  targetPosition.file < 0;

      // Check if the target square is occupied by a friendly piece
      const bool isFriendlyPiece = getSquare(friendlyPieces, targetPosition);

      // Check if the target square is occupied by an opponent piece
      const bool isOpponentPiece = getSquare(opponentPieces, targetPosition);

      // If the target position is outside the board or occupied by a friendly
      // piece, block the direction
      if (isOutsideBoard || isFriendlyPiece) {
        directionBlocked[direction] = true;
      }

      // If the direction is not blocked, set the target square as a legal move
      if (!directionBlocked[direction]) {
        setPiece(&legalMoves, targetPosition, true);
      }

      // If the target square is occupied by an opponent piece, block the
      // direction (capture)
      if (isOpponentPiece) {
        directionBlocked[direction] = true;
      }
    }
  }

  return legalMoves;
}
