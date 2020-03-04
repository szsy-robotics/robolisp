#include "log.hpp"

#include <cmath>
#include <cstddef>
#include <string>
#include <utility>

#include "../op.hpp"

#include "../env.hpp"
#include "../val.hpp"
#include "../val/list.hpp"
#include "../val/int_num.hpp"
#include "../val/float_num.hpp"
#include "../val/err.hpp"

using namespace robolisp::impl;
using namespace op;

ValPtr Log::take_res()
{
    return std::move(res_val_);
}

std::string Log::get_desc() const
{
    return "log";
}

std::size_t Log::get_min_args() const
{
    return 1;
}

std::size_t Log::get_max_args() const
{
    return 1;
}

void Log::process(val::List &&list)
{
    ValPtrList res_val_list = {};
    for (ValPtr &val : list.take())
    {
        bool has_err = false;
        ValPtr res_val = ::call(Log(), std::move(*val), &has_err);
        if (has_err)
        {
            reg_err(std::move(res_val));
            return;
        }
        
        res_val_list.push_back(std::move(res_val));
    }
    
    res_val_ = val::create_list(std::move(res_val_list));
}

void Log::process(val::IntNum &&int_num)
{
    res_val_ = val::create_float_num(std::log(int_num.get()));
}

void Log::process(val::FloatNum &&float_num)
{
    res_val_ = val::create_float_num(std::log(float_num.get()));
}

OpPtr robolisp::impl::op::create_log(Env */*env*/)
{
    return OpPtr(new Log());
}
