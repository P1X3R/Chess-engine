#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

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

// Debug utility to print a bitboard as an 8x8 grid
void printBitBoard(const BitBoard board) {
  printf("  a b c d e f g h\n");
  for (int rank = 7; rank >= 0; rank--) {
    printf("%d ", rank + 1);
    for (int file = 0; file < 8; file++) {
      Position pos = {.rank = rank, .file = file};
      bool isSet = (board >> (rank * BOARD_LENGTH + file)) & 1;
      printf("%c ", isSet ? '1' : '.');
    }
    printf("%d\n", rank + 1);
  }
  printf("  a b c d e f g h\n");
  printf("BitBoard value: 0x%016llx\n", (unsigned long long)board);
}

// Function to get all set positions from a bitboard
size_t getBitBoardPositions(const BitBoard board, Position *positions,
                            size_t maxCount) {
  size_t count = 0;
  for (int rank = 0; rank < BOARD_LENGTH; rank++) {
    for (int file = 0; file < BOARD_LENGTH; file++) {
      if ((board >> (rank * BOARD_LENGTH + file)) & 1) {
        if (count < maxCount) {
          positions[count].rank = rank;
          positions[count].file = file;
          count++;
        }
      }
    }
  }
  return count;
}

// Get number of pieces on the board
uint8_t popCount(BitBoard board) {
  uint8_t count = 0;
  while (board) {
    count++;
    board &= (board - 1); // Clear the least significant bit set
  }

  return count;
}
