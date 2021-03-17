#include <fstream>
#include <boost/tokenizer.hpp>
#include <string>
#include <boost/thread/mutex.hpp> // boost::mutex
#include <boost/thread.hpp>
#include <map>
#include <iostream>

class WaitableMap
{
public:
    void Add(std::string);
    std::pair<std::string,size_t>& GetMostFrequentWord();
    std::map<std::string, size_t>::iterator Begin();
    std::map<std::string, size_t>::iterator End();
private:
    boost::mutex m_mutex;
    std::map<std::string, size_t> word_map;
    std::pair<std::string,size_t> most_freq_pair;
};

void WaitableMap::Add(std::string str)
{
    boost::unique_lock<boost::mutex> guard((this->m_mutex));
    if(word_map.find(str) == word_map.end())
    {
        word_map[str] = 1;
    }
    else
    {
        word_map[str] +=1;
    }
    if(word_map[str] > most_freq_pair.second)
    {
        most_freq_pair.first = str;
        most_freq_pair.second = word_map[str]; 
    }
}
std::map<std::string, size_t>::iterator WaitableMap::Begin()
{
    boost::unique_lock<boost::mutex> guard((this->m_mutex));
    return word_map.begin();
}
std::map<std::string, size_t>::iterator WaitableMap::End()
{
    boost::unique_lock<boost::mutex> guard((this->m_mutex));
    return word_map.end();
}
std::pair<std::string,size_t>& WaitableMap::GetMostFrequentWord()
{
    boost::unique_lock<boost::mutex> guard((this->m_mutex));
    return most_freq_pair;
}


void ReadFileToMap(const char *file_path, const char seperators, WaitableMap *map)
{
    // read values into string
    typedef std::istreambuf_iterator<char> str_buf_iter;
    std::ifstream input_words(file_path);
    std::string str((str_buf_iter(input_words)),str_buf_iter());

    // tokenize string
    typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
    boost::char_separator<char> separators;
    tokenizer tokens(str, separators);

    for (tokenizer::iterator tok_iter = tokens.begin(); 
        tok_iter != tokens.end(); ++tok_iter)
    {
        map->Add(*tok_iter);
    }

}
void WriteMapToFile(const char *file_path, WaitableMap *map)
{
    std::ofstream file(file_path);
    for(std::map<std::string, size_t>::iterator it = map->Begin(); it != map->End(); ++it)
    {
        file << it->first <<std::endl;
    }
}


int main(int argc, char *argv[])
{
    const char *input1 = argv[1];
    const char *input2 = argv[2];
    const char *input3 = argv[3];
    WaitableMap word_map;
    std::cout << "Enter list of seperators:" << std::endl;
    char seperators ='\n';
    

    std::cin >> seperators;
    boost::thread reader1(boost::bind(&ReadFileToMap, input1, seperators, &word_map));
    boost::thread reader2(boost::bind(&ReadFileToMap, input2, seperators, &word_map));
    boost::thread reader3(boost::bind(&ReadFileToMap, input3, seperators, &word_map));

    reader1.join();
    reader2.join();
    reader3.join();
    boost::thread writer(boost::bind(&WriteMapToFile, "out.txt" ,&word_map));
    writer.join();
    std::cout << "most frequent used wored: "<<word_map.GetMostFrequentWord().first <<" : "<< word_map.GetMostFrequentWord().second<<std::endl;
    return 0;   
}