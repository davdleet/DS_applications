#include <stdio.h>
#include <stdlib.h>
#define FULL(ptr) (!(ptr))
#define MAX_SIZE 50
#define MALLOC(p, s) p = malloc(s)

//declaration of struct, enum types and pointers
typedef enum
{
    head,
    entry
} tagfield;
typedef struct matrixNode *matrixPointer;
typedef struct entryNode
{
    int row;
    int col;
    int value;
} entryNode;
typedef struct matrixNode
{
    matrixPointer down;
    matrixPointer right;
    tagfield tag;
    union {
        matrixPointer next;
        entryNode entry;
    } u;
} matrixNode;

//hdnode array will be used as a header of each row/col of a matrix
matrixPointer hdnode[MAX_SIZE];

//functions

matrixPointer newNode(void)
{
    matrixPointer temp;
    MALLOC(temp, sizeof(*temp));
    return temp;
}

matrixPointer mread(void)
{
    int numRows, numCols, numTerms, numHeads, i;
    int row, col, value, currentRow;
    matrixPointer temp, last, node;

    printf("Enter the number of rows, columns and number of nonzero terms: ");
    scanf("%d%d%d", &numRows, &numCols, &numTerms);
    if (numRows < 1 || numCols < 1 || numRows > MAX_SIZE || numCols > MAX_SIZE)
    {
        fprintf(stderr, "\nWrong row or Column\n");
        printf("Number of rows must be positive, and less than 51\n");
        return NULL;
    }
    numHeads = (numCols >= numRows) ? numCols : numRows;
    node = newNode();
    //the header node of header nodes H
    node->tag = entry;
    node->u.entry.row = numRows;
    node->u.entry.col = numCols;
    node->u.entry.value = numTerms;
    if (!numHeads)
        //if there are no header nodes of the matrix, it points at itself
        node->right = node;
    else
    {
        for (i = 0; i < numHeads; i++)
        //initializing header nodes
        {
            temp = newNode();
            hdnode[i] = temp;
            hdnode[i]->tag = head;
            hdnode[i]->right = temp;
            hdnode[i]->u.next = temp;
        }
        currentRow = 0;
        last = hdnode[0]; //last node in current row
        for (i = 0; i < numTerms; i++)
        {
            printf("Enter row, column and value: ");
            scanf("%d%d%d", &row, &col, &value);
            if (row > numRows || col > numCols || row < 0 || col < 0)
            {
                fprintf(stderr, "Wrong row or column.\n");
                numTerms++;
                continue;
            }
            if (row > currentRow) //close current row
            {
                last->right = hdnode[currentRow];
                currentRow = row;
                last = hdnode[row];
            }
            MALLOC(temp, sizeof(*temp));
            temp->tag = entry;
            temp->u.entry.row = row;
            temp->u.entry.col = col;
            temp->u.entry.value = value;
            last->right = temp;
            last = temp;
            hdnode[col]->u.next->down = temp;
            hdnode[col]->u.next = temp;
        }
        last->right = hdnode[currentRow];
        for (i = 0; i < numCols; i++)
            hdnode[i]->u.next->down = hdnode[i];
        for (i = 0; i < numHeads - 1; i++)
            hdnode[i]->u.next = hdnode[i + 1];
        hdnode[numHeads - 1]->u.next = node;
        node->right = hdnode[0];
    }
    return node;
}

void mwrite(matrixPointer node)
{
    //print out the matrix in row major form
    int i;
    matrixPointer temp, head = node->right;
    //matrix dimensions
    printf("\n numRows = %d, numCols = %d\n", node->u.entry.row, node->u.entry.col);
    printf("The matrix by row, column, and value: \n\n");
    for (i = 0; i < node->u.entry.row; i++)
    {
        //print out entries in each row
        for (temp = head->right; temp != head; temp = temp->right)
            printf("%5d%5d%5d \n",
                   temp->u.entry.row, temp->u.entry.col, temp->u.entry.value);
        head = head->u.next; //next row
    }
}

