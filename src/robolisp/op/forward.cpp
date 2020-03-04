#include "forward.hpp"

#include <cstddef>
#include <string>
#include <utility>

#include "../op.hpp"

#include "../env.hpp"
#include "../val.hpp"

using namespace robolisp::impl;
using namespace op;

ValPtr Forward::take_res()
{
    return std::move(res_val_);
}

std::string Forward::get_desc() const
{
    return "forward";
}

std::size_t Forward::get_min_args() const
{
    return 1;
}

std::size_t Forward::get_max_args() const
{
    return 1;
}

void Forward::default_process(Val &&val)
{
    res_val_ = val.acquire();
}

OpPtr robolisp::impl::op::create_forward(Env */*env*/)
{
    return OpPtr(new Forward());
}
