#pragma once

#include "../op.hpp"

#include <cstddef>
#include <string>

#include "../env.hpp"
#include "../val.hpp"
#include "../val/list.hpp"
#include "../val/lambda.hpp"
#include "../val/builtin_op.hpp"

namespace robolisp::impl
{

namespace op
{

class Sort : public Op
{
  public :
    
    explicit Sort(Env *env);
    
  private :
    
    Env *env_;
    ValPtrList res_val_list_ = {};
    ValPtr op_ = nullptr;
    
    ValPtr take_res() override;
    std::string get_desc() const override;
    std::size_t get_min_args() const override;
    std::size_t get_max_args() const override;
    
    void process(val::List &&list) override;
    void process(val::Lambda &&lambda) override;
    void process(val::BuiltinOp &&builtin_op) override;
};

OpPtr create_sort(Env *env);

}
// namespace op

}
// namespace robolisp::impl
