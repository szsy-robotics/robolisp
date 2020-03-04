#include "exp.hpp"

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

ValPtr Exp::take_res()
{
    return std::move(res_val_);
}

std::string Exp::get_desc() const
{
    return "exp";
}

std::size_t Exp::get_min_args() const
{
    return 1;
}

std::size_t Exp::get_max_args() const
{
    return 1;
}

void Exp::process(val::List &&list)
{
    ValPtrList res_val_list = {};
    for (ValPtr &val : list.take())
    {
        bool has_err = false;
        ValPtr res_val = ::call(Exp(), std::move(*val), &has_err);
        if (has_err)
        {
            reg_err(std::move(res_val));
            return;
        }
        
        res_val_list.push_back(std::move(res_val));
    }
    
    res_val_ = val::create_list(std::move(res_val_list));
}

void Exp::process(val::IntNum &&int_num)
{
    res_val_ = val::create_float_num(std::exp(int_num.get()));
}

void Exp::process(val::FloatNum &&float_num)
{
    res_val_ = val::create_float_num(std::exp(float_num.get()));
}

OpPtr robolisp::impl::op::create_exp(Env */*env*/)
{
    return OpPtr(new Exp());
}
