#pragma once

#include <functional>
#include <memory>
#include <string>

namespace robolisp::impl
{

class Val;
class Env;
class ValVis;

typedef std::unique_ptr<Val> ValPtr;
typedef std::function<ValPtr()> ValCreator;

enum class Quot
{
    NO_QUOT,
    SEMI_QUOT,
    QUOT
};

class Val
{
  public :
    
    enum ValType
    {
        INVALID,
        INT,
        FLOAT,
        SYM,
        STR,
        EXPR,
        Q_EXPR,
        SQ_EXPR,
        LAMBDA,
        BUILTIN_OP,
        ERR,
        
        NUM_VAL_TYPES
    };
    
    virtual ~Val() = default;
    
    virtual ValPtr eval(Env *env);
    virtual ValPtr copy() const = 0;
    virtual ValPtr acquire();
    virtual bool to_bool() const;
    virtual std::string to_str() const = 0;
    virtual void accept_vis(ValVis *val_vis) = 0;
};

namespace val
{

class List;
class IntNum;
class FloatNum;
class Sym;
class Str;
class Lambda;
class BuiltinOp;
class Err;

ValPtr eval(Val &&val, Env *env);
ValPtr eval(ValPtr val, Env *env);

}
// namespace val

}
// namespace robolisp::impl
