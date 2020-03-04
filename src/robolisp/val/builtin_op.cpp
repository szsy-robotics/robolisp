#include "builtin_op.hpp"

#include <string>
#include <utility>

#include "../env.hpp"
#include "../val.hpp"
#include "../val_vis.hpp"

using namespace robolisp::impl;
using namespace val;

BuiltinOp::BuiltinOp(OpCreator op_creator) :
    op_creator_ {std::move(op_creator)}
{}

ValPtr BuiltinOp::copy() const
{
    return ValPtr(new BuiltinOp(*this));
}

ValPtr BuiltinOp::acquire()
{
    return ValPtr(new BuiltinOp(std::move(*this)));
}

std::string BuiltinOp::to_str() const
{
    return op_creator_(nullptr)->to_str();
}

const OpCreator &BuiltinOp::get() const
{
    return op_creator_;
}

OpCreator BuiltinOp::take()
{
    return std::move(op_creator_);
}

void BuiltinOp::accept_vis(ValVis *val_vis)
{
    val_vis->visit(this);
}

ValPtr robolisp::impl::val::create_builtin_op(OpCreator op_creator)
{
    return ValPtr(new BuiltinOp(std::move(op_creator)));
}
