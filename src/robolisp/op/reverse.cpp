#include "reverse.hpp"

#include <cstddef>
#include <string>
#include <utility>

#include "../op.hpp"

#include "../env.hpp"
#include "../val.hpp"
#include "../val/list.hpp"

using namespace robolisp::impl;
using namespace op;

ValPtr Reverse::take_res()
{
    return val::create_list(std::move(res_val_list_));
}

std::string Reverse::get_desc() const
{
    return "reverse";
}

std::size_t Reverse::get_min_args() const
{
    return 1;
}

std::size_t Reverse::get_max_args() const
{
    return 1;
}

void Reverse::process(val::List &&list)
{
    res_val_list_ = list.take();
    res_val_list_.reverse();
}

OpPtr robolisp::impl::op::create_reverse(Env */*env*/)
{
    return OpPtr(new Reverse());
}
