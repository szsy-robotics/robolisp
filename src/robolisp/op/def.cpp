#include "def.hpp"

#include <cstddef>
#include <string>
#include <utility>
#include <variant>

#include "../op.hpp"

#include "../env.hpp"
#include "../val.hpp"
#include "../val/list.hpp"
#include "../val/sym.hpp"
#include "../val/err.hpp"

using namespace robolisp::impl;
using namespace op;

Def::Def(Env *env) :
    env_ {env}
{}

ValPtr Def::take_res()
{
    if (get_arg_cnt() % 2 != 0)
        reg_err(val::Err::ILLEGAL_ARG, "number of arguments must be even: got'" + std::to_string(get_arg_cnt()) + "'");
    
    return val::create_sym();
}

std::string Def::get_desc() const
{
    return "def";
}

std::size_t Def::get_min_args() const
{
    return 2;
}

std::size_t Def::get_max_args() const
{
    return ARG_SIZE_INF;
}

void Def::process(val::List &&list)
{
    if (std::holds_alternative<val::List>(key_))
    {
        if(::call(create_def,
                  std::get<val::List>(key_).get(),
                  list.get(),
                  env_,
                  [this](ValPtr val, bool is_err)
                  {
                      if (is_err)
                          reg_err(std::move(val));
                  })) // lists have different lengths
        {
            reg_err(val::Err::LOGIC_ERR,
                    "cannot operate on lists with different lengths: '" + std::get<val::List>(key_).to_str()
                    + "' and '" + list.to_str() + "'");
        }
        else // lists have the same length
        {
            key_ = std::monostate();
        }
    }
    else if (std::holds_alternative<val::Sym>(key_))
    {
        env_->def_global(std::move(std::get<val::Sym>(key_)), list.acquire());
        key_ = std::monostate();
    }
    else
    {
        key_ = std::move(list);
    }
}

void Def::process(val::Sym &&sym)
{
    if (std::holds_alternative<val::Sym>(key_))
    {
        env_->def_global(std::move(std::get<val::Sym>(key_)), sym.acquire());
        key_ = std::monostate();
    }
    else if (std::holds_alternative<val::List>(key_))
    {
        reg_illegal_arg(sym);
    }
    else
    {
        key_ = std::move(sym);
    }
}

void Def::default_process(Val &&val)
{
    if (std::holds_alternative<val::Sym>(key_))
    {
        env_->def_global(std::move(std::get<val::Sym>(key_)), val.acquire());
        key_ = std::monostate();
    }
    else
    {
        reg_illegal_arg(val);
    }
}

OpPtr robolisp::impl::op::create_def(Env *env)
{
    return OpPtr(new Def(env));
}
