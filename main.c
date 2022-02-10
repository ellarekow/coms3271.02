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
            fullMap[i][j]->visited = 0;
            fullMap[i][j]->y = 199 - i;
            fullMap[i][j]->x = j - 199;
        }
    }

    int row = 199;
    int column = 199;

    visit2(fullMap, row, column);
    print_map(fullMap[row][column]);

    printf("Please enter a direction:\n- 'n' for north\n- 's' for south\n- 'w' for west\n- 'e' for east\n- 'f XX YY' for fly (please ensure to include the 0X for single numbers) \n");

    int run = 1;
    while (run)
    {
        printf("Please enter a direction:\n");
        char *input = NULL;
        size_t len = 0;
        getline(&input, &len, stdin);

        switch (input[0])
        {
        case 'f':
            int tempRow;
            tempRow = (int)(input[2]) * 100;
            tempRow += (int)(input[3]) * 10;
            tempRow += (int)(input[4]);

            printf("%d", tempRow);

            int tempColumn;
            tempColumn = (int)(input[6]) * 100;
            tempColumn += (int)(input[7]) * 10;
            tempColumn += (int)(input[8]);

            if (tempColumn >= 0 && tempColumn <= 399 && tempRow >= 0 && tempRow <= 399)
            {
                column = tempColumn;
                row = tempRow;
                break;
            }
            else
            {
                printf("Please enter a valid input.\n");
                continue;
            }

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

        visit2(fullMap, row, column);
        print_map(fullMap[row][column]);
        free(input);
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