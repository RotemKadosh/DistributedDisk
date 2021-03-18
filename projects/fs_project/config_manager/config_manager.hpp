#ifndef __RD94_CONFIG_MANAGER_HPP__
#define __RD94_CONFIG_MANAGER_HPP__


#include "singleton.hpp" //singleton<Master>
#include "singleton_impl.hpp" //Singleton
#include <boost/noncopyable.hpp>//boost::noncopyable
#include <boost/unordered_map.hpp> //boost::unordered_map


namespace ilrd
{



#define CONFIG (Singleton<ConfigManager>::GetInstance())
#define GET_CONFIG(config_key) ((CONFIG)->GetConfig(config_key))

class ConfigManager
{
public:

    std::string GetConfig(std::string config_key); //may throw config not found

    struct ConfigFailException : public std::runtime_error
    {
        ConfigFailException(): std::runtime_error(""){}
        const char * what () const throw ()
        {
            return "Configuration Fail";
        }
    };
    struct ConfigNotFound : public ConfigFailException
    {
        ConfigNotFound()
        {} 
        const char * what () const throw ()
        {
            return "Configuration not found";
        } 
    };

private:
    friend class Singleton<ConfigManager>;
    explicit ConfigManager();
    typedef boost::unordered_map<std::string, std::string> ConfigMap;
    boost::shared_ptr<ConfigMap> ReadConfigFile(std::string path);

    
    boost::shared_ptr<ConfigMap> m_config_map;
};

}//namespace

#endif