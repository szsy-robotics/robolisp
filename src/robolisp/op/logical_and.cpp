#include "logical_and.hpp"

#include <cstddef>
#include <string>
#include <utility>
#include <variant>

#include "../op.hpp"

#include "../env.hpp"
#include "../val.hpp"
#include "../val/list.hpp"
#include "../val/sym.hpp"
#include "../val/err.hpp"

using namespace robolisp::impl;
using namespace op;

ValPtr LogicalAnd::take_res()
{
    ValPtr res_val = nullptr;
    if (std::holds_alternative<ValPtrList>(res_))
        res_val = val::create_list(std::move(std::get<ValPtrList>(res_)));
    else if (std::holds_alternative<bool>(res_))
        res_val = val::create_sym(std::get<bool>(res_));
    
    return res_val;
}

std::string LogicalAnd::get_desc() const
{
    return "logical_and";
}

std::size_t LogicalAnd::get_min_args() const
{
    return 2;
}

std::size_t LogicalAnd::get_max_args() const
{
    return ARG_SIZE_INF;
}

void LogicalAnd::process(val::List &&list)
{
    if (get_arg_cnt() == 1)
    {
        res_ = list.take();
    }
    else if (std::holds_alternative<ValPtrList>(res_))
    {
        ValPtrList res_val_list = {};
        if(::call(create_logical_and,
                  std::get<ValPtrList>(res_),
                  list.get(),
                  nullptr,
                  [this, &res_val_list](ValPtr val, bool is_err)
                  {
                      if (is_err)
                          reg_err(std::move(val));
                      else
                          res_val_list.push_back(std::move(val));
                  })) // lists have different lengths
        {
            reg_err(val::Err::LOGIC_ERR,
                    "cannot operate on lists with different lengths: '" + val::to_str(std::get<ValPtrList>(res_))
                    + "' and '" + list.to_str() + "'");
        }
        else // lists have the same length
        {
            res_ = std::move(res_val_list);
        }
    }
    else
    {
        reg_illegal_arg(list);
    }
}

void LogicalAnd::default_process(Val &&val)
{
    if (get_arg_cnt() == 1)
        res_ = val.to_bool();
    else if (std::holds_alternative<bool>(res_))
        std::get<bool>(res_) &= val.to_bool();
    else
        reg_illegal_arg(val);
}

OpPtr robolisp::impl::op::create_logical_and(Env */*env*/)
{
    return OpPtr(new LogicalAnd());
}
