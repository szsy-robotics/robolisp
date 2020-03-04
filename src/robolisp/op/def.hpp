#pragma once

#include "../op.hpp"

#include <cstddef>
#include <string>
#include <variant>

#include "../env.hpp"
#include "../val.hpp"
#include "../val/list.hpp"
#include "../val/sym.hpp"

namespace robolisp::impl
{

namespace op
{

class Def : public Op
{
  public :
    
    explicit Def(Env *env); 
    
  private :
    
    Env *env_;
    std::variant<std::monostate, val::List, val::Sym> key_ = std::monostate();
    
    ValPtr take_res() override;
    std::string get_desc() const override;
    std::size_t get_min_args() const override;
    std::size_t get_max_args() const override;
    
    void process(val::List &&list) override;
    void process(val::Sym &&sym) override;
    void default_process(Val &&val) override;
};

OpPtr create_def(Env *env);

}
// namespace op

}
// namespace robolisp::impl
