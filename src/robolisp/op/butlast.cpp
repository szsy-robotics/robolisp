#include "butlast.hpp"

#include <cstddef>
#include <string>
#include <utility>

#include "../op.hpp"

#include "../env.hpp"
#include "../val.hpp"
#include "../val/list.hpp"

using namespace robolisp::impl;
using namespace op;

ValPtr Butlast::take_res()
{
    return val::create_list(std::move(res_val_list_));
}

std::string Butlast::get_desc() const
{
    return "butlast";
}

std::size_t Butlast::get_min_args() const
{
    return 1;
}

std::size_t Butlast::get_max_args() const
{
    return 1;
}

void Butlast::process(val::List &&list)
{
    res_val_list_ = list.take();
    if (!res_val_list_.empty())
        res_val_list_.pop_back();
}

OpPtr robolisp::impl::op::create_butlast(Env */*env*/)
{
    return OpPtr(new Butlast());
}
