#include "append.hpp"

#include <cstddef>
#include <string>
#include <utility>

#include "../op.hpp"

#include "../env.hpp"
#include "../val.hpp"
#include "../val/list.hpp"

using namespace robolisp::impl;
using namespace op;

ValPtr Append::take_res()
{
    return val::create_list(std::move(res_val_list_));
}

std::string Append::get_desc() const
{
    return "append";
}

std::size_t Append::get_min_args() const
{
    return 2;
}

std::size_t Append::get_max_args() const
{
    return 2;
}

void Append::process(val::List &&list)
{
    if (get_arg_cnt() == 1)
        res_val_list_ = list.take();
    else
        res_val_list_.splice(res_val_list_.cend(), list.take());
}

OpPtr robolisp::impl::op::create_append(Env */*env*/)
{
    return OpPtr(new Append());
}
