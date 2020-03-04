#pragma once

#include "../op.hpp"

#include <cstddef>
#include <string>

#include "../env.hpp"
#include "../val.hpp"
#include "../val/list.hpp"
#include "../val/int_num.hpp"
#include "../val/float_num.hpp"

namespace robolisp::impl
{

namespace op
{

class Log : public Op
{
  private :
    
    ValPtr res_val_ = nullptr;
    
    ValPtr take_res() override;
    std::string get_desc() const override;
    std::size_t get_min_args() const override;
    std::size_t get_max_args() const override;
    
    void process(val::List &&list) override;
    void process(val::IntNum &&int_num) override;
    void process(val::FloatNum &&float_num) override;
};

OpPtr create_log(Env *env);

}
// namespace op

}
// namespace robolisp::impl
