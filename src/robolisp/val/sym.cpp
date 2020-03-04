#include "sym.hpp"

#include <string>
#include <unordered_set>
#include <utility>

#include "../env.hpp"
#include "../val.hpp"
#include "../val_vis.hpp"

using namespace robolisp::impl;
using namespace val;

static constexpr const char *const SYM_NIL = "nil";
static constexpr const char *const SYM_TRUE = "true";
static constexpr const char *const SYM_FALSE = "false";
static const std::unordered_set<std::string> RESERVED_SYMS = {SYM_NIL, SYM_TRUE, SYM_FALSE};

Sym::Sym() :
    Sym(SYM_NIL, Quot::QUOT)
{}

Sym::Sym(bool val) :
    Sym(val ? SYM_TRUE : SYM_FALSE, Quot::QUOT)
{}

Sym::Sym(std::string str, Quot quot) :
    str_ {std::move(str)},
    quot_ {quot}
{}

Sym::Sym(const char *str, Quot quot) :
    Sym(std::string(str), quot)
{}

ValPtr Sym::eval(Env *env)
{
    if (quot_ != Quot::NO_QUOT || is_reserved())
        return nullptr;
    
    return env->lookup(*this);
}

ValPtr Sym::copy() const
{
    return ValPtr(new Sym(*this));
}

ValPtr Sym::acquire()
{
    return ValPtr(new Sym(std::move(*this)));
}

bool Sym::to_bool() const
{
    return str_ != SYM_NIL && str_ != SYM_FALSE;
}

std::string Sym::to_str() const
{
    return str_;
}

void Sym::accept_vis(ValVis *val_vis)
{
    val_vis->visit(this);
}

const std::string &Sym::get() const
{
    return str_;
}

std::string Sym::take()
{
    return std::move(str_);
}

ValPtr Sym::unquot_eval(Env *env, bool *has_err)
{
    if (is_reserved())
    {
        *has_err = false;
        return nullptr;
    }
    
    return env->lookup(*this, has_err);
}

bool Sym::is_reserved() const
{
    return RESERVED_SYMS.count(str_) != 0;
}

bool Sym::is_nil() const
{
    return str_ == SYM_NIL;
}

bool Sym::is_bool() const
{
    return str_ == SYM_TRUE || str_ == SYM_FALSE;
}

ValPtr robolisp::impl::val::create_sym()
{
    return ValPtr(new Sym());
}

ValPtr robolisp::impl::val::create_sym(bool val)
{
    return ValPtr(new Sym(val));
}

ValPtr robolisp::impl::val::create_sym(std::string str, Quot quot)
{
    return ValPtr(new Sym(std::move(str), quot));
}

ValPtr robolisp::impl::val::create_sym(const char *str, Quot quot)
{
    return ValPtr(new Sym(str, quot));
}

ValPtr robolisp::impl::val::unquot_eval(Sym sym, Env *env, bool *has_err)
{
    ValPtr res_val = sym.unquot_eval(env, has_err);
    if (!res_val)
        res_val = sym.acquire();
    
    return res_val;
}
