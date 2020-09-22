#include <stdio.h>
#include <stdlib.h>
#define MAX 100
#define SWAPm(x, y) \
    {               \
        x = x ^ y;  \
        y = x ^ y;  \
        x = x ^ y;  \
    }
int arr1[10] = {0, 9, 2, 12, 0, 1, 3, 5, 15, 7};

typedef struct
{
    int key;
} element;

element arr2[10] = {
    {0},
    {9},
    {2},
    {12},
    {0},
    {1},
    {3},
    {5},
    {15},
    {7}};

void SWAP(element *a, element *b, element *temp)
{
    *temp = *a;
    *a = *b;
    *b = *temp;
}

//***insertion sort***
void insert(int e, int a[], int i);
void insertionSort(int a[], int n)
{
    //sort [1:n] in non decreasing order
    int j;
    for (j = 2; j <= n; j++) //this starts from 2 because 1st element starts from a[1] and we assume a[1] is already part of a sorted list
    {
        int temp = a[j];
        insert(temp, a, j - 1);
    }
}

void insert(int e, int a[], int i)
{
    a[0] = e;
    while (e < a[i])
    {
        a[i + 1] = a[i];
        i--;
    }
    a[i + 1] = e;
}

//***quick sort***
void quickSort(element a[], int left, int right)
{
    /* sort a[left:right] into nondecreasing order on the key field;
    a[left].key is arbitrarily chosen as the pivot key;
    it is assumed that a[left].key <= a[right+1].key */

    int pivot, i, j;
    element temp;
    if (left < right)
    {
        i = left;
        j = right + 1;
        pivot = a[left].key;
        do
        { //search for keys from the left and right sublists
            //swapping out of order elements
            //until left and right boundaries cross or meet
            do
            {
                i++;
            } while (a[i].key < pivot);
            do
            {
                j--;
            } while (a[j].key > pivot);
            if (i < j)
                SWAP(&a[i], &a[j], &temp);

        } while (i < j);
        SWAP(&a[left], &a[j], &temp);
        quickSort(a, left, j - 1);
        quickSort(a, j + 1, right);
    }
}

int main(void)
{
    int i;
    quickSort(arr2, 0, 9);
    for (i = 0; i < 10; i++)
    {
        printf("%3d", arr2[i].key);
    }
}