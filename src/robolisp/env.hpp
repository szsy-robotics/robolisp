#pragma once

#include <string>
#include <unordered_map>

#include "val.hpp"

namespace robolisp::impl
{

typedef std::unordered_map<std::string, ValPtr> ValPtrMap;

class Env
{
  public :
    
    explicit Env(Env *parent = nullptr, ValPtrMap val_map = {});
    Env(const Env &env);
    Env(Env &&env) noexcept;
    
    Env &operator=(const Env &env);
    Env &operator=(Env &&env) noexcept;
    
    ValPtr lookup(const val::Sym &key) const;
    ValPtr lookup(const val::Sym &key, bool *has_err) const;
    ValPtr set(const val::Sym &key, ValPtr val);
    void def_local(val::Sym key, ValPtr val);
    void undef_local(const val::Sym &key);
    void def_global(val::Sym key, ValPtr val);
    void undef_global(const val::Sym &key);
    void clear();
    Env merge_copy();
    std::string to_str() const;
    
  private :
    
    ValPtrMap val_map_;
    Env *parent_;
    
    const ValPtr *find(const val::Sym &key) const;
};

}
// namespace robolisp::impl
