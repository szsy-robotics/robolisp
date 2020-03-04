#pragma once

#include "../val.hpp"

#include <list>
#include <string>

#include "../env.hpp"

namespace robolisp::impl
{

typedef std::list<ValPtr> ValPtrList;

namespace val
{

class List : public Val
{
  public :
    
    List();
    explicit List(ValPtrList val_list, Quot quot = Quot::NO_QUOT);
    List(const List &list);
    List(List &&list) noexcept;
    
    List &operator=(const List &list);
    List &operator=(List &&list) noexcept;
    
    ValPtr eval(Env *env) override;
    ValPtr copy() const override;
    ValPtr acquire() override;
    std::string to_str() const override;
    void accept_vis(ValVis *val_vis) override;
    
    const ValPtrList &get() const;
    ValPtrList take();
    ValPtr unquot_eval(Env *env, bool *has_err);
    
  private :
    
    ValPtrList val_list_;
    Quot quot_;
    
    void eval_contents(Env *env);
};

ValPtr create_list();
ValPtr create_list(ValPtrList val_list, Quot quot = Quot::NO_QUOT);
ValPtr unquot_eval(List list, Env *env, bool *has_err);
std::string to_str(const ValPtrList &val_list);

}
// namespace val

}
// namespace robolisp::impl
