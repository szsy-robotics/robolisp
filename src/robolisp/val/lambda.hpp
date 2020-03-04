#pragma once

#include "../val.hpp"

#include <string>
#include <vector>

#include "../env.hpp"
#include "../val/list.hpp"
#include "../val/sym.hpp"

namespace robolisp::impl
{

namespace val
{

typedef std::vector<Sym> SymVect;

class Lambda : public Val
{
  public :
    
    explicit Lambda(SymVect formals, List body, Env *env);
    
    ValPtr copy() const override;
    ValPtr acquire() override;
    std::string to_str() const override;
    void accept_vis(ValVis *val_vis) override;
    
    const SymVect &get_formals() const;
    const List &get_body() const;
    SymVect take_formals();
    List take_body();
    ValPtr eval_lambda(List args, Env *context_env, bool *has_err);
    
  private :
    
    SymVect formals_;
    List body_;
    Env env_;
};

ValPtr create_lambda(SymVect formals, List body, Env *env);

}
// namespace val

}
// namespace robolisp::impl
