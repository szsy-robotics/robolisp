#pragma once

#include "../val.hpp"

#include <string>

namespace robolisp::impl
{

namespace val
{

class Err : public Val
{
  public :
    
    enum ErrType
    {
        GENERAL_FAILURE,
        SYNTAX_ERR,
        UNBOUND_SYM,
        ILLEGAL_ARG,
        LOGIC_ERR,
        ARITHMETIC_ERR,
        
        NUM_ERR_TYPES
    };
    
    explicit Err(ErrType err_type, std::string desc);
    
    ValPtr copy() const override;
    ValPtr acquire() override;
    std::string to_str() const override;
    void accept_vis(ValVis *val_vis) override;
    
    const ErrType &get_type() const;
    const std::string &get_desc() const;
    std::string take_desc();
    
  private :
    
    ErrType type_;
    std::string desc_;
};

ValPtr create_err(Err::ErrType type, std::string desc);
ValPtr create_err(Err::ErrType type, const std::string &desc0, const std::string &desc1);
ValPtr append_err_desc(val::Err err, const std::string &desc);

}
// namespace val

}
// namespace robolisp::impl
