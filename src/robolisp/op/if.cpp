#include "if.hpp"

#include <cstddef>
#include <string>
#include <utility>

#include "../op.hpp"

#include "../env.hpp"
#include "../val.hpp"
#include "../val/sym.hpp"
#include "../op/eval.hpp"

using namespace robolisp::impl;
using namespace op;

If::If(Env *env) :
    env_ {env}
{}

ValPtr If::take_res()
{
    return std::move(res_val_);
}

std::string If::get_desc() const
{
    return "if";
}

std::size_t If::get_min_args() const
{
    return 2;
}

std::size_t If::get_max_args() const
{
    return 3;
}

void If::default_process(Val &&val)
{
    switch (get_arg_cnt())
    {
        case (1) :
        {
            condition_ = unquot_eval(std::move(val))->to_bool();
            break;
        }
        
        case (2) :
        {
            if (condition_)
                res_val_ = unquot_eval(std::move(val));
            
            break;
        }
        
        case (3) :
        {
            if (!condition_)
                res_val_ = unquot_eval(std::move(val));
            
            break;
        }
    }
}

ValPtr If::unquot_eval(Val &&val)
{
    bool has_err = false;
    ValPtr res_val = ::call(Eval(env_), std::move(val), &has_err);
    if (has_err)
    {
        reg_err(std::move(res_val));
        res_val = val::create_sym();
    }
    
    return res_val;
}

OpPtr robolisp::impl::op::create_if(Env *env)
{
    return OpPtr(new If(env));
}
