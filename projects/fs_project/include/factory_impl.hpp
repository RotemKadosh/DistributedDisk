#ifndef __RD94_FACTORY_IMPL_HPP__
#define __RD94_FACTORY_IMPL_HPP__

#include "factory.hpp"

namespace ilrd
{

template<typename BASE, typename ARG, typename ID >
void Factory<BASE, ARG, ID>::Add(ID class_id_, boost::function< boost::shared_ptr<BASE> (ARG) > create_func_)
{
    m_map[class_id_] = create_func_;
}

template<typename BASE, typename ARG, typename ID >
boost::shared_ptr<BASE> Factory<BASE, ARG, ID>::Create(ID class_id_, ARG arg_)
{
    try
    {
        return m_map.at(class_id_)(arg_);
    }
    catch(std::out_of_range& err)
    {
        throw new WrongKey();
    }
}


}// ilrd

 
#endif // __RD94_FACTORY_IMPL_HPP__