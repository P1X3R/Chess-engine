#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define BOARD_LENGTH 8
#define PIECE_TYPES 6

typedef enum { pawn, knight, bishop, rook, queen, king } ChessPiece;
typedef uint64_t BitBoard;

typedef struct {
  BitBoard whites[PIECE_TYPES], blacks[PIECE_TYPES];
} ChessBoard;

typedef struct {
  uint8_t rank, file; // The range is 0-7
} Position;

bool getSquare(const BitBoard *board, const Position position) {
  if (position.rank < 0 || position.rank >= 7 || position.file < 0 ||
      position.file >= 7 || NULL == board)
    return false;

  const uint8_t square = position.rank * BOARD_LENGTH + position.file;

  return (1 << (square - 1)) & 1;
}

void setPiece(BitBoard *board, const Position position, const bool clear) {
  if (position.rank < 0 || position.rank >= 7 || position.file < 0 ||
      position.file >= 7 || NULL == board)
    return;

  const uint8_t square = position.rank * BOARD_LENGTH + position.file;

  if (clear)
    *board &= ~(1 << square); // Clear the bit
  else
    *board |= 1 << square; // Set the bit
}
