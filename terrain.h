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

    char tile[ROWS][COLUMNS];
} block;

void visit(block *map, block *toNorth, block *toSouth, block *toEast, block *toWest);
void print_map(block *map);

#endif