#include "lambda.hpp"

#include <cstddef>
#include <string>
#include <utility>

#include "../op.hpp"

#include "../env.hpp"
#include "../val.hpp"
#include "../val/list.hpp"
#include "../val/sym.hpp"
#include "../val/lambda.hpp"
#include "../val/err.hpp"

using namespace robolisp::impl;
using namespace op;

Lambda::Lambda(Env *env) :
    env_ {env}
{}

ValPtr Lambda::take_res()
{
    return std::move(res_val_);
}

std::string Lambda::get_desc() const
{
    return "lambda";
}

std::size_t Lambda::get_min_args() const
{
    return 1;
}

std::size_t Lambda::get_max_args() const
{
    return ARG_SIZE_INF;
}

void Lambda::process(val::List &&list)
{
    if (!res_val_)
        res_val_ = val::create_lambda(std::move(formals_), std::move(list), env_);
    else
        reg_illegal_arg(list);
}

void Lambda::process(val::Sym &&sym)
{
    if (!res_val_)
    {
        if (sym.is_reserved())
        {
            reg_err(val::Err::LOGIC_ERR, "illegal use of reserved symbol '" + sym.to_str() + "' as lambda formal");
            return;
        }
        
        formals_.push_back(std::move(sym));
    }
    else
    {
        reg_illegal_arg(sym);
    }
}

OpPtr robolisp::impl::op::create_lambda(Env *env)
{
    return OpPtr(new Lambda(env));
}
