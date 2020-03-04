#pragma once

#include "../val.hpp"

#include <string>

#include "../op.hpp"

namespace robolisp::impl
{

namespace val
{

class BuiltinOp : public Val
{
  public :
    
    explicit BuiltinOp(OpCreator op_creator);
    
    ValPtr copy() const override;
    ValPtr acquire() override;
    std::string to_str() const override;
    void accept_vis(ValVis *val_vis) override;
    
    const OpCreator &get() const;
    OpCreator take();
    
  private :
    
    OpCreator op_creator_;
};

ValPtr create_builtin_op(OpCreator op_creator);

}
// namespace val

}
// namespace robolisp::impl
