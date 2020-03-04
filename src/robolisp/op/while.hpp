#pragma once

#include "../op.hpp"

#include <cstddef>
#include <string>
#include <vector>

#include "../env.hpp"
#include "../val.hpp"
#include "../val/sym.hpp"

namespace robolisp::impl
{

namespace op
{

class While : public Op
{
  public :
    
    explicit While(Env *env); 
    
  private :
    
    Env *env_;
    ValPtr condition_ = nullptr;
    std::vector<ValPtr> args_ = {};
    
    ValPtr take_res() override;
    std::string get_desc() const override;
    std::size_t get_min_args() const override;
    std::size_t get_max_args() const override;
    
    void default_process(Val &&val) override;
    
    ValPtr unquot_eval(ValPtr val);
};

OpPtr create_while(Env *env);

}
// namespace op

}
// namespace robolisp::impl
