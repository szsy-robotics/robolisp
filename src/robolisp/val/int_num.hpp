#pragma once

#include "../val.hpp"

#include <cinttypes>
#include <string>

namespace robolisp::impl
{

namespace val
{

class IntNum : public Val
{
  public :
    
    explicit IntNum(std::int32_t val);
    
    ValPtr copy() const override;
    std::string to_str() const override;
    void accept_vis(ValVis *val_vis) override;
    
    const std::int32_t &get() const;
    
  private :
    
    std::int32_t val_;
};

ValPtr create_int_num(std::int32_t val);

}
// namespace val

}
// namespace robolisp::impl
