#ifndef TERRAIN_DOT_H
#define TERRAIN_DOT_H

#define ROWS 21
#define COLUMNS 80

typedef struct Block
{
    int n;
    int s;
    int e;
    int w;

    int x;
    int y;

    int visited;

    char tile[ROWS][COLUMNS];
} block;

void visit2(block *full[399][399], int row, int column);

void visit(block *map, block *toNorth, block *toSouth, block *toEast, block *toWest);
void print_map(block *map);

#endif