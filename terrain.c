#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "terrain.h"

// generates the rock boarder
void edge_gen(block *map)
{
    int row, column;

    // creates the border of impassible rocks
    for (row = 0; row < ROWS; row++)
    {
        for (column = 0; column < COLUMNS; column++)
        {
            if (column == 0 || row == 0 || column == COLUMNS - 1 || row == ROWS - 1)
                map->tile[row][column] = '%';
            else
                map->tile[row][column] = ' ';
        }
    }
}

void path_gen(block *map)
{
    int row, column;
    if (map->n != -1)
        column = map->n;
    else
    {
        column = rand() % COLUMNS;
        map->n = column;
    }

    int random = rand() % 12;

    // N to S path
    for (row = 0; row < ROWS; row++)
    {
        map->s = column;
        if (row < ROWS - 3)
        {
            if (column == 0)
                column++;
            else if (column == ROWS - 1)
                column--;

            map->tile[row][column] = '#';
            if (random % 3 == 0 && column < COLUMNS - 1)
                column += (rand() % 3) - 1;

            random = rand() % 12;
        }

        // conects to the correct end
        else
        {
            while (column < map->s)
            {
                map->tile[row][column] = '#';
                column++;
            }

            while (column > map->s)
            {
                map->tile[row][column] = '#';
                column--;
            }
            map->tile[row][column] = '#';
        }
    }

    if (map->w != -1)
        row = map->w;
    else
    {
        row = rand() % ROWS;
        map->w = row;
    }

    // W to E
    for (column = 0; column < COLUMNS; column++)
    {
        map->e = row;
        if (column < COLUMNS - 3)
        {
            if (row == 0)
                row++;
            else if (row == ROWS - 1)
                row--;

            map->tile[row][column] = '#';
            if (random % 3 == 0 && row < ROWS - 1)
                row += (rand() % 3) - 1;

            random = rand() % 12;
        }

        // conects to the correct end
        else
        {
            while (row < map->e)
            {
                map->tile[row][column] = '#';
                row++;
            }

            while (row > map->e)
            {
                map->tile[row][column] = '#';
                row--;
            }
            map->tile[row][column] = '#';
        }
    }
}

// generates patches of different types
void patch_gen(int size, int startR, int startC, char c, block *map)
{
    int row;
    int column;

    if (startR == 0)
        startR += 1;

    if (startC == 0)
        startC += 1;

    for (row = startR; (row < size + startR) && row < ROWS - 1; row++)
    {
        for (column = startC; (column < size + startC) && column < COLUMNS - 1; column++)
        {
            if (map->tile[row][column] == ' ')
                map->tile[row][column] = c;
        }
    }
}

// builds the different buildings
void building_gen(char c, block *map)
{
    int row = (rand() % ROWS - 4);
    if (row < 1)
        row = 2;
    int column = (rand() % COLUMNS - 4);
    if (column < 1)
        column = 2;

    int i, j;

    while (map->tile[row - 1][column] != '#' && map->tile[row][column - 1] != '#' && map->tile[row][column - 1] != 'M' && map->tile[row][column - 1] != 'C' && row > 0 && row < ROWS && column > 0 && column < COLUMNS && map->tile[row + 1][column] != '#' && map->tile[row][column + 1] != '#' && map->tile[row][column + 1] != 'M' && map->tile[row][column + 1] != 'C')
    {
        row = ((rand() % ROWS) - 4);
        if (row < 1)
            row = 2;
        column = ((rand() % COLUMNS) - 4);
        if (column < 1)
            column = 2;
    }

    for (i = row; i < row + 4; i++)
    {
        for (j = column; j < column + 6; j++)
        {
            if (i == row || j == column || i == row + 3 || j == column + 5)
            {
                if (map->tile[i][j] != '%')
                    map->tile[i][j] = '#';
            }
            else
            {
                if (map->tile[i][j] != '%')
                    map->tile[i][j] = c;
            }
        }
    }
}

