#include <iostream>

void swap(int *num1, int* num2)
{
    int temp = *num1;
    *num1 = *num2;
    *num2 = temp;
}



void Merge(int *arr, size_t size_l, size_t size_r)
{
    int *tempArr = new int [size_l + size_r];
    int *runner = tempArr;
    int *left = arr;
    int *right = arr + size_l;
    
    while(left < arr + size_l && right < arr + size_l + size_r)
    {
        if(*left < *right)
        {
            *runner = *left;
            ++left;
        }
        else
        {
           *runner = *right;
           ++right; 
        }
        ++runner;
    }
    while(left < arr + size_l)
    {
        *runner = *left;
        ++left;
        ++runner;
    }
    while(right < arr + size_r + size_l)
    {
       *runner = *right;
        ++right; 
        ++runner;
    }
    for(size_t i = 0; i < size_l + size_r; ++i)
    {
        arr[i] = tempArr[i];
    }
}

void mergeSort(int *arr, size_t size)
{
    if(1 == size)
    {
        return;
    }
    size_t pivot = size / 2;
    mergeSort(arr, size/2);
    mergeSort(arr + pivot, (size / 2) + (size % 2 == 1));
    Merge(arr, size / 2, (size / 2) + (size % 2 == 1));
}


void print(int *arr, size_t size)
{
    for(int i = 0; i < size ; ++i)
    {
        std::cout<< arr[i] << " ->";
    }
    std::cout<< "\n";
}

int main()
{
    int arr[] = {1,1,1,2,5,4,1,8,4};
    mergeSort(arr, 9);
    print(arr, 9);
    return 0;
}