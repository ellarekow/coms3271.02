#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "terrain.h"

#define ROWS 21
#define COLUMNS 80

int north, south, east, west;

char BLOCK[ROWS][COLUMNS];

// generates the rock boarder
void edge_gen()
{
    int row, column;

    // creates the border of impassible rocks
    for (row = 0; row < ROWS; row++)
    {
        for (column = 0; column < COLUMNS; column++)
        {
            if (column == 0 || row == 0 || column == COLUMNS - 1 || row == ROWS - 1)
                BLOCK[row][column] = '%';
            else
                BLOCK[row][column] = ' ';
        }
    }
}

void path_gen(int n, int s, int e, int w)
{
    int row;
    int column = n;

    int random = rand() % 12;

    // N to S path
    for (row = 0; row < ROWS; row++)
    {
        if (row < ROWS - 3)
        {
            if (column == 0)
                column++;
            else if (column == ROWS - 1)
                column--;

            BLOCK[row][column] = '#';
            if (random % 3 == 0 && column < COLUMNS - 1)
                column += (rand() % 3) - 1;

            random = rand() % 12;
        }

        // conects to the correct end
        else
        {
            while (column < s)
            {
                BLOCK[row][column] = '#';
                column++;
            }

            while (column > s)
            {
                BLOCK[row][column] = '#';
                column--;
            }
            BLOCK[row][column] = '#';
        }
    }

    row = w;

    // W to E
    for (column = 0; column < COLUMNS; column++)
    {
        if (column < COLUMNS - 3)
        {
            if (row == 0)
                row++;
            else if (row == ROWS - 1)
                row--;

            BLOCK[row][column] = '#';
            if (random % 3 == 0 && row < ROWS - 1)
                row += (rand() % 3) - 1;

            random = rand() % 12;
        }

        // conects to the correct end
        else
        {
            while (row < e)
            {
                BLOCK[row][column] = '#';
                row++;
            }

            while (row > e)
            {
                BLOCK[row][column] = '#';
                row--;
            }
            BLOCK[row][column] = '#';
        }
    }
}

// generates patches of different types
void patch_gen(int size, int startR, int startC, char c)
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
            if (BLOCK[row][column] == ' ')
                BLOCK[row][column] = c;
        }
    }
}

// builds the different buildings
void building_gen(char c)
{
    int row = (rand() % ROWS - 4);
    if (row < 1)
        row = 2;
    int column = (rand() % COLUMNS - 4);
    if (column < 1)
        column = 2;

    int i, j;

    while (BLOCK[row - 1][column] != '#' && BLOCK[row][column - 1] != '#' && BLOCK[row][column - 1] != 'M' && BLOCK[row][column - 1] != 'C' && row > 0 && row < ROWS && column > 0 && column < COLUMNS && BLOCK[row + 1][column] != '#' && BLOCK[row][column + 1] != '#' && BLOCK[row][column + 1] != 'M' && BLOCK[row][column + 1] != 'C')
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
                if (BLOCK[i][j] != '%')
                    BLOCK[i][j] = '#';
            }
            else
            {
                if (BLOCK[i][j] != '%')
                    BLOCK[i][j] = c;
            }
        }
    }
}

// generates the map
void gen_terrain(int n, int e, int s, int w)
{
    edge_gen();

    int size = (rand() % 10) + 3;
    int row = (rand() % 5) + 2;
    int column = (rand() % 10) + 2;
    int number = (rand() % 20) + 2;

    int what = rand() % 3;

    int i;
    for (i = 0; i < 2; i++)
    {
        patch_gen(size, row, column, ',');

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
                BLOCK[row][column] = '%';
        }
        else if (what == 1)
        {
            if (row > 0 && column > 0)
                BLOCK[row][column] = '"';
        }
        else
            patch_gen(size, row, column, ',');

        size = (rand() % 10) + 2;
        row = (rand() % ROWS - 2) + 1;
        column = (rand() % COLUMNS - 2) + 1;
        what = (rand() % 3);
    }
    path_gen(n, s, e, w);
    building_gen('M');
    building_gen('C');

    // creates the border of impassible rocks
    for (row = 0; row < ROWS; row++)
    {
        for (column = 0; column < COLUMNS; column++)
        {
            if (BLOCK[row][column] == ' ')
                BLOCK[row][column] = '.';
        }
    }
    north = n;
    south = s;
    east = e;
    west = w;
}

int main()
{
    int seed = time(NULL);
    srand(seed);

    printf("Seed: %d\n", seed);

    int row, column;

    int n, s, e, w;

    n = rand() % COLUMNS;
    s = rand() % COLUMNS;
    e = rand() % ROWS;
    w = rand() % ROWS;

    gen_terrain(n, e, s, w);

    // prints the map
    char pos;
    for (row = 0; row < ROWS; row++)
    {
        for (column = 0; column < COLUMNS; column++)
        {

            pos = BLOCK[row][column];

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

    return 0;
}