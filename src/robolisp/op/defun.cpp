#include "defun.hpp"

#include <cstddef>
#include <string>
#include <utility>

#include "../op.hpp"

#include "../env.hpp"
#include "../val.hpp"
#include "../val/list.hpp"
#include "../val/sym.hpp"
#include "../val/err.hpp"
#include "../op/lambda.hpp"

using namespace robolisp::impl;
using namespace op;

Defun::Defun(Env *env) :
    env_ {env}
{}

ValPtr Defun::take_res()
{
    return val::create_sym();
}

std::string Defun::get_desc() const
{
    return "defun";
}

std::size_t Defun::get_min_args() const
{
    return 2;
}

std::size_t Defun::get_max_args() const
{
    return 2;
}

void Defun::process(val::List &&list)
{
    if (get_arg_cnt() == 2)
    {
        bool has_err = false;
        ValPtr res_val = ::call(Lambda(env_), list.take(), &has_err);
        if (has_err)
        {
            reg_err(std::move(res_val));
            return;
        }
        
        env_->def_global(std::move(sym_), std::move(res_val));
    }
    else
    {
        reg_illegal_arg(list);
    }
}

void Defun::process(val::Sym &&sym)
{
    if (get_arg_cnt() == 1)
        sym_ = std::move(sym);
    else
        reg_illegal_arg(sym);
}

OpPtr robolisp::impl::op::create_defun(Env *env)
{
    return OpPtr(new Defun(env));
}
