#include "err.hpp"

#include <array>
#include <string>
#include <utility>

#include "../val.hpp"
#include "../val_vis.hpp"

using namespace robolisp::impl;
using namespace val;

static constexpr const char *const ERR_DESC_CONNECTOR_STR = " -> ";

Err::Err(Err::ErrType err_type, std::string desc) :
    type_ {err_type},
    desc_ {std::move(desc)}
{}

ValPtr Err::copy() const
{
    return ValPtr(new Err(*this));
}

ValPtr Err::acquire()
{
    return ValPtr(new Err(std::move(*this)));
}

std::string Err::to_str() const
{
    static const std::array<std::string, Err::NUM_ERR_TYPES> ERR_DESCS =
    {
        "general failure: ",
        "syntax error: ",
        "unbound symbol: ",
        "illegal argument: ",
        "logic error: ",
        "arithmetic error: "
    };
    
    return "<error: " + ERR_DESCS[type_] + desc_ + ">";
}

void Err::accept_vis(ValVis *val_vis)
{
    val_vis->visit(this);
}

const Err::ErrType &Err::get_type() const
{
    return type_;
}

const std::string &Err::get_desc() const
{
    return desc_;
}

std::string Err::take_desc()
{
    return std::move(desc_);
}

ValPtr robolisp::impl::val::create_err(Err::ErrType type, std::string desc)
{
    return ValPtr(new Err(type, std::move(desc)));
}

ValPtr robolisp::impl::val::create_err(Err::ErrType type, const std::string &desc0, const std::string &desc1)
{
    return create_err(type, desc0 + ERR_DESC_CONNECTOR_STR + desc1);
}

ValPtr robolisp::impl::val::append_err_desc(val::Err err, const std::string &desc)
{
    return create_err(err.get_type(), err.take_desc() + ERR_DESC_CONNECTOR_STR + desc);
}