// generates the map
void gen_terrain(block *map)
{
    edge_gen(map);

    int size = (rand() % 10) + 3;
    int row = (rand() % 5) + 2;
    int column = (rand() % 10) + 2;
    int number = (rand() % 20) + 5;

    int what = rand() % 3;

    int i;
    for (i = 0; i < 2; i++)
    {
        patch_gen(size, row, column, ',', map);

        size = (rand() % 10) + 2;
        row = (rand() % ROWS - 2) + 1;
        column = (rand() % COLUMNS - 2) + 1;
        what = (rand() % 3);
    }

    // generates other objects
    for (i = 0; i < number; i++)
    {

        if (what == 0)
        {
            if (row > 0 && column > 0)
                map->tile[row][column] = '%';
        }
        else if (what == 1)
        {
            if (row > 0 && column > 0 && map != NULL)
                map->tile[row][column] = '"';
        }
        else
            patch_gen(size, row, column, ',', map);

        size = (rand() % 10) + 2;
        row = (rand() % ROWS - 2) + 1;
        column = (rand() % COLUMNS - 2) + 1;
        what = (rand() % 3);
    }
    path_gen(map);

    int d = abs(map->x) + abs(map->y);
    if (rand() % 100 > ((-45 * d / 200 + 50) / 100))
    {
        building_gen('M', map);
        building_gen('C', map);
    }

    // creates the border of impassible rocks
    for (row = 0; row < ROWS; row++)
    {
        for (column = 0; column < COLUMNS; column++)
        {
            if (map->tile[row][column] == ' ')
                map->tile[row][column] = '.';
        }
    }
}

void print_map(block *map)
{
    int row, column;

    // prints the map
    char pos;
    for (row = 0; row < ROWS; row++)
    {
        for (column = 0; column < COLUMNS; column++)
        {

            pos = map->tile[row][column];

            // curses library for color

            if (pos == 'C') // red
                printf("\033[1;31m");
            else if (pos == '.') // green
                printf("\033[0;32m");
            else if (pos == 'M') // blue
                printf("\033[0;34m");
            else if (pos == '#') // white
                printf("\033[0;37m");
            else if (pos == '"') // yellow
                printf("\033[0;33m");
            else if (pos == '%') // black
                printf("\e[0;34m");
            else if (pos == ',') // bright green
                printf("\033[92;102m");
            else
                printf("\e[0m");

            printf("%c", pos);
        }
        printf("\n");
    }
}

void visit(block *map, block *toNorth, block *toSouth, block *toEast, block *toWest)
{
    if (map == NULL)
    {
        if (toNorth != NULL)
            map->n = toNorth->s;
        else
            map->n = -1;

        if (toSouth != NULL)
            map->s = toSouth->n;
        else
            map->s = -1;

        if (toEast != NULL)
            map->e = toEast->w;
        else
            map->e = -1;

        if (toWest != NULL)
            map->w = toWest->e;
        else
            map->w = -1;

        gen_terrain(map);
    }
}

void visit2(block *full[399][399], int row, int column)
{
    if (full[row][column]->visited)
    {
        return;
    }

    // checking block north
    if (row > 0)
    {
        if (full[row - 1][column] != NULL)
            full[row][column]->n = full[row - 1][column]->s;
        else
            full[row][column]->n = -1;
    }
    else
        full[row][column]->n = -1;

    // checking for block south
    if (row < 399)
    {
        if (full[row + 1][column] != NULL)
            full[row][column]->s = full[row + 1][column]->n;
        else
            full[row][column]->s = -1;
    }
    else
        full[row][column]->s = -1;

    // checking block west
    if (column > 0)
    {
        if (full[row][column - 1] != NULL)
            full[row][column]->w = full[row][column - 1]->e;
        else
            full[row][column]->w = -1;
    }
    else
        full[row][column]->w = -1;

    // checking for block east
    if (column < 399)
    {
        if (full[row][column + 1] != NULL)
            full[row][column]->e = full[row][column + 1]->w;
        else
            full[row][column]->e = -1;
    }
    else
        full[row][column]->e = -1;

    full[row][column]->visited = 1;
    gen_terrain(full[row][column]);
}
