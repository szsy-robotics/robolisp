#include "nth.hpp"

#include <cinttypes>
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

ValPtr Nth::take_res()
{
    return std::move(res_val_);
}

std::string Nth::get_desc() const
{
    return "nth";
}

std::size_t Nth::get_min_args() const
{
    return 2;
}

std::size_t Nth::get_max_args() const
{
    return 2;
}

void Nth::process(val::List &&list)
{
    if (get_arg_cnt() == 2)
    {
        size_t idx = 0;
        for (ValPtr &val : list.take())
        {
            if (idx == idx_)
            {
                res_val_ = std::move(val);
                return;
            }
            
            idx++;
        }
        
        reg_err(val::Err::LOGIC_ERR, "index out of bounds: length of list is '" + std::to_string(idx)
                                     + "', cannot access element '" + std::to_string(idx_) + "'");
    }
    else
    {
        reg_illegal_arg(list);
    }
}

void Nth::process(val::IntNum &&int_num)
{
    if (get_arg_cnt() == 1)
    {
        int32_t idx = int_num.get();
        if (idx >= 0)
            idx_ = idx;
        else
            reg_err(val::Err::LOGIC_ERR, "index of list cannot be negative: got '" + std::to_string(idx) + "'");
    }
    else
    {
        reg_illegal_arg(int_num);
    }
}

OpPtr robolisp::impl::op::create_nth(Env */*env*/)
{
    return OpPtr(new Nth());
}