void merase(matrixPointer *node)
{ //erase the matrix, return the nodes to the heap
    matrixPointer x, y, head = (*node)->right;
    int i;
    //free the entry and header nodes by row
    for (i = 0; i < (*node)->u.entry.row; i++)
    {
        y = head->right;
        while (y != head)
        {
            x = y;
            y = y->right;
            free(x);
        }
        x = head;
        head = head->u.next;
        free(x);
    }
    //free remaining header nodes
    y = head;
    while (y != *node)
    {
        x = y;
        y = y->u.next;
        free(x);
    }
    free(*node);
    *node = NULL;
}

//add function, takes two arguments and returns a pointer to the matrix
matrixPointer madd(matrixPointer matrix1, matrixPointer matrix2)
{
    matrixPointer matrix3, m1, m2, tempnode, last;
    int numRows, numCols, numTerms, numHeads, i, j;
    int row1, col1, val1, row2, col2, val2;
    m1 = matrix1->right;
    m2 = matrix2->right;
    //save the number of rows and columns of the new function
    numRows = matrix1->u.entry.row;
    numCols = matrix1->u.entry.col;
    numTerms = matrix1->u.entry.value + matrix2->u.entry.value;
    numHeads = (numCols >= numRows) ? numCols : numRows;
    //make a new node and initialize
    matrix3 = newNode();
    matrix3->tag = entry;
    matrix3->u.entry.row = numRows;
    matrix3->u.entry.col = numCols;
    matrix3->u.entry.value = numTerms;
    matrix3->down = NULL;
    if (!numHeads) //if numHeads is empty
        matrix3->right = matrix3;
    else
    {
        //initialize header nodes
        for (i = 0; i < numHeads; i++)
        {
            tempnode = newNode();
            hdnode[i] = tempnode;
            hdnode[i]->tag = head;
            hdnode[i]->right = tempnode;
            hdnode[i]->u.next = tempnode;
        }
        for (i = 0; i < numHeads; i++)
        {
            //fill the new matrix row by row
            last = hdnode[i];
            for (j = 0; j < i; j++)
            {
                m1 = m1->u.next;
            }
            for (j = 0; j < i; j++)
            {
                m2 = m2->u.next;
            }
            m1 = m1->right;
            m2 = m2->right;
            //repeat until both m1 and m2 are pointing at the header node
            //while traversing m1 and m2
            while (m1->tag == entry || m2->tag == entry)
            {
                //if there is an entry in m1 and m2
                if (m1->tag == entry && m2->tag == entry)
                {
                    row1 = m1->u.entry.row;
                    col1 = m1->u.entry.col;
                    val1 = m1->u.entry.value;
                    row2 = m2->u.entry.row;
                    col2 = m2->u.entry.col;
                    val2 = m2->u.entry.value;
                    //if row and column match
                    if (row1 == row2 && col1 == col2)
                    {
                        m1 = m1->right;
                        m2 = m2->right;
                        MALLOC(tempnode, sizeof(*tempnode));
                        tempnode->tag = entry;
                        tempnode->u.entry.row = row1;
                        tempnode->u.entry.col = col1;
                        tempnode->u.entry.value = val1 + val2;
                        last->right = tempnode;
                        last = tempnode;
                        hdnode[col1]->u.next->down = tempnode;
                        hdnode[col1]->u.next = tempnode;
                    }
                    //if row and col do not match, assign m1 value to matrix3
                    else
                    {
                        m1 = m1->right;
                        MALLOC(tempnode, sizeof(*tempnode));
                        tempnode->tag = entry;
                        tempnode->u.entry.row = row1;
                        tempnode->u.entry.col = col1;
                        tempnode->u.entry.value = val1;
                        last->right = tempnode;
                        last = tempnode;
                        hdnode[col1]->u.next->down = tempnode;
                        hdnode[col1]->u.next = tempnode;
                    }
                }
                //if only m1 has an entry in the row
                else if (m1->tag == entry && m2->tag == head)
                {
                    row1 = m1->u.entry.row;
                    col1 = m1->u.entry.col;
                    val1 = m1->u.entry.value;
                    m1 = m1->right;
                    MALLOC(tempnode, sizeof(*tempnode));
                    tempnode->tag = entry;
                    tempnode->u.entry.row = row1;
                    tempnode->u.entry.col = col1;
                    tempnode->u.entry.value = val1;
                    last->right = tempnode;
                    last = tempnode;
                    hdnode[col1]->u.next->down = tempnode;
                    hdnode[col1]->u.next = tempnode;
                }
                //only m2 has an entry in the row
                else if (m2->tag == entry && m1->tag == head)
                {
                    row2 = m2->u.entry.row;
                    col2 = m2->u.entry.col;
                    val2 = m2->u.entry.value;
                    m2 = m2->right;
                    MALLOC(tempnode, sizeof(*tempnode));
                    tempnode->tag = entry;
                    tempnode->u.entry.row = row2;
                    tempnode->u.entry.col = col2;
                    tempnode->u.entry.value = val2;
                    last->right = tempnode;
                    last = tempnode;
                    hdnode[col2]->u.next->down = tempnode;
                    hdnode[col2]->u.next = tempnode;
                }
                else
                {
                    m1 = m1->right;
                    m2 = m2->right;
                }
            }
            //close row, link the last node to header node
            last->right = hdnode[i];
            m1 = matrix1->right;
            m2 = matrix2->right;
        }
        //link the nodes
        for (i = 0; i < numCols; i++)
            hdnode[i]->u.next->down = hdnode[i];
        for (i = 0; i < numHeads - 1; i++)
            hdnode[i]->u.next = hdnode[i + 1];
        hdnode[numHeads - 1]->u.next = matrix3;
        matrix3->right = hdnode[0];
    }
    return matrix3;
}

