#include "cond.hpp"

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

Cond::Cond(Env *env) :
    env_ {env}
{}

ValPtr Cond::take_res()
{
    return val::create_list(std::move(res_val_list_));
}

std::string Cond::get_desc() const
{
    return "cond";
}

std::size_t Cond::get_min_args() const
{
    return 2;
}

std::size_t Cond::get_max_args() const
{
    return ARG_SIZE_INF;
}

void Cond::default_process(Val &&val)
{
    if (get_arg_cnt() % 2 == 1)
    {
        condition_ = unquot_eval(std::move(val))->to_bool();
    }
    else
    {
        if (condition_)
            res_val_list_.push_back(unquot_eval(std::move(val)));
    }
}

ValPtr Cond::unquot_eval(Val &&val)
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

OpPtr robolisp::impl::op::create_cond(Env *env)
{
    return OpPtr(new Cond(env));
}
