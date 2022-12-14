#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define BUFFER_SIZE 1024
#define MAZE_CAPACITY_INCREASE 10
#define CHAR_WALL 'X'
#define CHAR_START 'S'
#define CHAR_END 'E'
#define CHAR_SPACE ' '

typedef struct TCoord {
  size_t x;
  size_t y;
  TCoord* prev;
} TCoord;

typedef struct TCoordBuffer {
  size_t count;
  size_t capacity;
  TCoord* data;
} TCoordBuffer;

typedef struct TLine {
  char* data;
  size_t length;
} TLine;

typedef struct TMaze {
  TLine* data;
  size_t height;
  size_t width;
  size_t capacity;
  TCoord* start;
  TCoord* end;
} TMaze;

int readLine(TLine* line);
int addLineToMaze(TMaze* maze, TLine* line);
void disposeLine(TLine* line);
void disposeMaze(TMaze* maze);
int readMaze(TMaze* maze);
void printMaze(TMaze* maze);
int parseMaze(TMaze* maze, int* errorCode);
void printParseErrorCode(int errorCode);

int main(void) {
  int err = 0;
  TMaze maze = {0};

  // Read the maze from stdin
  err = !readMaze(&maze);

  // Parse the maze from buffers
  int parseErrorCode = 0;
  if (!err) {
    err = !parseMaze(&maze, &parseErrorCode);
  }

  if (err) {
    fprintf(stderr, "Wrong input.\n");

    if (parseErrorCode) {
      printParseErrorCode(parseErrorCode);
    }
  }

  disposeMaze(&maze);

  // TODO: get rid of this after we return the path insctuctions
  printf("\n");

  return !err;
}

int readLine(TLine* line) {
  int res = 1;
  char buffer[BUFFER_SIZE];

  if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) return EOF;

  if (buffer[0] == '\n') return EOF;

  line->length = strlen(buffer);
  if (line->length <= 1) return EOF;
  if (buffer[line->length - 1] != '\n') {
    res = EOF;
    line->length++;
  }
  line->data = (char*)realloc(line->data, line->length * sizeof(char));
  strncpy(line->data, buffer, line->length - (res == EOF));
  if (res != EOF) {
    line->data[line->length - 1] = '\0';
  }

  return 1;
}

int addLineToMaze(TMaze* maze, TLine* line) {
  if (maze->height == maze->capacity) {
    maze->capacity += MAZE_CAPACITY_INCREASE;
    maze->data = (TLine*)realloc(maze->data, maze->capacity * sizeof(TLine));

    if (maze->data == NULL) return 0;  // realloc failed
  }

  maze->data[maze->height++] = *line;
  return 1;
}

void disposeLine(TLine* line) { free(line->data); }

void disposeMaze(TMaze* maze) {
  for (size_t i = 0; i < maze->height; i++) {
    disposeLine(&maze->data[i]);
  }

  free(maze->data);
}

int readMaze(TMaze* maze) {
  TLine nextLine = {0};
  int res, err = 0;

  while (1) {
    res = readLine(&nextLine);

    // If something was read, validate it and save as a next line
    if (nextLine.data != NULL) {
      if (maze->height > 0 &&
          nextLine.length != maze->width)
        err = 1;

      if (!err && !addLineToMaze(maze, &nextLine)) err = 1;
    }

    // Invalid input or EOF
    if (err || res != 1) break;

    if (maze->height == 1) {
      maze->width = nextLine.length;
    }

    nextLine = (TLine){0};
  }

  if (err) {
    disposeLine(&nextLine);
    return 0;
  }

  return 1;
}

void printMaze(TMaze* maze) {
  for (size_t i = 0; i < maze->height; i++) {
    printf("\r%s\n", maze->data[i].data);
  }
}

void printParseErrorCode(int errorCode) {
  printf("Failed to parse the maze: ");
  switch (errorCode) {
    case 2:
      printf("broken frame");
      break;
    case 3:
      printf("multiple start/end points");
      break;
    case 4:
      printf("unallowed character");
      break;
    case 5:
      printf("missing start/end point");
      break;
  }
  printf("\n");
}

/// @note Error codes:
/// 2 - broken frame
/// 3 - multiple start/stop points
/// 4 - unallowed character
/// 5 - missing start/stop point
int parseMaze(TMaze* maze, int* errorCode) {
  // Validate the structure and find the start and end points
  for (size_t l = 0; l < maze->height; l++) {
    for (size_t i = 0; i < maze->width; i++) {
      char c = maze->data[l].data[i];

      // Check the integrity of the frame
      // ->top and bottom
      if ((l == 0 || l == (maze->height - 1)) && i != (maze->width - 1) && c != CHAR_WALL) {
        *errorCode = 2;
        return 0;
      }
      // ->left and right sides
      // note: the right side contains of '\0's, so where the wall
      // is supposed to be is one step to the left
      if ((i == 0 || i == (maze->width - 2)) && c != CHAR_WALL) {
        *errorCode = 2;
        return 0;
      }

      // Find the start and end points
      // -> start
      if (c == CHAR_START) {
        if (maze->start == NULL) {
          TCoord* start = (TCoord*)malloc(sizeof(TCoord));
          start->prev = NULL;
          start->x = i;
          start->y = l;
          maze->start = start;
          continue;
        } else {
          *errorCode = 3;
          return 0;
        }
      }
      // -> end
      if (c == CHAR_END) {
        if (maze->end == NULL) {
          TCoord* end = (TCoord*)malloc(sizeof(TCoord));
          end->prev = NULL;
          end->x = i;
          end->y = l;
          maze->end = end;
          continue;
        } else {
          *errorCode = 3;
          return 0;
        }
      }

      // The last remaining could only be either a space or a wall,
      // if it is something else, then that character is not allowed
      if (c != CHAR_SPACE && c != CHAR_WALL && c != '\0') {
        *errorCode = 4;
        return 0;
      }
    }
  }

  // Check if start and end points were found
  if (maze->start == NULL || maze->end == NULL) {
    *errorCode = 5;
    return 0;
  }

  // The structure is OK
  return 1;
}
