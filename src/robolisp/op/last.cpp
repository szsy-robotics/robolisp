#include "last.hpp"

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

ValPtr Last::take_res()
{
    return std::move(res_val_);
}

std::string Last::get_desc() const
{
    return "last";
}

std::size_t Last::get_min_args() const
{
    return 1;
}

std::size_t Last::get_max_args() const
{
    return 1;
}

void Last::process(val::List &&list)
{
    res_val_ = !list.get().empty() ? std::move(list.take().back()) : val::create_sym();
}

OpPtr robolisp::impl::op::create_last(Env */*env*/)
{
    return OpPtr(new Last());
}
