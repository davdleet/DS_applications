#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFFSIZE 100
#define MAX_SIZE 100
#define MAX_LINES 1000
#define MALLOC(p, s) (p) = malloc(s)
#define readline fgets(buff, sizeof(buff), fp1)

int maze[MAX_SIZE][MAX_SIZE];
int mark[MAX_SIZE][MAX_SIZE];

//set possible moves
typedef struct
{
    short int vert;
    short int horiz;
} coord;
coord start, goal;
coord move[8] = {
    {-1, 0},
    {-1, 1},
    {0, 1},
    {1, 1},
    {1, 0},
    {1, -1},
    {0, -1},
    {-1, -1}};

//linked stack representation
typedef struct
{
    int row;
    int col;
    int dir;
} element;

typedef struct stack *stackPointer;
struct stack
{
    element data;
    stackPointer link;
};

stackPointer top = NULL;
element stackEmpty();

void push(element item)
{
    stackPointer temp;
    MALLOC(temp, sizeof(*temp));

    temp->data = item;
    temp->link = top;
    top = temp;
}

element pop()
{
    stackPointer temp;
    element item;
    if (!temp)
        return stackEmpty();
    temp = top;
    top = top->link;
    item = temp->data;
    free(temp);
    return item;
}

element stackEmpty()
{
    fprintf(stderr, "Stack is empty, cannot pop element");
    return top->data;
}

//initialize maze
void maze_init()
{
    int i, j;
    for (i = 0; i < MAX_SIZE; i++)
    {
        for (j = 0; j < MAX_SIZE; j++)
        {
            maze[i][j] = 1;
        }
    }
}

//fill maze after receiving data from text
void fill_maze(char *buff, int vert)
{
    int i = 1;
    char *str1 = strtok(buff, " ");
    while (str1 != NULL)
    {
        maze[vert][i] = atoi(str1);
        i++;
        str1 = strtok(NULL, " ");
    }
}
//get the number of nodes in a stack
int getCount(stackPointer top)
{
    int count = 0;
    stackPointer current = top;
    while (current != NULL)
    {
        count++;
        current = current->link;
    }
    return count;
}

//get start coordinates from text file
void getStart(char *buff)
{
    int i = 0;
    char *str1 = strtok(buff, " ");
    char *sArr[10] = {
        NULL,
    };
    while (str1 != NULL)
    {
        sArr[i] = str1;
        i++;
        str1 = strtok(NULL, " ");
    }
    start.vert = atoi(sArr[0]);
    start.horiz = atoi(sArr[1]);
}

//get goal coordinates from text file
void getGoal(char *buff)
{
    int i = 0;
    char *str1 = strtok(buff, " ");
    char *sArr[10] = {
        NULL,
    };
    while (str1 != NULL)
    {
        sArr[i] = str1;
        i++;
        str1 = strtok(NULL, " ");
    }
    goal.vert = atoi(sArr[0]);
    goal.horiz = atoi(sArr[1]);
}

//solve the maze
void path(char *argv2)
{
    FILE *fp2 = fopen(argv2, "wt");
    int i, row, col, nextRow, nextCol, dir, found = 0;
    element position;
    mark[start.vert][start.horiz] = 1;
    stackPointer first;
    MALLOC(first, sizeof(*first));
    first->link = NULL;
    first->data.row = 1;
    first->data.col = 1;
    first->data.dir = 1;
    top = first;
    while (top != NULL && !found)
    {
        position = pop();
        row = position.row;
        col = position.col;
        dir = position.dir;
        while (dir < 8 && !found)
        {
            nextRow = row + move[dir].vert;
            nextCol = col + move[dir].horiz;
            if (nextRow == goal.vert && nextCol == goal.horiz)
                found = 1;
            else if (!maze[nextRow][nextCol] && !mark[nextRow][nextCol])
            {
                mark[nextRow][nextCol] = 1;
                position.row = row;
                position.col = col;
                position.dir = ++dir;
                push(position);
                row = nextRow;
                col = nextCol;
                dir = 0;
            }
            else
                ++dir;
        }
    }
    if (found)
    {
        int i;
        int count = getCount(top);
        char arr1[MAX_LINES][10];
        stackPointer temp = top;

        for (i = count; i > 0; i--)
        {
            sprintf(arr1[i], "%d %d", temp->data.row, temp->data.col);
            temp = temp->link;
        }
        for (i = 1; i < count; i++)
        {
            fprintf(fp2, "%s\n", arr1[i]);
        }
        fprintf(fp2, "%d %d\n", row, col);
        fprintf(fp2, "%d %d\n", goal.vert, goal.horiz);
    }
    else
        fprintf(fp2, "%d %d\n", start.vert, start.horiz);
    fclose(fp2);
}

//run the program
int main(int argc, char *argv1[])
{
    int i = 1, j;
    char buff[BUFFSIZE];
    FILE *fp1 = fopen(argv1[0], "rt");
    readline;
    getStart(buff);
    readline;
    getGoal(buff);
    readline;
    maze_init();
    while (readline != NULL)
    {
        fill_maze(buff, i);
        i++;
    }
    path(argv1[1]);
    fclose(fp1);
    return 0;
}