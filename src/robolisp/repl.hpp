#pragma once

#include <functional>
#include <string>

#include "env.hpp"
#include "op.hpp"

namespace robolisp::impl
{

typedef std::function<void(const std::string &)> Output;

class Repl
{
  public :
    
    explicit Repl(Output output = [](const std::string &/*str*/) {});
    
    std::string eval(const std::string &str);
    void set_print_env_ena(bool val);
    
  private :
    
    Output output_;
    Env env_ = impl::Env(nullptr);
    bool print_env_ena_ = false;
    
    void def_val(const std::string &key, ValPtr val);
    void def_builtin_op(const std::string &key, const OpCreator &op_creator);
    void print_desc() const;
    void print_prompt() const;
};

}
// robolisp::impl
