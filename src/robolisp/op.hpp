#pragma once

#include "val_vis.hpp"

#include <cstddef>
#include <functional>
#include <memory>
#include <string>

#include "env.hpp"
#include "val.hpp"
#include "val/list.hpp"
#include "val/err.hpp"

namespace robolisp::impl
{

class Op;

typedef std::unique_ptr<Op> OpPtr;
typedef std::function<OpPtr(Env *)> OpCreator;

class Op : public ValVis
{
  public :
    
    void visit(val::List *list) final;
    void visit(val::IntNum *int_num) final;
    void visit(val::FloatNum *float_num) final;
    void visit(val::Sym *sym) final;
    void visit(val::Str *str) final;
    void visit(val::Lambda *lambda) final;
    void visit(val::BuiltinOp *builtin_op) final;
    void visit(val::Err *err) final;
    
    ValPtr eval_res();
    ValPtr eval_res(bool *has_err);
    void call(Val &&arg);
    void call(ValPtrList &&args);
    std::string to_str() const;
    
  protected :
    
    static constexpr std::size_t ARG_SIZE_INF = -1;
    
    std::size_t get_arg_cnt() const;
    void reg_err(ValPtr err);
    void reg_err(val::Err::ErrType type, const std::string &desc);
    void reg_illegal_arg(const Val &val);
    
  private :
    
    std::size_t arg_cnt_ = 0;
    bool has_err_ = false;
    ValPtr err_ = nullptr;
    
    bool chk_res_validity();
    bool chk_arg_validity();
    bool is_below_min_args() const;
    bool is_above_max_args() const;
    
    virtual ValPtr take_res() = 0;
    virtual std::string get_desc() const = 0;
    virtual std::size_t get_min_args() const = 0;
    virtual std::size_t get_max_args() const = 0;
    
    virtual void process(val::List &&list);
    virtual void process(val::IntNum &&int_num);
    virtual void process(val::FloatNum &&float_num);
    virtual void process(val::Sym &&sym);
    virtual void process(val::Str &&str);
    virtual void process(val::Lambda &&lambda);
    virtual void process(val::BuiltinOp &&builtin_op);
    virtual void process(val::Err &&err);
    virtual void default_process(Val &&val);
};

namespace op
{
    
ValPtr call(Op &&op, Val &&val);
ValPtr call(Op &&op, Val &&val0, Val &&val1);
ValPtr call(Op &&op, ValPtrList &&val_list);
ValPtr call(Op &&op, Val &&val, bool *has_err);
ValPtr call(Op &&op, Val &&val0, Val &&val1, bool *has_err);
ValPtr call(Op &&op, ValPtrList &&val_list, bool *has_err);
bool call(const OpCreator &op_creator,
          const ValPtrList &val_list_0,
          const ValPtrList &val_list_1,
          Env *env,
          const std::function<void(ValPtr, bool)> &callback);
    
}
// namespace op

}
// namespace robolisp::impl
