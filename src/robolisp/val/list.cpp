#include "list.hpp"

#include <string>
#include <utility>

#include "../apply.hpp"
#include "../env.hpp"
#include "../val.hpp"
#include "../val_vis.hpp"

using namespace robolisp::impl;
using namespace val;

static ValPtrList copy_val_list(const ValPtrList &val_list);

List::List() :
    List({}, Quot::QUOT)
{}

List::List(ValPtrList val_list, Quot quot) :
    val_list_ {std::move(val_list)},
    quot_ {quot}
{}

List::List(const List &list) :
    List(copy_val_list(list.val_list_), list.quot_)
{}

List::List(List &&list) noexcept :
    List(std::move(list.val_list_), list.quot_)
{}

List &List::operator=(const List &list)
{
    val_list_ = copy_val_list(list.val_list_);
    quot_ = list.quot_;
    return *this;
}

List &List::operator=(List &&list) noexcept
{
    val_list_ = std::move(list.val_list_);
    quot_ = list.quot_;
    return *this;
}

ValPtr List::eval(Env *env)
{
    if (quot_ == Quot::QUOT || val_list_.empty())
        return nullptr;
    
    eval_contents(env);
    if (quot_ == Quot::SEMI_QUOT)
    {
        quot_ = Quot::QUOT;
        return nullptr;
    }
    
    ValPtr op = std::move(val_list_.front());
    val_list_.pop_front();
    return apply(std::move(op), this, env);
}

ValPtr List::copy() const
{
    return ValPtr(new List(*this));
}

ValPtr List::acquire()
{
    return ValPtr(new List(std::move(*this)));
}

std::string List::to_str() const
{
    return ::to_str(val_list_);
}

void List::accept_vis(ValVis *val_vis)
{
    val_vis->visit(this);
}

const ValPtrList &List::get() const
{
    return val_list_;
}

ValPtrList List::take()
{
    return std::move(val_list_);
}

ValPtr List::unquot_eval(Env *env, bool *has_err)
{
    if (val_list_.empty())
    {
        *has_err = false;
        return nullptr;
    }
    
    eval_contents(env);
    ValPtr op = std::move(val_list_.front());
    val_list_.pop_front();
    return apply(std::move(op), this, env, has_err);
}

void List::eval_contents(Env *env)
{
    for (ValPtr &val : val_list_)
    {
        ValPtr res_val = val->eval(env);
        if (res_val)
            val = std::move(res_val);
    }
}

ValPtr robolisp::impl::val::create_list()
{
    return ValPtr(new List());
}

ValPtr robolisp::impl::val::create_list(ValPtrList val_list, Quot quot)
{
    return ValPtr(new List(std::move(val_list), quot));
}

ValPtr robolisp::impl::val::unquot_eval(List list, Env *env, bool *has_err)
{
    ValPtr res_val = list.unquot_eval(env, has_err);
    if (!res_val)
        res_val = list.acquire();
    
    return res_val;
}

std::string robolisp::impl::val::to_str(const ValPtrList &val_list)
{
    std::string res_str = "(";
    for (auto val_iter = val_list.cbegin(); val_iter != val_list.cend(); val_iter++)
    {
        if (val_iter != val_list.cbegin())
            res_str += " ";
        
        res_str += (*val_iter)->to_str();
    }
    
    return res_str + ")"; 
}

static ValPtrList copy_val_list(const ValPtrList &val_list)
{
    ValPtrList res_val_list = {};
    for (const ValPtr &val : val_list)
        res_val_list.push_back(val->copy());
    
    return res_val_list;
}
