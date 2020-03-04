#include "foreach.hpp"

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

Foreach::Foreach(Env *env) :
    env_ {env}
{}

ValPtr Foreach::take_res()
{
    ValPtrList res_val_list = {};
    Env env = Env(env_);
    env.def_local(val::Sym(sym_), val::create_sym());
    for (ValPtr &val : list_.take())
    {
        ValPtrList val_list = {};
        env.set(sym_, std::move(val));
        for (const ValPtr &arg : args_)
            val_list.push_back(unquot_eval(arg->copy(), &env));
        
        res_val_list.push_back(val::create_list(std::move(val_list)));
    }
    
    return val::create_list(std::move(res_val_list));
}

std::string Foreach::get_desc() const
{
    return "foreach";
}

std::size_t Foreach::get_min_args() const
{
    return 3;
}

std::size_t Foreach::get_max_args() const
{
    return ARG_SIZE_INF;
}

void Foreach::process(val::List &&list)
{
    if (get_arg_cnt() == 2)
        list_ = std::move(list);
    else if (get_arg_cnt() >= 3)
        args_.push_back(list.acquire());
    else
        reg_illegal_arg(list);
}

void Foreach::process(val::Sym &&sym)
{
    if (get_arg_cnt() == 1)
        sym_ = std::move(sym);
    else if (get_arg_cnt() >= 3)
        args_.push_back(sym.acquire());
    else
        reg_illegal_arg(sym);
}

void Foreach::default_process(Val &&val)
{
    if (get_arg_cnt() >= 3)
        args_.push_back(val.acquire());
    else
        reg_illegal_arg(val);
}

ValPtr Foreach::unquot_eval(ValPtr val, Env *env)
{
    bool has_err = false;
    ValPtr res_val = ::call(Eval(env), std::move(*val), &has_err);
    if (has_err)
    {
        reg_err(std::move(res_val));
        res_val = val::create_sym();
    }
    
    return res_val;
}

OpPtr robolisp::impl::op::create_foreach(Env *env)
{
    return OpPtr(new Foreach(env));
}
