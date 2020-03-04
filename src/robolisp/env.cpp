#include "env.hpp"

#include <algorithm>
#include <map>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "val.hpp"
#include "val/sym.hpp"
#include "val/err.hpp"

using namespace robolisp::impl;

static ValPtrMap copy_map(const ValPtrMap &map);

Env::Env(Env *parent, ValPtrMap val_map) :
    val_map_ {std::move(val_map)},
    parent_ {parent}
{}

Env::Env(const Env &env) :
    Env(env.parent_, copy_map(env.val_map_))
{}

Env::Env(Env &&env) noexcept :
    Env(env.parent_, std::move(env.val_map_))
{}

Env &Env::operator=(const Env &env)
{
    val_map_ = copy_map(env.val_map_);
    parent_ = env.parent_;
    return *this;
}

Env &Env::operator=(Env &&env) noexcept
{
    val_map_ = std::move(env.val_map_);
    parent_ = env.parent_;
    return *this;
}

ValPtr Env::lookup(const val::Sym &key) const
{
    const ValPtr *found_val = find(key);
    if (!found_val)
        return create_err(val::Err::UNBOUND_SYM, "failed to find symbol '" + key.to_str() + "' in '" + to_str() + "'");
    
    return (*found_val)->copy();
}

ValPtr Env::lookup(const val::Sym &key, bool *has_err) const
{
    const ValPtr *found_val = find(key);
    if (!found_val)
    {
        *has_err = true;
        return create_err(val::Err::UNBOUND_SYM, "failed to find symbol '" + key.to_str() + "' in '" + to_str() + "'");
    }
    
    *has_err = false;
    return (*found_val)->copy();
}

ValPtr Env::set(const val::Sym &key, ValPtr val)
{
    const ValPtr *found_val = find(key);
    if (!found_val)
        return create_err(val::Err::UNBOUND_SYM, "failed to find symbol '" + key.to_str() + "' in '" + to_str() + "'");
    
    *const_cast<ValPtr *>(found_val) = std::move(val);
    return val::create_sym();
}

void Env::def_local(val::Sym key, ValPtr val)
{
    undef_local(key);
    val_map_.emplace(key.take(), std::move(val));
}

void Env::undef_local(const val::Sym &key)
{
    val_map_.erase(key.get());
}

void Env::def_global(val::Sym key, ValPtr val)
{
    if (!parent_)
        def_local(std::move(key), std::move(val));
    else
        parent_->def_global(std::move(key), std::move(val));
}

void Env::undef_global(const val::Sym &key)
{
    if (!parent_)
        undef_local(key);
    else
        parent_->undef_global(key);
}

void Env::clear()
{
    val_map_ = ValPtrMap();
}

Env Env::merge_copy()
{
    if (!parent_)
        return Env(this);
    
    if (!parent_->parent_)
        return *this;
    
    Env res_env = parent_->merge_copy();
    for (const ValPtrMap::value_type &entry : val_map_)
    {
        res_env.val_map_.erase(entry.first);
        res_env.val_map_.emplace(entry.first, entry.second->copy());
    }
    
    return res_env;
}

std::string Env::to_str() const
{
    std::map<std::string, std::string> entries = {};
    std::transform(val_map_.begin(),
                   val_map_.end(),
                   std::inserter(entries, entries.begin()), 
                   [](const ValPtrMap::value_type &entry) { return std::make_pair(entry.first, entry.second->to_str()); });
    
    std::string res_str = "<environment (";
    for (auto entry_iter = entries.cbegin(); entry_iter != entries.cend(); entry_iter++)
    {
        if (entry_iter != entries.cbegin())
            res_str += " ";
        
        res_str += "{" + entry_iter->first + " : " + entry_iter->second + "}";
    }
    
    res_str += ")";
    if (parent_)
        res_str += " -> " + parent_->to_str();
    
    return res_str + ">";
}

const ValPtr *Env::find(const val::Sym &key) const
{
    auto found_val_ptr_iter = val_map_.find(key.get());
    if (found_val_ptr_iter == val_map_.end())
    {
        if (!parent_)
            return nullptr;
        
        return parent_->find(key);
    }
    
    return &(found_val_ptr_iter->second);
}

static ValPtrMap copy_map(const ValPtrMap &map)
{
    ValPtrMap res_map = {};
    for (const ValPtrMap::value_type &entry : map)
        res_map.emplace(entry.first, entry.second->copy());
    
    return res_map;
}
