#include "list.hpp"

#include <cstddef>
#include <string>
#include <utility>

#include "../op.hpp"

#include "../env.hpp"
#include "../val.hpp"
#include "../val/list.hpp"

using namespace robolisp::impl;
using namespace op;

ValPtr List::take_res()
{
    return val::create_list(std::move(res_val_list_));
}

std::string List::get_desc() const
{
    return "list";
}

std::size_t List::get_min_args() const
{
    return 0;
}

std::size_t List::get_max_args() const
{
    return ARG_SIZE_INF;
}

void List::default_process(Val &&val)
{
    res_val_list_.push_back(val.acquire());
}

OpPtr robolisp::impl::op::create_list(Env */*env*/)
{
    return OpPtr(new List());
}
