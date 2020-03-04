#pragma once

#include "../val.hpp"

#include <string>

namespace robolisp::impl
{

namespace val
{

class Str : public Val
{
  public :
    
    explicit Str(std::string str);
    
    ValPtr copy() const override;
    ValPtr acquire() override;
    std::string to_str() const override;
    void accept_vis(ValVis *val_vis) override;
    
    const std::string &get() const;
    std::string take();
    
  private :
    
    std::string str_;
};

ValPtr create_str(std::string str);

}
// namespace val

}
// namespace robolisp::impl
