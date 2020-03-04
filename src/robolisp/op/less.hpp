#pragma once

#include "../op.hpp"

#include <cstddef>
#include <string>
#include <variant>

#include "../env.hpp"
#include "../val.hpp"
#include "../val/list.hpp"
#include "../val/int_num.hpp"
#include "../val/float_num.hpp"
#include "../val/str.hpp"

namespace robolisp::impl
{

namespace op
{

class Less : public Op
{
  private :
    
    bool res_ = true;
    std::variant<std::monostate, val::List, val::IntNum, val::FloatNum, val::Str> ref_val_ = std::monostate();
    
    ValPtr take_res() override;
    std::string get_desc() const override;
    std::size_t get_min_args() const override;
    std::size_t get_max_args() const override;
    
    void process(val::List &&list) override;
    void process(val::IntNum &&int_num) override;
    void process(val::FloatNum &&float_num) override;
    void process(val::Str &&str) override;
};

OpPtr create_less(Env *env);

}
// namespace op

}
// namespace robolisp::impl
