#include "terrain.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <endian.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <limits.h>
#include <sys/time.h>
#include <assert.h>

int main(int argc, char *argv[])
{
    struct timeval tv;
    uint32_t seed;
    if (argc == 2)
    {
        seed = atoi(argv[1]);
    }
    else
    {
        gettimeofday(&tv, NULL);
        seed = (tv.tv_usec ^ (tv.tv_sec << 20)) & 0xffffffff;
    }

    printf("Seed: %u\n", seed);
    srand(seed);

    block *fullMap[399][399];

    int i, j;
    for (i = 0; i < 399; i++)
    {
        for (j = 0; j < 399; j++)
        {
            fullMap[i][j] = NULL;
            fullMap[i][j] = malloc(sizeof(block));
        }
    }

    int row = 155;
    int column = 155;

    visit(fullMap[row][column], fullMap[row - 1][column], fullMap[row + 1][column], fullMap[row][column + 1], fullMap[row][column - 1]);
    print_map(fullMap[row][column]);

    printf("Please enter a direction:\n- 'n' for north\n- 's' for south\n- 'w' for west\n- 'e' for east\n- 'f XX YY' for fly (please ensure to include the 0X for single numbers) \n");

    int run = 1;
    while (run)
    {
        printf("Please enter a direction:\n");
        char input[20];
        scanf("%s", input);

        switch (input[0])
        {
        case 'f':
            int tempRow = (input[2] - '0') * 10;
            tempRow += input[3] - '0';
            int tempColumn = (input[5] - '0') * 10;
            tempColumn += input[6] - '0';

            if (tempColumn >= 0 && tempColumn <= 399 && tempRow >= 0 && tempRow <= 399)
            {
            }
            break;

        case 'n':
            if (row < 399)
                row -= 1;
            break;
        case 's':
            if (row > 0)
                row += 1;
            break;
        case 'w':
            if (column > 0)
                column -= 1;
            break;
        case 'e':
            if (column < 399)
                column += 1;
            break;
        case 'q':
            run = 0;
            break;
        default:
            printf("please enter a valid input.");
            continue;
        }

        visit(fullMap[row][column], fullMap[row - 1][column], fullMap[row + 1][column], fullMap[row][column + 1], fullMap[row][column - 1]);
        print_map(fullMap[row][column]);
    }

    for (i = 0; i < 399; i++)
    {
        for (j = 0; j < 399; j++)
        {
            free(fullMap[i][j]);
        }
    }

    return 0;
}