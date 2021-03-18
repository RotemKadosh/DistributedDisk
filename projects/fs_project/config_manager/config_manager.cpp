#include <iostream> // ifstream
#include <boost/tokenizer.hpp>// tokenizer
#include "config_manager.hpp"
#include "logger.hpp"
#include "singleton_impl.hpp"

namespace ilrd
{

ConfigManager::ConfigManager():
m_config_map(ReadConfigFile("./configurations"))
{

}
std::string ConfigManager::GetConfig(std::string config_key)
{
    if(m_config_map->find(config_key) == m_config_map->end())
    {
        throw ConfigNotFound();
    }
    return (*m_config_map)[config_key];
}

boost::shared_ptr<ConfigManager::ConfigMap> ConfigManager::ReadConfigFile(std::string path)
{
    // read values into string
    typedef std::istreambuf_iterator<char> str_buf_iter;
    std::ifstream config_fs(path.c_str());
    std::string str((str_buf_iter(config_fs)),str_buf_iter());

    // tokenize string
    typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
    boost::char_separator<char> separators(":\n");
    tokenizer tokens(str, separators);
    
    //insert to map
    boost::shared_ptr<ConfigMap> values = boost::make_shared<ConfigMap>(ConfigMap());
    try
    {
        for (tokenizer::iterator tok_iter = tokens.begin();tok_iter != tokens.end(); ++tok_iter)
        {
            std::string key = *tok_iter;
            (*values)[key] = *(++tok_iter);

        }
    }
    catch(const std::exception& e)
    {   
        LOG_ERROR("Configuration Fail");
        throw ConfigFailException();
    }

    return values;
}

}

