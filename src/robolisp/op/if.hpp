#pragma once

#include "../op.hpp"

#include <cstddef>
#include <string>

#include "../env.hpp"
#include "../val.hpp"
#include "../val/sym.hpp"

namespace robolisp::impl
{

namespace op
{

class If : public Op
{
  public :
    
    explicit If(Env *env); 
    
  private :
    
    Env *env_;
    ValPtr res_val_ = val::create_sym();
    bool condition_ = true;
    
    ValPtr take_res() override;
    std::string get_desc() const override;
    std::size_t get_min_args() const override;
    std::size_t get_max_args() const override;
    
    void default_process(Val &&val) override;
    
    ValPtr unquot_eval(Val &&val);
};

OpPtr create_if(Env *env);

}
// namespace op

}
// namespace robolisp::impl
