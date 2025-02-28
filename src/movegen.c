#pragma once

#include "bitboard.c"
#include <stdbool.h>
#include <stdint.h>

#define STARTER_RANK_WHITES 1
#define STARTER_RANK_BLACKS 6
#define POSSIBLE_HORSE_MOVES 8

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

    setPiece(&legalMoves, forward, false);
    forward.rank += isWhite ? 1 : -1;
    areBlockingPieces = getSquare(otherPieces, forward); // Recalculate

    if (inStarterRank && forward.rank < BOARD_LENGTH && !areBlockingPieces) {
      setPiece(&legalMoves, forward, false);
    }
  }

  return legalMoves;
}

// TODO: En passant
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

// To get the kills, you can just `and` the legal moves with the rival's board
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