//multiplication function, takes two arguments
//and returns a pointer to the matrix
matrixPointer mmult(matrixPointer matrix1, matrixPointer matrix2)
{
    matrixPointer matrix3, m1, m2, tempnode, last, fnode, fnode2;
    int i, j, k, row1, row2, col1, col2, tempval = 0, count = 0;
    int numRows, numCols, numHeads;
    m1 = matrix1->right;
    m2 = matrix2->right;
    //save the number of rows, columns, header nodes of the new matrix
    numRows = matrix1->u.entry.row;
    numCols = matrix2->u.entry.col;
    numHeads = (numCols >= numRows) ? numCols : numRows;
    //make a new node to point at the matrix, initialize
    matrix3 = newNode();
    matrix3->right = matrix3;
    matrix3->tag = entry;
    matrix3->u.entry.row = numRows;
    matrix3->u.entry.col = numCols;
    matrix3->u.entry.value = count;
    matrix3->down = NULL;
    if (!numHeads) //if numHeads is empty
        matrix3->right = matrix3;
    else
    {
        //initialize header nodes of the new matrix
        for (i = 0; i < numHeads; i++)
        {
            tempnode = newNode();
            hdnode[i] = tempnode;
            hdnode[i]->tag = head;
            hdnode[i]->right = tempnode;
            hdnode[i]->u.next = tempnode;
        }
        //fill the new matrix while traversing m1 and m2
        for (i = 0; i < numHeads; i++)
        {
            last = hdnode[i];
            for (j = 0; j < i; j++)
                m1 = m1->u.next;
            m1 = m1->right;
            fnode = m1;
            m2 = m2->down;
            row1 = m1->u.entry.row;
            col1 = m1->u.entry.col;
            row2 = m2->u.entry.row;
            col2 = m2->u.entry.col;
            //repeat multiplication process for numCols times
            for (j = 0; j < numCols; j++)
            {
                //while m1 is pointing at an entry node
                while (m1->tag == entry)
                {
                    //while m2 is pointing at an entry node
                    while (m2->tag == entry)
                    {
                        //if m1 and m2 has matching conditions for multiplication
                        //multiply the two and save to tempval
                        if (m1->u.entry.col == m2->u.entry.row)
                        {
                            tempval += m1->u.entry.value * m2->u.entry.value;
                        }
                        m2 = m2->down;
                    }
                    m1 = m1->right;
                    m2 = m2->down;
                }
                //if tempval is not 0, make a new node
                if (tempval != 0)
                {
                    MALLOC(tempnode, sizeof(*tempnode));
                    tempnode->tag = entry;
                    tempnode->u.entry.row = row1;
                    tempnode->u.entry.col = j;
                    tempnode->u.entry.value = tempval;
                    last->right = tempnode;
                    last = tempnode;
                    hdnode[j]->u.next->down = tempnode;
                    hdnode[j]->u.next = tempnode;
                    count++;
                }
                tempval = 0;
                m1 = fnode;
                m2 = matrix2->right;
                //m2 points to the next header node
                for (k = 0; k < j + 1; k++)
                {
                    m2 = m2->u.next;
                }
                m2 = m2->down;
            }
            //close row, connect last pointer to header node
            last->right = hdnode[i];
            m1 = matrix1->right;
            m2 = matrix2->right;
        }
        //link nodes
        for (i = 0; i < numCols; i++)
            hdnode[i]->u.next->down = hdnode[i];
        for (i = 0; i < numHeads - 1; i++)
            hdnode[i]->u.next = hdnode[i + 1];
        hdnode[numHeads - 1]->u.next = matrix3;
        matrix3->right = hdnode[0];
        matrix3->u.entry.value = count;
    }
    return matrix3;
}

