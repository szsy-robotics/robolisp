#include "sort.hpp"

#include <cinttypes>
#include <cstddef>
#include <string>
#include <utility>

#include "../op.hpp"

#include "../apply.hpp"
#include "../env.hpp"
#include "../val.hpp"
#include "../val/list.hpp"
#include "../val/sym.hpp"
#include "../val/lambda.hpp"
#include "../val/builtin_op.hpp"

using namespace robolisp::impl;
using namespace op;

Sort::Sort(Env *env) :
    env_ {env}
{}

ValPtr Sort::take_res()
{
    return val::create_list(std::move(res_val_list_));
}

std::string Sort::get_desc() const
{
    return "sort";
}

std::size_t Sort::get_min_args() const
{
    return 2;
}

std::size_t Sort::get_max_args() const
{
    return 2;
}

void Sort::process(val::List &&list)
{
    if (get_arg_cnt() == 2)
    {
        res_val_list_ = list.take();
        res_val_list_.sort([this](const ValPtr &lhs, const ValPtr &rhs)
                           {
                                ValPtrList val_list = {};
                                val_list.push_back(lhs->copy());
                                val_list.push_back(rhs->copy());
                                val::List args = val::List(std::move(val_list), Quot::QUOT);
                                bool has_err = false;
                                ValPtr res_val = apply(op_->copy(), &args, env_, &has_err);
                                bool condition = res_val->to_bool();
                                if (has_err)
                                {
                                    reg_err(std::move(res_val));
                                    condition = false;
                                }
                                
                                return condition;
                            });
    }
    else
    {
        reg_illegal_arg(list);
    }
}

void Sort::process(val::Lambda &&lambda)
{
    if (get_arg_cnt() == 1)
        op_ = lambda.acquire();
    else
        reg_illegal_arg(lambda);
}

void Sort::process(val::BuiltinOp &&builtin_op)
{
    if (get_arg_cnt() == 1)
        op_ = builtin_op.acquire();
    else
        reg_illegal_arg(builtin_op);
}

OpPtr robolisp::impl::op::create_sort(Env *env)
{
    return OpPtr(new Sort(env));
}
