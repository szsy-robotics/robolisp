#include "val.hpp"

#include <string>
#include <utility>

#include "env.hpp"

using namespace robolisp::impl;
using namespace val;

ValPtr Val::eval(Env */*env*/)
{
    return nullptr;
}

ValPtr Val::acquire()
{
    return copy();
}

bool Val::to_bool() const
{
    return true;
}

ValPtr robolisp::impl::val::eval(Val &&val, Env *env)
{
    ValPtr res_val = val.eval(env);
    if (!res_val)
        res_val = val.acquire();
    
    return res_val;
}

ValPtr robolisp::impl::val::eval(ValPtr val, Env *env)
{
    ValPtr res_val = val->eval(env);
    if (!res_val)
        res_val = std::move(val);
    
    return res_val;
}
