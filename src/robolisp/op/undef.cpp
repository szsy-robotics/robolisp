#include "undef.hpp"

#include <cstddef>
#include <string>
#include <utility>

#include "../op.hpp"

#include "../env.hpp"
#include "../val.hpp"
#include "../val/list.hpp"
#include "../val/sym.hpp"
#include "../val/err.hpp"

using namespace robolisp::impl;
using namespace op;

Undef::Undef(Env *env) :
    env_ {env}
{}

ValPtr Undef::take_res()
{
    return val::create_sym();
}

std::string Undef::get_desc() const
{
    return "undef";
}

std::size_t Undef::get_min_args() const
{
    return 1;
}

std::size_t Undef::get_max_args() const
{
    return ARG_SIZE_INF;
}

void Undef::process(val::List &&list)
{
    bool has_err = false;
    for (ValPtr &key : list.take())
    {
        ValPtr res_val = ::call(Undef(env_), std::move(*key), &has_err);
        if (has_err)
        {
            reg_err(std::move(res_val));
            return;
        }
    }
}

void Undef::process(val::Sym &&sym)
{
    env_->undef_global(sym);
}

OpPtr robolisp::impl::op::create_undef(Env *env)
{
    return OpPtr(new Undef(env));
}
