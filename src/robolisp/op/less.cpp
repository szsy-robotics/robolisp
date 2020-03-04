#include "less.hpp"

#include <cstddef>
#include <string>
#include <utility>
#include <variant>

#include "../op.hpp"

#include "../env.hpp"
#include "../val.hpp"
#include "../val/list.hpp"
#include "../val/int_num.hpp"
#include "../val/float_num.hpp"
#include "../val/sym.hpp"
#include "../val/str.hpp"
#include "../val/err.hpp"

using namespace robolisp::impl;
using namespace op;

ValPtr Less::take_res()
{
    return val::create_sym(res_);
}

std::string Less::get_desc() const
{
    return "less";
}

std::size_t Less::get_min_args() const
{
    return 2;
}

std::size_t Less::get_max_args() const
{
    return ARG_SIZE_INF;
}

void Less::process(val::List &&list)
{
    if (get_arg_cnt() == 1)
    {
        ref_val_ = std::move(list);
    }
    else if (std::holds_alternative<val::List>(ref_val_))
    {
        if (!res_)
            return;
        
        if(::call(create_less,
                  std::get<val::List>(ref_val_).get(),
                  list.get(),
                  nullptr,
                  [this](ValPtr val, bool is_err)
                  {
                      if (is_err)
                          reg_err(std::move(val));
                      else
                          res_ &= val->to_bool();
                  })) // lists have different lengths
        {
            reg_err(val::Err::LOGIC_ERR,
                    "cannot operate on lists with different lengths: '" + std::get<val::List>(ref_val_).to_str()
                    + "' and '" + list.to_str() + "'");
        }
        else // lists have the same length
        {
            ref_val_ = std::move(list);
        }
    }
    else
    {
        reg_illegal_arg(list);
    }
}

void Less::process(val::IntNum &&int_num)
{
    if (get_arg_cnt() == 1)
    {
        ref_val_ = std::move(int_num);
    }
    else if (std::holds_alternative<val::IntNum>(ref_val_))
    {
        if (!res_)
            return;
        
        res_ = std::get<val::IntNum>(ref_val_).get() < int_num.get();
        ref_val_ = std::move(int_num);
    }
    else if (std::holds_alternative<val::FloatNum>(ref_val_))
    {
        if (!res_)
            return;
        
        res_ = std::get<val::FloatNum>(ref_val_).get() < int_num.get();
        ref_val_ = std::move(int_num);
    }
    else
    {
        reg_illegal_arg(int_num);
    }
}

void Less::process(val::FloatNum &&float_num)
{
    if (get_arg_cnt() == 1)
    {
        ref_val_ = std::move(float_num);
    }
    else if (std::holds_alternative<val::FloatNum>(ref_val_))
    {
        if (!res_)
            return;
        
        res_ = std::get<val::FloatNum>(ref_val_).get() < float_num.get();
        ref_val_ = std::move(float_num);
    }
    else if (std::holds_alternative<val::IntNum>(ref_val_))
    {
        if (!res_)
            return;
        
        res_ = std::get<val::IntNum>(ref_val_).get() < float_num.get();
        ref_val_ = std::move(float_num);
    }
    else
    {
        reg_illegal_arg(float_num);
    }
}

void Less::process(val::Str &&str)
{
    if (get_arg_cnt() == 1)
    {
        ref_val_ = std::move(str);
    }
    else if (std::holds_alternative<val::Str>(ref_val_))
    {
        if (!res_)
            return;
        
        res_ = std::get<val::Str>(ref_val_).get() < str.get();
        ref_val_ = std::move(str);
    }
    else
    {
        reg_illegal_arg(str);
    }
}

OpPtr robolisp::impl::op::create_less(Env */*env*/)
{
    return OpPtr(new Less());
}
