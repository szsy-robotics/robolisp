#include "eval.hpp"

#include <cstddef>
#include <string>
#include <utility>

#include "../op.hpp"

#include "../env.hpp"
#include "../val.hpp"
#include "../val/list.hpp"
#include "../val/sym.hpp"
#include "../op/forward.hpp"

using namespace robolisp::impl;
using namespace op;

Eval::Eval(Env *env) :
    env_ {env}
{}

ValPtr Eval::take_res()
{
    return std::move(res_val_);
}

std::string Eval::get_desc() const
{
    return "eval";
}

std::size_t Eval::get_min_args() const
{
    return 1;
}

std::size_t Eval::get_max_args() const
{
    return 1;
}

void Eval::process(val::List &&list)
{
    bool has_err = false;
    res_val_ = val::unquot_eval(std::move(list), env_, &has_err);
    if (has_err)
        reg_err(std::move(res_val_));
}

void Eval::process(val::Sym &&sym)
{
    bool has_err = false;
    res_val_ = val::unquot_eval(std::move(sym), env_, &has_err);
    if (has_err)
        reg_err(std::move(res_val_));
}

void Eval::default_process(Val &&val)
{
    res_val_ = val.acquire();
}

OpPtr robolisp::impl::op::create_eval(Env *env)
{
    return OpPtr(new Eval(env));
}
