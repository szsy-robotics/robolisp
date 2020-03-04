#include "while.hpp"

#include <cstddef>
#include <string>
#include <utility>

#include "../op.hpp"

#include "../env.hpp"
#include "../val.hpp"
#include "../val/list.hpp"
#include "../val/sym.hpp"
#include "../op/eval.hpp"

using namespace robolisp::impl;
using namespace op;

While::While(Env *env) :
    env_ {env}
{}

ValPtr While::take_res()
{
    ValPtrList res_val_list = {};
    while (unquot_eval(condition_->copy())->to_bool())
    {
        ValPtrList val_list = {};
        for (const ValPtr &arg : args_)
            val_list.push_back(unquot_eval(arg->copy()));
        
        res_val_list.push_back(val::create_list(std::move(val_list)));
    }
    
    return val::create_list(std::move(res_val_list));
}

std::string While::get_desc() const
{
    return "while";
}

std::size_t While::get_min_args() const
{
    return 2;
}

std::size_t While::get_max_args() const
{
    return ARG_SIZE_INF;
}

void While::default_process(Val &&val)
{
    if (get_arg_cnt() == 1)
        condition_ = val.acquire();
    else
        args_.push_back(val.acquire());
}

ValPtr While::unquot_eval(ValPtr val)
{
    bool has_err = false;
    ValPtr res_val = ::call(Eval(env_), std::move(*val), &has_err);
    if (has_err)
    {
        reg_err(std::move(res_val));
        res_val = val::create_sym();
    }
    
    return res_val;
}

OpPtr robolisp::impl::op::create_while(Env *env)
{
    return OpPtr(new While(env));
}
