#pragma once 

#include "val.hpp"

namespace robolisp::impl
{

class ValVis
{
  public :
    
    virtual ~ValVis() = default;
    
    virtual void visit(val::List *list) = 0;
    virtual void visit(val::IntNum *int_num) = 0;
    virtual void visit(val::FloatNum *float_num) = 0;
    virtual void visit(val::Sym *sym) = 0;
    virtual void visit(val::Str *str) = 0;
    virtual void visit(val::Lambda *lambda) = 0;
    virtual void visit(val::BuiltinOp *builtin_op) = 0;
    virtual void visit(val::Err *err) = 0;
};

}
// namespace robolisp::impl
