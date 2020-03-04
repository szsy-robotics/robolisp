#include "first.hpp"

#include <cstddef>
#include <string>
#include <utility>

#include "../op.hpp"

#include "../env.hpp"
#include "../val.hpp"
#include "../val/list.hpp"
#include "../val/sym.hpp"

using namespace robolisp::impl;
using namespace op;

ValPtr First::take_res()
{
    return std::move(res_val_);
}

std::string First::get_desc() const
{
    return "first";
}

std::size_t First::get_min_args() const
{
    return 1;
}

std::size_t First::get_max_args() const
{
    return 1;
}

void First::process(val::List &&list)
{
    res_val_ = !list.get().empty() ? std::move(list.take().front()) : val::create_sym();
}

OpPtr robolisp::impl::op::create_first(Env */*env*/)
{
    return OpPtr(new First());
}