//transpose function, takes one argument
//and returns a pointer to the matrix
matrixPointer mtranspose(matrixPointer matrix1)
{
    matrixPointer matrix2, m1, tempnode, last;
    int numRows, numCols, numTerms, numHeads, i, j, changerow;
    int row1, col1, val1, row2, col2, val2;
    m1 = matrix1->right;
    numRows = matrix1->u.entry.row;
    numCols = matrix1->u.entry.col;
    numTerms = matrix1->u.entry.value;
    numHeads = (numCols >= numRows) ? numCols : numRows;
    //make new node to point at the new matrix
    matrix2 = newNode();
    matrix2->tag = entry;
    matrix2->u.entry.row = numCols;
    matrix2->u.entry.col = numRows;
    matrix2->u.entry.value = numTerms;
    matrix2->down = NULL;
    //make the header nodes
    for (i = 0; i < numHeads; i++)
    {
        tempnode = newNode();
        hdnode[i] = tempnode;
        hdnode[i]->tag = head;
        hdnode[i]->right = tempnode;
        hdnode[i]->u.next = tempnode;
    }
    //traverse input matrix to transpose and save to new matrix
    for (i = 0; i < numHeads; i++)
    {
        for (j = 0; j < i; j++)
        {
            m1 = m1->u.next;
        }
        m1 = m1->right;
        //while m1 is pointing at an entry node
        //copy entry nodes, swapping row, col
        while (m1->tag == entry)
        {
            changerow = m1->u.entry.col;
            last = hdnode[changerow];
            //adjust last pointer to point at the last entry node of the row
            while (last->right->tag == entry)
                last = last->right;
            row1 = m1->u.entry.row;
            col1 = m1->u.entry.col;
            val1 = m1->u.entry.value;
            m1 = m1->right;
            MALLOC(tempnode, sizeof(*tempnode));
            tempnode->tag = entry;
            tempnode->u.entry.row = col1;
            tempnode->u.entry.col = row1;
            tempnode->u.entry.value = val1;
            last->right = tempnode;
            last = tempnode;
            hdnode[col1]->u.next->down = tempnode;
            hdnode[col1]->u.next = tempnode;
            last->right = hdnode[changerow];
        }
        m1 = matrix1->right;
    }
    //link nodes
    for (i = 0; i < numCols; i++)
        hdnode[i]->u.next->down = hdnode[i];
    for (i = 0; i < numHeads - 1; i++)
        hdnode[i]->u.next = hdnode[i + 1];
    hdnode[numHeads - 1]->u.next = matrix2;
    matrix2->right = hdnode[0];
    return matrix2;
}

