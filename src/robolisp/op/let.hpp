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

class Let : public Op
{
  public :
    
    explicit Let(Env *env); 
    
  private :
    
    Env enclosed_env_;
    ValPtrList res_val_list_ = {};
    val::Sym key_ = val::Sym();
    
    ValPtr take_res() override;
    std::string get_desc() const override;
    std::size_t get_min_args() const override;
    std::size_t get_max_args() const override;
    
    void process(val::List &&list) override;
    void process(val::Sym &&sym) override;
    void default_process(Val &&val) override;
    
    ValPtr unquot_eval(Val &&val);
};

OpPtr create_let(Env *env);

}
// namespace op

}
// namespace robolisp::impl
