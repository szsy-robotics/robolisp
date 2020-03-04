#pragma once

#include "../val.hpp"

#include <string>

#include "../env.hpp"

namespace robolisp::impl
{

namespace val
{

class Sym : public Val
{
  public :
    
    Sym();
    explicit Sym(bool val);
    explicit Sym(std::string str, Quot quot = Quot::NO_QUOT);
    explicit Sym(const char *str, Quot quot = Quot::NO_QUOT);
    
    ValPtr eval(Env *env) override;
    ValPtr copy() const override;
    ValPtr acquire() override;
    bool to_bool() const override;
    std::string to_str() const override;
    void accept_vis(ValVis *val_vis) override;
    
    const std::string &get() const;
    std::string take();
    ValPtr unquot_eval(Env *env, bool *has_err);
    bool is_reserved() const;
    bool is_nil() const;
    bool is_bool() const;
    
  private :
    
    std::string str_;
    Quot quot_;
};

ValPtr create_sym();
ValPtr create_sym(bool val);
ValPtr create_sym(std::string str, Quot quot = Quot::NO_QUOT);
ValPtr create_sym(const char *str, Quot quot = Quot::NO_QUOT);
ValPtr unquot_eval(Sym sym, Env *env, bool *has_err);

}
// namespace val

}
// namespace robolisp::impl
