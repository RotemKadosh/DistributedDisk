#include <math.h>


void findtwoNum(int *arr, int n, int *x, int * y)
{
    int sum_p = 0;
    int sum_e = 0;

    int mult_p = 0;
    int mult_e = 0;

    FindMult(*arr, n, &mult_e, mult_p);
    FindSum(*arr, n,&sum_e, &sum_p);

    double mult = mult_p/mult_e;
    double sum = sum_p  - sum_e;

    double y1 = (sum + sqrt((sum *sum) - (4 * mult)))/2;
    double y2 = (sum - sqrt((sum *sum) - (4 * mult)))/2;

    *y = (y1 > n || y1 < 0) ? y2 : y1;

    *x = sum - *y;
}
void FindMult(int *arr, int n, int *x, int * y)
{
    for(int i =1; i < n - 2 ; ++i)
    {
        *x *= arr[i];
        *y *= i;
    }
    *y *= (n-1);
    *y *= n;
}
void FindSum(int *arr, int n, int *x, int * y)
{
    for(int i =1; i < n - 2 ; ++i)
    {
        *x += arr[i];
        *y += i;
    }
    *y += (n-1);
    *y += n;
}

int main()
{
    int x = 0;
    int y = 0;
    int arr[11] = {0,1,2,3,4,6,8,9,10,11,12}; // 5,7
    findtwoNum(arr,12,&x,&y);
    printf("x = %d\n", x);
    printf("y = %d\n", y);
}