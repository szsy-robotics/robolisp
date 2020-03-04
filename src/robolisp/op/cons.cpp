#include "cons.hpp"

#include <cstddef>
#include <string>
#include <utility>

#include "../op.hpp"

#include "../env.hpp"
#include "../val.hpp"
#include "../val/list.hpp"

using namespace robolisp::impl;
using namespace op;

ValPtr Cons::take_res()
{
    return val::create_list(std::move(res_val_list_));
}

std::string Cons::get_desc() const
{
    return "cons";
}

std::size_t Cons::get_min_args() const
{
    return 2;
}

std::size_t Cons::get_max_args() const
{
    return 2;
}

void Cons::process(val::List &&list)
{
    if (get_arg_cnt() == 2)
    {
        res_val_list_ = list.take();
        res_val_list_.push_front(std::move(val_));
    }
    else
    {
        reg_illegal_arg(list);
    }
}

void Cons::default_process(Val &&val)
{
    if (get_arg_cnt() == 1)
        val_ = val.acquire();
    else
        reg_illegal_arg(val);
}

OpPtr robolisp::impl::op::create_cons(Env */*env*/)
{
    return OpPtr(new Cons());
}
