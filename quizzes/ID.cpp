#include <iostream>// std::cout
#include <string> // string
using namespace std;
 


size_t CountNextDec(string str, size_t idx, size_t len)
{
    size_t j = idx + 1;
    size_t NumOfNextDec = 0;
    while (j < len && str[j] == 'D') 
    {
        ++NumOfNextDec;
        ++j;
    }
    return NumOfNextDec;
}



void InsertI(int *ans, int dec_count, int *curr_max, int *last_dig)
{
    *ans *= 10;
    *curr_max = *curr_max + dec_count + 1;
    *ans += *curr_max;
    *last_dig = *curr_max;
}

void InsertDecSequence(int *ans, int dec_count, size_t *cur_idx ,int *last_dig)
{
    while(dec_count-- > 0)
    {
        *ans *= 10;
        *ans += --(*last_dig);
        *cur_idx += 1;
    }
}

void InsertDFirst(int *ans, int dec_count,  int *curr_max,int *last_dig)
{
    *curr_max = dec_count + 2;
    *ans = *curr_max;
    *ans *= 10;
    *ans += *curr_max - 1;
    *last_dig = *curr_max - 1;
}

int MinNumberForPattern(string str)
{
    size_t len = str.length();
    int ans = 0;
    int curr_max_dig = 0;
    int last_dig = 0;
    for(size_t i = 0; i < len; ++i)
    {
        if('I' == str[i])
        {
            int next_dec = CountNextDec(str, i, len);
            if(0 == i)
            {
                ans = 1;
                curr_max_dig = 1; 
            }
            InsertI(&ans, next_dec, &curr_max_dig, &last_dig);
            InsertDecSequence(&ans, next_dec, &i ,&last_dig);  
        }
        else
        {
            if(0 == i)
            {
                int next_dec = CountNextDec(str, i, len);
                InsertDFirst(&ans, next_dec, &curr_max_dig, &last_dig);
            }
            else
            {
                InsertDecSequence(&ans, 1, &i, &last_dig);
            } 
        }
    }
    return ans;
}

int main()
{
    string s1("D");
    std::cout << MinNumberForPattern(s1)<< "\n";
    string s2("I");
    std::cout << MinNumberForPattern(s2)<< "\n";
    string s3("DD");
    std::cout << MinNumberForPattern(s3)<< "\n";
    string s4("II");
    std::cout << MinNumberForPattern(s4)<< "\n";
    string s5("DIDI");
    std::cout << MinNumberForPattern(s5)<< "\n";
    string s6("IIDDD");
    std::cout << MinNumberForPattern(s6)<< "\n";
    string s7("DDIDDIID");
    std::cout << MinNumberForPattern(s7)<< "\n";
    string s8("DIDIDDDI");
    std::cout << MinNumberForPattern(s8)<< "\n"; 
    return 0;
}