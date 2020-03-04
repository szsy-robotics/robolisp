#pragma once

#include "../op.hpp"

#include <cstddef>
#include <string>

#include "../env.hpp"
#include "../val.hpp"
#include "../val/list.hpp"
#include "../val/sym.hpp"

namespace robolisp::impl
{

namespace op
{

class Cond : public Op
{
  public :
    
    explicit Cond(Env *env); 
    
  private :
    
    Env *env_;
    ValPtrList res_val_list_ = {};
    bool condition_ = true;
    
    ValPtr take_res() override;
    std::string get_desc() const override;
    std::size_t get_min_args() const override;
    std::size_t get_max_args() const override;
    
    void default_process(Val &&val) override;
    
    ValPtr unquot_eval(Val &&val);
};

OpPtr create_cond(Env *env);

}
// namespace op

}
// namespace robolisp::impl
