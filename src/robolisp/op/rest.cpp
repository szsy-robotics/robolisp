#include "rest.hpp"

#include <cstddef>
#include <string>
#include <utility>

#include "../op.hpp"

#include "../env.hpp"
#include "../val.hpp"
#include "../val/list.hpp"

using namespace robolisp::impl;
using namespace op;

ValPtr Rest::take_res()
{
    return val::create_list(std::move(res_val_list_));
}

std::string Rest::get_desc() const
{
    return "rest";
}

std::size_t Rest::get_min_args() const
{
    return 1;
}

std::size_t Rest::get_max_args() const
{
    return 1;
}

void Rest::process(val::List &&list)
{
    res_val_list_ = list.take();
    if (!res_val_list_.empty())
        res_val_list_.pop_front();
}

OpPtr robolisp::impl::op::create_rest(Env */*env*/)
{
    return OpPtr(new Rest());
}
