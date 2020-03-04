#include "drop.hpp"

#include <cstddef>
#include <string>
#include <utility>

#include "../op.hpp"

#include "../env.hpp"
#include "../val.hpp"
#include "../val/list.hpp"
#include "../val/int_num.hpp"

using namespace robolisp::impl;
using namespace op;

ValPtr Drop::take_res()
{
    return val::create_list(std::move(res_val_list_));
}

std::string Drop::get_desc() const
{
    return "drop";
}

std::size_t Drop::get_min_args() const
{
    return 2;
}

std::size_t Drop::get_max_args() const
{
    return 2;
}

void Drop::process(val::List &&list)
{
    if (get_arg_cnt() == 2)
    {
        res_val_list_ = list.take();
        while (len_ > 0 && !res_val_list_.empty())
        {
            res_val_list_.pop_front();
            len_--;
        }
    }
    else
    {
        reg_illegal_arg(list);
    }
}

void Drop::process(val::IntNum &&int_num)
{
    if (get_arg_cnt() == 1)
        len_ = int_num.get();
    else
        reg_illegal_arg(int_num);
}

OpPtr robolisp::impl::op::create_drop(Env */*env*/)
{
    return OpPtr(new Drop());
}
