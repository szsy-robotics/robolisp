#include "logical_not.hpp"

#include <cstddef>
#include <string>
#include <utility>

#include "../op.hpp"

#include "../env.hpp"
#include "../val.hpp"
#include "../val/list.hpp"
#include "../val/sym.hpp"
#include "../val/err.hpp"

using namespace robolisp::impl;
using namespace op;

ValPtr LogicalNot::take_res()
{
    return std::move(res_val_);
}

std::string LogicalNot::get_desc() const
{
    return "logical_not";
}

std::size_t LogicalNot::get_min_args() const
{
    return 1;
}

std::size_t LogicalNot::get_max_args() const
{
    return 1;
}

void LogicalNot::process(val::List &&list)
{
    ValPtrList res_val_list = {};
    for (ValPtr &val : list.take())
    {
        bool has_err = false;
        ValPtr res_val = ::call(LogicalNot(), std::move(*val), &has_err);
        if (has_err)
        {
            reg_err(std::move(res_val));
            return;
        }
        
        res_val_list.push_back(std::move(res_val));
    }
    
    res_val_ = val::create_list(std::move(res_val_list));
}

void LogicalNot::default_process(Val &&val)
{
    res_val_ = val::create_sym(!val.to_bool());
}

OpPtr robolisp::impl::op::create_logical_not(Env */*env*/)
{
    return OpPtr(new LogicalNot());
}
