#pragma once

#include <string>

#include "val.hpp"
#include "val/list.hpp"
#include "val/err.hpp"

namespace robolisp::impl
{

class Parser
{
  public :
    
    explicit Parser(const std::string &str);
    
    ValPtr parse();
    
  private :
    
    std::string::const_iterator char_iter_;
    ValPtrList err_list_ = {};
    
    ValPtr read_val(char end);
    ValPtr read_list(Quot quot, char end);
    ValPtr read_sym(Quot quot, char end);
    ValPtr read_str(char end);
    ValPtr parse_sym(std::string str, Quot quot);
    std::string copy_word(char end);
    void skip(size_t len);
    void skip_until(char end);
    void skip_whitespace();
    bool at_str_end() const;
    bool at_word_end(char end) const;
    void reg_err(val::Err::ErrType type, const std::string &desc);
};

}
// namespace robolisp::impl
