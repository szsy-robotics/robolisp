#include "op.hpp"

#include <cstddef>
#include <string>
#include <utility>

#include "env.hpp"
#include "val.hpp"
#include "val/list.hpp"
#include "val/int_num.hpp"
#include "val/float_num.hpp"
#include "val/sym.hpp"
#include "val/str.hpp"
#include "val/lambda.hpp"
#include "val/builtin_op.hpp"
#include "val/err.hpp"

using namespace robolisp::impl;

ValPtr Op::eval_res()
{
    ValPtr res_val = take_res();
    if (!chk_res_validity())
        res_val = std::move(err_);
    
    if (!res_val)
        res_val = val::create_err(val::Err::GENERAL_FAILURE, "result of operator is null", to_str()); // for debugging
    
    return res_val;
}

ValPtr Op::eval_res(bool *has_err)
{
    *has_err = false;
    ValPtr res_val = take_res();
    if (!chk_res_validity())
    {
        res_val = std::move(err_);
        *has_err = true;
    }
    
    if (!res_val)
    {
        res_val = val::create_err(val::Err::GENERAL_FAILURE, "result of operator is null", to_str()); // for debugging
        *has_err = true;
    }
    
    return res_val;
}

void Op::visit(val::List *list)
{
    if (chk_arg_validity())
        process(std::move(*list));
}

void Op::visit(val::IntNum *int_num)
{
    if (chk_arg_validity())
        process(std::move(*int_num));
}

void Op::visit(val::FloatNum *float_num)
{
    if (chk_arg_validity())
        process(std::move(*float_num));
}

void Op::visit(val::Sym *sym)
{
    if (chk_arg_validity())
        process(std::move(*sym));
}

void Op::visit(val::Str *str)
{
    if (chk_arg_validity())
        process(std::move(*str));
}

void Op::visit(val::Lambda *lambda)
{
    if (chk_arg_validity())
        process(std::move(*lambda));
}

void Op::visit(val::BuiltinOp *builtin_op)
{
    if (chk_arg_validity())
        process(std::move(*builtin_op));
}

void Op::visit(val::Err *err)
{
    if (chk_arg_validity())
        process(std::move(*err));
}

void Op::call(Val &&arg)
{
    arg.accept_vis(this);
}

void Op::call(ValPtrList &&args)
{
    for (ValPtr &arg : args)
    {
        call(std::move(*arg));
        if (has_err_)
            break;
    }
}

std::string Op::to_str() const
{
    return "<built-in operation '" + get_desc() + "'>";
}

std::size_t Op::get_arg_cnt() const
{
    return arg_cnt_;
}

void Op::reg_err(ValPtr err) // TODO: implement err description insertion for recursive apply of operations
{
    if (!has_err_)
    {
        has_err_ = true;
        err_ = std::move(err);
        if (!err_)
            err_ = val::create_err(val::Err::GENERAL_FAILURE, "error registered by operator is null", to_str());
    }
}

void Op::reg_err(val::Err::ErrType type, const std::string &desc)
{
    reg_err(create_err(type, desc, to_str()));
}

void Op::reg_illegal_arg(const Val &val)
{
    reg_err(val::Err::ILLEGAL_ARG, "argument " + std::to_string(arg_cnt_) + ": '" + val.to_str() + "'");
}

bool Op::chk_res_validity()
{
    bool is_valid = true;
    if (has_err_)
    {
        is_valid = false;
    }
    else if (is_below_min_args())
    {
        is_valid = false;
        reg_err(val::Err::ILLEGAL_ARG,
                "too few arguments: expected at least " + std::to_string(get_min_args())
                + ", got " + std::to_string(arg_cnt_));
    }
    else if (is_above_max_args())
    {
        is_valid = false;
        reg_err(val::Err::ILLEGAL_ARG,
                "too many arguments: expected at most " + std::to_string(get_max_args())
                + ", got " + std::to_string(arg_cnt_));
    }
    
    return is_valid;
}

bool Op::chk_arg_validity()
{
    arg_cnt_++;
    return !(has_err_ || is_above_max_args());
}

bool Op::is_below_min_args() const
{
    return arg_cnt_ < get_min_args();
}

bool Op::is_above_max_args() const
{
    return arg_cnt_ > get_max_args() && get_max_args() != ARG_SIZE_INF;
}

void Op::process(val::List &&list)
{
    default_process(std::move(list));
}

void Op::process(val::IntNum &&int_num)
{
    default_process(std::move(int_num));
}

void Op::process(val::FloatNum &&float_num)
{
    default_process(std::move(float_num));
}

void Op::process(val::Sym &&sym)
{
    default_process(std::move(sym));
}

void Op::process(val::Str &&str)
{
    default_process(std::move(str));
}

void Op::process(val::Lambda &&lambda)
{
    default_process(std::move(lambda));
}

void Op::process(val::BuiltinOp &&builtin_op)
{
    default_process(std::move(builtin_op));
}

void Op::process(val::Err &&err)
{
    reg_err(val::append_err_desc(err, to_str()));
}

void Op::default_process(Val &&val)
{
    reg_illegal_arg(val);
}

ValPtr op::call(Op &&op, Val &&val)
{
    op.call(std::move(val));
    ValPtr res_val = op.eval_res();
    return res_val;
}

ValPtr op::call(Op &&op, Val &&val0, Val &&val1)
{
    op.call(std::move(val0));
    op.call(std::move(val1));
    ValPtr res_val = op.eval_res();
    return res_val;
}

ValPtr op::call(Op &&op, ValPtrList &&val_list)
{
    op.call(std::move(val_list));
    ValPtr res_val = op.eval_res();
    return res_val;
}

ValPtr op::call(Op &&op, Val &&val, bool *has_err)
{
    op.call(std::move(val));
    ValPtr res_val = op.eval_res(has_err);
    return res_val;
}

ValPtr op::call(Op &&op, Val &&val0, Val &&val1, bool *has_err)
{
    op.call(std::move(val0));
    op.call(std::move(val1));
    ValPtr res_val = op.eval_res(has_err);
    return res_val;
}

ValPtr op::call(Op &&op, ValPtrList &&val_list, bool *has_err)
{
    op.call(std::move(val_list));
    ValPtr res_val = op.eval_res(has_err);
    return res_val;
}

bool op::call(const OpCreator &op_creator,
              const ValPtrList &val_list_0,
              const ValPtrList &val_list_1,
              Env *env,
              const std::function<void(ValPtr, bool)> &callback) // TODO: optimize using templates
{
    auto val_iter_0 = val_list_0.cbegin();
    auto val_iter_1 = val_list_1.cbegin();
    ValPtr res_val = nullptr;
    bool has_err = false;
    while (val_iter_0 != val_list_0.cend() && val_iter_1 != val_list_1.cend())
    {
        res_val = call(std::move(*op_creator(env)),
                       std::move(*(*val_iter_0)->copy()),
                       std::move(*(*val_iter_1)->copy()),
                       &has_err);
        callback(std::move(res_val), has_err);
        val_iter_0++;
        val_iter_1++;
    }
    
    return val_iter_0 != val_list_0.cend() || val_iter_1 != val_list_1.cend(); // lists have different lengths
}
