#pragma once

#include "../op.hpp"

#include <cstddef>
#include <string>

#include "../env.hpp"
#include "../val.hpp"
#include "../val/list.hpp"
#include "../val/int_num.hpp"

namespace robolisp::impl
{

namespace op
{

class Nth : public Op
{
  private :
    
    ValPtr res_val_ = nullptr;
    size_t idx_ = 0;
    
    ValPtr take_res() override;
    std::string get_desc() const override;
    std::size_t get_min_args() const override;
    std::size_t get_max_args() const override;
    
    void process(val::List &&list) override;
    void process(val::IntNum &&int_num) override;
};

OpPtr create_nth(Env *env);

}
// namespace op

}
// namespace robolisp::impl
