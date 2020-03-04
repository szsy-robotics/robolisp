#include "take.hpp"

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

ValPtr Take::take_res()
{
    return val::create_list(std::move(res_val_list_));
}

std::string Take::get_desc() const
{
    return "take";
}

std::size_t Take::get_min_args() const
{
    return 2;
}

std::size_t Take::get_max_args() const
{
    return 2;
}

void Take::process(val::List &&list)
{
    if (get_arg_cnt() == 2)
    {
        for (ValPtr &val : list.take())
        {
            if (len_ <= 0)
                break;
            
            res_val_list_.push_back(std::move(val));
            len_--;
        }
    }
    else
    {
        reg_illegal_arg(list);
    }
}

void Take::process(val::IntNum &&int_num)
{
    if (get_arg_cnt() == 1)
        len_ = int_num.get();
    else
        reg_illegal_arg(int_num);
}

OpPtr robolisp::impl::op::create_take(Env */*env*/)
{
    return OpPtr(new Take());
}
