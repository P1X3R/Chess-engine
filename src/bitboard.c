#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#define BOARD_LENGTH 8
#define PIECE_TYPES 6
#define POSITION_BOUNDS_CHECKING(RET)                                          \
  if (position.rank < 0 || position.rank >= 7 || position.file < 0 ||          \
      position.file >= 7 || NULL == board)                                     \
    return RET;

typedef enum { pawn, knight, bishop, rook, queen, king } ChessPiece;
typedef uint64_t BitBoard;

typedef struct {
  BitBoard whites[PIECE_TYPES], blacks[PIECE_TYPES];
} ChessBoard;

typedef struct {
  uint8_t rank, file; // The range is 0-7
} Position;

/**
 * Checks if a specific square on a BitBoard is set.
 *
 * @param board    Pointer to the BitBoard.
 * @param position The position of the square to check.
 * @return         True if the square is set, false otherwise.
 */
bool getSquare(const BitBoard *board, const Position position) {
  POSITION_BOUNDS_CHECKING((bool)false)
  const uint8_t square = position.rank * BOARD_LENGTH + position.file;

  return (1 << (square - 1)) & 1;
}

/**
 * Sets or clears a specific square on a BitBoard.
 *
 * @param board  Pointer to the BitBoard.
 * @param position The position of the square to set or clear.
 * @param clear  True to clear the square, false to set it.
 */
void setPiece(BitBoard *board, const Position position, const bool clear) {
  POSITION_BOUNDS_CHECKING()
  const uint8_t square = position.rank * BOARD_LENGTH + position.file;

  if (clear)
    *board &= ~(1 << square); // Clear the bit
  else
    *board |= 1 << square; // Set the bit
}

/**
 * Prints a BitBoard as an 8x8 grid for debugging purposes.
 *
 * @param board The BitBoard to print.
 */
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

/**
 * Gets all set positions from a BitBoard and stores them in an array.
 *
 * @param board     The BitBoard to process.
 * @param positions Array to store the positions.
 * @param maxCount  Maximum number of positions to store.
 * @return          The number of positions stored.
 */
size_t getBitBoardPositions(const BitBoard board, Position *positions,
                            size_t maxCount) {
  size_t count = 0;
  for (int rank = 0; rank < BOARD_LENGTH; rank++) {
    for (int file = 0; file < BOARD_LENGTH; file++) {

      // Check if the bit is set
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

/**
 * Counts the number of set bits in a BitBoard.
 *
 * @param board The BitBoard to process.
 * @return      The number of set bits.
 */
uint8_t popCount(BitBoard board) {
  uint8_t count = 0;
  while (board) {
    count++;
    board &= (board - 1); // Clear the least significant bit set
  }

  return count;
}
