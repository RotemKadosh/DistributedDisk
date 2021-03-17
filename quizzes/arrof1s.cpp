#include <iostream>
/*
max_length
curr_length;
for each element:
    if(element == 0 && k > 0)
    {
        cur_length += 1;
        --k;   
    }
    else if(element == 1)
    {
        cur_length += 1;
    }
    else if(element == 0 && k == 0)
    {
        k = old_k;
        cur_length = 0;
    }
    if(cur_length >= max_length)
    {
        max_length = cur_length;
    }
*/
int ContiniuosOns(int *arr, size_t size, int k)
{
    int init_k = k;
    size_t max_length = 0;
    size_t last_start = 0;
    size_t i = 0;
    size_t curr_length = 0;
    while(i < size)
    {
        int element = arr[i];
        if(0 == element && k > 0)
        {
            if(k == init_k)
            {
                last_start = i;
            }
            ++curr_length;
            --k;
            ++i;
        }
        else if(element == 1)
        {
            ++curr_length;
            ++i;
        }
        else if(element == 0 && k == 0)
        {
            k = init_k;
            curr_length = 0;
            ++last_start;
            i = last_start;
        }
        if(curr_length >= max_length)
        {
            max_length = curr_length; 
        }
    }
    new int[5][5]();
    return max_length;
}





int main()
{
    int arr[] = {1,1,1,0,0,0,1,1,1,1,0};// k = 2 => 10
    std::cout << ContiniuosOns(arr, 11, 2) << std::endl;

    int arr2[] = {0,0,1,1,0,0,1,1,1,0,1,1,0,0,0,1,1,1,1}; // k = 3 => 10
    std::cout << ContiniuosOns(arr2, (sizeof(arr2)/sizeof(int)), 3) << std::endl;

    int arr3[] = {1,0,1,1,0,0,1}; //k=3 => 7
    std::cout << ContiniuosOns(arr3, (sizeof(arr3)/sizeof(int)), 3) << std::endl;
    int arr4[] = {1,0,1,1,0,0,1}; //k=2 => 5
    std::cout << ContiniuosOns(arr4, (sizeof(arr4)/sizeof(int)), 2) << std::endl;


    int arr5[] = {1,0,1,1,0,0,1}; //k=1 => 4
    std::cout << ContiniuosOns(arr5, (sizeof(arr5)/sizeof(int)), 1) << std::endl;

}
