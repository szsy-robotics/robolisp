#include "apply.hpp"

#include <cstddef>
#include <string>
#include <utility>
#include <variant>

#include "op.hpp"
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

namespace
{

class Apply : public Op
{
  public :
    
    explicit Apply(Env *env);
    
  private :
    
    Env *env_;
    ValPtr res_val_ = nullptr;
    std::variant<std::monostate, val::Lambda, val::BuiltinOp> func_ = std::monostate();
    
    ValPtr take_res() override;
    std::string get_desc() const override;
    std::size_t get_min_args() const override;
    std::size_t get_max_args() const override;
    
    void process(val::List &&list) override;
    void process(val::Lambda &&lambda) override;
    void process(val::BuiltinOp &&builtin_op) override;
    
    ValPtr eval_lambda(val::Lambda &&lambda, val::List &&args);
};

}
// namespace

Apply::Apply(Env *env) :
    env_ {env}
{}

ValPtr Apply::take_res()
{
    return std::move(res_val_);
}

std::string Apply::get_desc() const
{
    return "apply";
}

std::size_t Apply::get_min_args() const
{
    return 2;
}

std::size_t Apply::get_max_args() const
{
    return 2;
}

void Apply::process(val::List &&list)
{
    if (get_arg_cnt() == 2)
    {
        if (std::holds_alternative<val::Lambda>(func_))
        {
            res_val_ = eval_lambda(std::move(std::get<val::Lambda>(func_)), std::move(list));
        }
        else
        {
            bool has_err = false;
            res_val_ = op::call(std::move(*std::get<val::BuiltinOp>(func_).get()(env_)), list.take(), &has_err);
            if (has_err)
                reg_err(std::move(res_val_));
        }
    }
    else
    {
        reg_illegal_arg(list);
    }
}

void Apply::process(val::Lambda &&lambda)
{
    if (get_arg_cnt() == 1)
        func_ = std::move(lambda);
    else
        reg_illegal_arg(lambda);
}

void Apply::process(val::BuiltinOp &&builtin_op)
{
    if (get_arg_cnt() == 1)
        func_ = std::move(builtin_op);
    else
        reg_illegal_arg(builtin_op);
}

ValPtr Apply::eval_lambda(val::Lambda &&lambda, val::List &&args)
{
    static const val::Sym DYNAMIC_SCOPING_ENA_SYM = val::Sym("dynamic-scoping-ena");
    bool has_err = false;
    ValPtr res_val = lambda.eval_lambda(std::move(args),
                                        env_->lookup(DYNAMIC_SCOPING_ENA_SYM)->to_bool() ? env_ : nullptr,
                                        &has_err);
    if (has_err)
    {
        reg_err(std::move(res_val));
        res_val = nullptr;
    }
    
    return res_val;
}

ValPtr robolisp::impl::apply(ValPtr func, val::List *args, Env *env)
{
    return op::call(Apply(env), std::move(*func), std::move(*args));
}

ValPtr robolisp::impl::apply(ValPtr func, val::List *args, Env *env, bool *has_err)
{
    return op::call(Apply(env), std::move(*func), std::move(*args), has_err);
}
