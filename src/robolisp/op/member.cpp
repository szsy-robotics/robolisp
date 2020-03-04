#include "member.hpp"

#include <cstddef>
#include <string>
#include <utility>

#include "../op.hpp"

#include "../env.hpp"
#include "../val.hpp"
#include "../val/list.hpp"
#include "../val/sym.hpp"
#include "../op/equal.hpp"

using namespace robolisp::impl;
using namespace op;

ValPtr Member::take_res()
{
    return !res_val_list_.empty() ? val::create_list(std::move(res_val_list_)) : val::create_sym();
}

std::string Member::get_desc() const
{
    return "member";
}

std::size_t Member::get_min_args() const
{
    return 2;
}

std::size_t Member::get_max_args() const
{
    return 2;
}

void Member::process(val::List &&list)
{
    if (get_arg_cnt() == 2)
    {
        res_val_list_ = list.take();
        for (const ValPtr &val : res_val_list_)
        {
            bool has_err = false;
            ValPtr res_val = ::call(Equal(), std::move(*val_->copy()), std::move(*val->copy()), &has_err);
            if (has_err)
            {
                reg_err(std::move(res_val));
                return;
            }
            
            if (res_val->to_bool())
                break;
            
            res_val_list_.pop_front();
        }
    }
    else
    {
        reg_illegal_arg(list);
    }
}

void Member::default_process(Val &&val)
{
    if (get_arg_cnt() == 1)
        val_ = val.acquire();
    else
        reg_illegal_arg(val);
}

OpPtr robolisp::impl::op::create_member(Env */*env*/)
{
    return OpPtr(new Member());
}
