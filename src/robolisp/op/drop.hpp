#pragma once

#include "../op.hpp"

#include <cinttypes>
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

class Drop : public Op
{
  private :
    
    ValPtrList res_val_list_ = {};
    int32_t len_ = 0;
    
    ValPtr take_res() override;
    std::string get_desc() const override;
    std::size_t get_min_args() const override;
    std::size_t get_max_args() const override;
    
    void process(val::List &&list) override;
    void process(val::IntNum &&int_num) override;
};

OpPtr create_drop(Env *env);

}
// namespace op

}
// namespace robolisp::impl
