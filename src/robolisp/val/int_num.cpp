#include "int_num.hpp"

#include <cinttypes>
#include <string>

#include "../val.hpp"
#include "../val_vis.hpp"

using namespace robolisp::impl;
using namespace val;

IntNum::IntNum(int val) :
    val_ {val}
{}

ValPtr IntNum::copy() const
{
    return ValPtr(new IntNum(*this));
}

std::string IntNum::to_str() const
{
    return std::to_string(val_);
}

const std::int32_t &IntNum::get() const
{
    return val_;
}

void IntNum::accept_vis(ValVis *val_vis)
{
    val_vis->visit(this);
}

ValPtr robolisp::impl::val::create_int_num(std::int32_t val)
{
    return ValPtr(new IntNum(val));
}
