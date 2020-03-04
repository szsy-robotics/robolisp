#pragma once

#include "../val.hpp"

#include <string>

namespace robolisp::impl
{

namespace val
{

class FloatNum : public Val
{
  public :
    
    explicit FloatNum(float val);
    
    ValPtr copy() const override;
    std::string to_str() const override;
    void accept_vis(ValVis *val_vis) override;
    
    const float &get() const;
    
  private :
    
    float val_;
};

ValPtr create_float_num(float val);

}
// namespace val

}
// namespace robolisp::impl