int main(void)
{
    //save the matrices in marr array
    matrixPointer marr[64];
    //mark array indicates which array is filled or is empty
    int mark[64] = {0};
    int menunum = 0;
    int index1, index2, index3;
    matrixPointer header;
    matrixPointer *temp;
    //repeat menu prompt until program is closed
    while (menunum != 7)
    {
        printf("\n***SPARSE MATRIX PROGRAM MENU***\n\n");
        printf("1. Read\n2. Write\n3. Erase\n4. Add\n5. Multiply\n6. Transpose\n7. Exit\n\n");
        printf("Enter a Menu number: ");
        scanf("%d", &menunum);
        fflush(stdin);
        if (menunum < 1 || menunum > 8)
            printf("\nWrong number. Input a number between 1 to 7.\n");
        //read
        if (menunum == 1)
        {
            printf("Save index: ");
            scanf("%d", &index1);
            fflush(stdin);
            if (index1 < 1 || index1 > 64)
            {
                printf("\nWrong index. Input a number between 1 to 64.\n");
            }
            else
            {
                marr[index1] = mread();
                if (marr[index1] == NULL)
                {
                    printf("\nError occurred. Matrix not saved\n");
                    continue;
                }
                printf("\nMatrix successfully saved to index\n");
                mark[index1] = 1;
            }
        }
        //write
        if (menunum == 2)
        {
            printf("Print index: ");
            scanf("%d", &index1);
            fflush(stdin);
            if (!mark[index1])
            {
                printf("\nThe index is empty.\n");
            }
            else
            {
                printf("\nPrinting matrix...\n");
                mwrite(marr[index1]);
            }
        }
        //erase
        if (menunum == 3)
        {
            printf("Erase index: ");
            scanf("%d", &index1);
            fflush(stdin);
            if (!mark[index1])
            {
                printf("\nThe index is empty.\n");
            }
            else
            {
                printf("\nMatrix erased.\n");
                temp = &(marr[index1]);
                merase(temp);
                mark[index1] = 0;
            }
        }
        //add
        if (menunum == 4)
        {
            printf("First, second, save index: ");
            scanf("%d %d %d", &index1, &index2, &index3);
            fflush(stdin);
            if (mark[index1] == 0 || mark[index2] == 0)
            {
                fprintf(stderr, "\nOne of the input matrices are empty.\n");
                continue;
            }
            if (marr[index1]->u.entry.row != marr[index2]->u.entry.row ||
                marr[index1]->u.entry.col != marr[index2]->u.entry.col)
            {
                fprintf(stderr, "\nThe sizes of the matrices do not match.\n");
                printf("The row and column of First, second matrix must be same.\n");
                continue;
            }
            marr[index3] = madd(marr[index1], marr[index2]);
            mark[index3] = 1;
        }
        //multiply
        if (menunum == 5)
        {
            printf("First, second, save index: ");
            scanf("%d %d %d", &index1, &index2, &index3);
            fflush(stdin);
            if (mark[index1] == 0 || mark[index2] == 0)
            {
                fprintf(stderr, "\nOne of the input matrices are empty.\n");
                continue;
            }
            if (marr[index1]->u.entry.col != marr[index2]->u.entry.row)
            {
                fprintf(stderr, "\nThe sizes of the matrices do not match. \n");
                continue;
            }
            marr[index3] = mmult(marr[index1], marr[index2]);
            printf("Matrix successfully saved to index\n");
            mark[index3] = 1;
        }
        //transpose
        if (menunum == 6)
        {
            printf("Transpose index, save index: ");
            scanf("%d %d", &index1, &index2);
            fflush(stdin);
            if (mark[index1] == 0)
            {
                fprintf(stderr, "\nTranspose index is empty.\n");
                continue;
            }
            marr[index2] = mtranspose(marr[index1]);
            printf("Matrix successfully saved to index\n");
            mark[index2] = 1;
        }
    }
    printf("\nProgram exited.\n");
    return 0;
}