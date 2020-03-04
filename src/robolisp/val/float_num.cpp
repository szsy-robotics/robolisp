#include "float_num.hpp"

#include <string>

#include "../val.hpp"
#include "../val_vis.hpp"

using namespace robolisp::impl;
using namespace val;

FloatNum::FloatNum(float val) :
    val_ {val}
{}

ValPtr FloatNum::copy() const
{
    return ValPtr(new FloatNum(*this));
}

std::string FloatNum::to_str() const
{
    return std::to_string(val_);
}

void FloatNum::accept_vis(ValVis *val_vis)
{
    val_vis->visit(this);
}

const float &FloatNum::get() const
{
    return val_;
}

ValPtr robolisp::impl::val::create_float_num(float val)
{
    return ValPtr(new FloatNum(val));
}
