#include "let.hpp"

#include <cstddef>
#include <string>
#include <utility>

#include "../op.hpp"

#include "../env.hpp"
#include "../val.hpp"
#include "../val/list.hpp"
#include "../val/sym.hpp"
#include "../val/err.hpp"
#include "../op/eval.hpp"

using namespace robolisp::impl;
using namespace op;

Let::Let(Env *env) :
    enclosed_env_ {env}
{}

ValPtr Let::take_res()
{
    return val::create_list(std::move(res_val_list_));
}

std::string Let::get_desc() const
{
    return "let";
}

std::size_t Let::get_min_args() const
{
    return 1;
}

std::size_t Let::get_max_args() const
{
    return ARG_SIZE_INF;
}

void Let::process(val::List &&list)
{
    if (res_val_list_.empty() && get_arg_cnt() % 2 == 0)
        enclosed_env_.def_local(std::move(key_), list.acquire());
    else
        res_val_list_.push_back(unquot_eval(std::move(list)));
}

void Let::process(val::Sym &&sym)
{
    if (!res_val_list_.empty())
        reg_illegal_arg(sym);
    else if (get_arg_cnt() % 2 == 0)
        enclosed_env_.def_local(std::move(key_), sym.acquire());
    else
        key_ = std::move(sym);
}

void Let::default_process(Val &&val)
{
    if (res_val_list_.empty() && get_arg_cnt() % 2 == 0)
        enclosed_env_.def_local(std::move(key_), val.acquire());
    else
        reg_illegal_arg(val);
}

ValPtr Let::unquot_eval(Val &&val)
{
    bool has_err = false;
    ValPtr res_val = ::call(Eval(&enclosed_env_), std::move(val), &has_err);
    if (has_err)
    {
        reg_err(std::move(res_val));
        res_val = val::create_sym();
    }
    
    return res_val;
}

OpPtr robolisp::impl::op::create_let(Env *env)
{
    return OpPtr(new Let(env));
}
