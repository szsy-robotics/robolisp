#include "equal.hpp"

#include <cmath>
#include <cstddef>
#include <limits>
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

static bool is_equal_float(float val0, float val1);

ValPtr Equal::take_res()
{
    return val::create_sym(res_);
}

std::string Equal::get_desc() const
{
    return "equal";
}

std::size_t Equal::get_min_args() const
{
    return 2;
}

std::size_t Equal::get_max_args() const
{
    return ARG_SIZE_INF;
}

void Equal::process(val::List &&list)
{
    if (get_arg_cnt() == 1)
    {
        ref_val_ = std::move(list);
    }
    else if (std::holds_alternative<val::List>(ref_val_))
    {
        if (!res_)
            return;
        
        if(::call(create_equal,
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
            res_ = false;
        }
    }
    else
    {
        res_ = false;
    }
}

void Equal::process(val::IntNum &&int_num)
{
    if (get_arg_cnt() == 1)
    {
        ref_val_ = std::move(int_num);
    }
    else if (std::holds_alternative<val::IntNum>(ref_val_))
    {
        if (!res_)
            return;
        
        res_ = std::get<val::IntNum>(ref_val_).get() == int_num.get();
    }
    else if (std::holds_alternative<val::FloatNum>(ref_val_))
    {
        if (!res_)
            return;
        
        res_ = is_equal_float(std::get<val::FloatNum>(ref_val_).get(), int_num.get());
    }
    else
    {
        res_ = false;
    }
}

void Equal::process(val::FloatNum &&float_num)
{
    if (get_arg_cnt() == 1)
    {
        ref_val_ = std::move(float_num);
    }
    else if (std::holds_alternative<val::FloatNum>(ref_val_))
    {
        if (!res_)
            return;
        
        res_ = is_equal_float(std::get<val::FloatNum>(ref_val_).get(), float_num.get());
    }
    else if (std::holds_alternative<val::IntNum>(ref_val_))
    {
        if (!res_)
            return;
        
        res_ = is_equal_float(std::get<val::IntNum>(ref_val_).get(), float_num.get());
    }
    else
    {
        res_ = false;
    }
}

void Equal::process(val::Sym &&sym)
{
    if (get_arg_cnt() == 1)
    {
        ref_val_ = std::move(sym);
    }
    else if (std::holds_alternative<val::Sym>(ref_val_))
    {
        if (!res_)
            return;
        
        res_ = std::get<val::Sym>(ref_val_).get() == sym.get();
    }
    else
    {
        res_ = false;
    }
}

void Equal::process(val::Str &&str)
{
    if (get_arg_cnt() == 1)
    {
        ref_val_ = std::move(str);
    }
    else if (std::holds_alternative<val::Str>(ref_val_))
    {
        if (!res_)
            return;
        
        res_ = std::get<val::Str>(ref_val_).get() == str.get();
    }
    else
    {
        res_ = false;
    }
}

OpPtr robolisp::impl::op::create_equal(Env */*env*/)
{
    return OpPtr(new Equal());
}

static bool is_equal_float(float val0, float val1)
{
    return std::fabs(val0 - val1) < std::numeric_limits<float>::epsilon();
}
