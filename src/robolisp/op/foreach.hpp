#pragma once

#include "../op.hpp"

#include <cstddef>
#include <string>
#include <vector>

#include "../env.hpp"
#include "../val.hpp"
#include "../val/list.hpp"
#include "../val/sym.hpp"

namespace robolisp::impl
{

namespace op
{

class Foreach : public Op
{
  public :
    
    explicit Foreach(Env *env); 
    
  private :
    
    Env *env_;
    val::Sym sym_ = val::Sym();
    val::List list_ = val::List();
    std::vector<ValPtr> args_ = {};
    
    ValPtr take_res() override;
    std::string get_desc() const override;
    std::size_t get_min_args() const override;
    std::size_t get_max_args() const override;
    
    void process(val::List &&list) override;
    void process(val::Sym &&sym) override;
    void default_process(Val &&val) override;
    
    ValPtr unquot_eval(ValPtr val, Env *env);
};

OpPtr create_foreach(Env *env);

}
// namespace op

}
// namespace robolisp::impl
