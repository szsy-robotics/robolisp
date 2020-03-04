#include "lambda.hpp"

#include <cinttypes>
#include <string>
#include <utility>

#include "../val.hpp"
#include "../val/list.hpp"
#include "../val/sym.hpp"
#include "../val/err.hpp"
#include "../val_vis.hpp"

using namespace robolisp::impl;
using namespace val;

Lambda::Lambda(SymVect formals, List body, Env *env) :
    formals_ {std::move(formals)},
    body_ {std::move(body)},
    env_ {env->merge_copy()}
{}

ValPtr Lambda::copy() const
{
    return ValPtr(new Lambda(*this));
}

ValPtr Lambda::acquire()
{
    return ValPtr(new Lambda(std::move(*this)));
}

std::string Lambda::to_str() const
{
    std::string res_str = "<lambda (";
    for (auto sym_iter = formals_.cbegin(); sym_iter != formals_.cend(); sym_iter++)
    {
        if (sym_iter != formals_.cbegin())
            res_str += " ";
        
        res_str += (sym_iter)->to_str();
    }
    
    return res_str + ") " + body_.to_str() + ">";
}

const SymVect &Lambda::get_formals() const
{
    return formals_;
}

const List &Lambda::get_body() const
{
    return body_;
}

void Lambda::accept_vis(ValVis *val_vis)
{
    val_vis->visit(this);
}

SymVect Lambda::take_formals()
{
    return std::move(formals_);
}

List Lambda::take_body()
{
    return std::move(body_);
}

ValPtr Lambda::eval_lambda(List args, Env *context_env, bool *has_err)
{
    ValPtr res_val_ = nullptr;
    Env env = Env(context_env ? context_env : &env_);
    size_t idx = 0;
    for (ValPtr &val : args.take())
    {
        if (idx < formals_.size())
            env.def_local(std::move(formals_[idx]), std::move(val));
        
        idx++;
    }
    
    if (idx == formals_.size())
    {
        res_val_ = unquot_eval(std::move(body_), &env, has_err);
    }
    else
    {
        *has_err = true;
        if (idx < formals_.size())
        {
            res_val_ = create_err(val::Err::ILLEGAL_ARG,
                                  "too few arguments: expected " + std::to_string(formals_.size())
                                  + ", got " + std::to_string(idx));
        }
        else
        {
            res_val_ = create_err(val::Err::ILLEGAL_ARG,
                                  "too many arguments: expected " + std::to_string(formals_.size())
                                  + ", got " + std::to_string(idx));
        }
    }
    
    return res_val_;
}

ValPtr robolisp::impl::val::create_lambda(SymVect formals, List body, Env *env)
{
    return ValPtr(new Lambda(std::move(formals), std::move(body), env));
}
