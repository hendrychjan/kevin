#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define BUFFER_SIZE 1024
#define MAZE_CAPACITY_INCREASE 10

typedef struct TLine {
  char* data;
  size_t length;
} TLine;

typedef struct TMaze {
  TLine* data;
  size_t height;
  size_t capacity;
} TMaze;

int readLine(TLine* line);
int addLineToMaze(TMaze* maze, TLine* line);
void disposeLine(TLine* line);
void disposeMaze(TMaze* maze);
int readMaze(TMaze* maze);
void printMaze(TMaze* maze);

int main(void) {
  int err = 0;
  TMaze maze = {0};

  err = !readMaze(&maze);

  if (!err) printMaze(&maze);

  if (err) {
    fprintf(stderr, "Wrong input.\n");
  }

  disposeMaze(&maze);

  return !err;
}

int readLine(TLine* line) {
  int res = 1;
  char buffer[BUFFER_SIZE];

  if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) return EOF;

  if (buffer[0] == '\n') return EOF;

  line->length = strlen(buffer);
  if (line->length >= 1) return EOF;
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

void disposeMaze(TMaze* maze) {
  for (size_t i = 0; i < maze->height; i++) {
    disposeLine(&maze->data[i]);
  }

  free(maze->data);
}

void disposeLine(TLine* line) { free(line->data); }

int readMaze(TMaze* maze) {
  TLine nextLine = {0};
  int res, err = 0;

  while (1) {
    res = readLine(&nextLine);
    
    // If something was read, validate it and save as a next line
    if (nextLine.data != NULL) {
      if (maze->height > 0 &&
          nextLine.length != maze->data[maze->height - 1].length)
        err = 1;

      if (!err && !addLineToMaze(maze, &nextLine)) err = 1;
    }

    // Invalid input or EOF
    if (err || res != 1) break;

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
    printf("%s\n", maze->data[i].data);
  }
}
