#include "parse.hpp"

#include <cctype>
#include <cinttypes>
#include <cstddef>
#include <cstdlib>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "val.hpp"
#include "val/list.hpp"
#include "val/int_num.hpp"
#include "val/float_num.hpp"
#include "val/sym.hpp"
#include "val/str.hpp"
#include "val/err.hpp"

using namespace robolisp::impl;

static const std::unordered_set<char> RESERVED_CHARS = {'(', ')', '[', ']', '{', '}', '\'', '`', '\"', '~', ';'};

static bool is_reserved_char(char val);
static bool is_leagal_sym_char(char val);
static char unescape(char val);
static std::string to_str(char val);

Parser::Parser(const std::string &str) :
    char_iter_ {str.cbegin()}
{}

ValPtr Parser::parse()
{
    ValPtr res_val = read_val('\0');
    if (!err_list_.empty())
    {
        res_val = val::create_list(std::move(err_list_), Quot::QUOT);
        err_list_ = ValPtrList();
    }
    
    return res_val;
}

ValPtr Parser::read_val(char end)
{
    while (*char_iter_ != end && !at_str_end())
    {
        switch (*char_iter_)
        {
            case (';') : // annotation
            {
                char_iter_++;
                skip_until('\n');
                continue;
            }
            
            case ('(') : // non-quoted list
            {
                char_iter_++;
                return read_list(Quot::NO_QUOT, ')');
            }
            
            case ('\'') : // quoted value
            {
                char_iter_++;
                if (*char_iter_ == '(') // quoted list
                {
                    char_iter_++;
                    return read_list(Quot::QUOT, ')');
                }
                
                if (is_leagal_sym_char(*char_iter_)) // quoted symbol (or number)
                    return read_sym(Quot::QUOT, end);
                
                reg_err(val::Err::SYNTAX_ERR, "illegal symbol ''" + copy_word(end) + "'");
                return nullptr;
            }
            
            case ('`') : // semi-quoted value
            {
                char_iter_++;
                if (*char_iter_ == '(') // semi-quoted list
                {
                    char_iter_++;
                    return read_list(Quot::SEMI_QUOT, ')');
                }
                
                if (is_leagal_sym_char(*char_iter_)) // semi-quoted symbol (or number)
                    return read_sym(Quot::SEMI_QUOT, end);
                
                reg_err(val::Err::SYNTAX_ERR, "illegal symbol '`" + copy_word(end) + "'");
                return nullptr;
            }
            
            case ('\"') : // string
            {
                char_iter_++;
                return read_str('\"');
            }
            
            default :
            {
                if (std::isspace(*char_iter_)) // whitespace
                {
                    char_iter_++;
                    skip_whitespace();
                    continue;
                }
                
                if (is_leagal_sym_char(*char_iter_)) // non-quoted symbol (or number)
                    return read_sym(Quot::NO_QUOT, end);
                
                reg_err(val::Err::SYNTAX_ERR, "illegal symbol '" + copy_word(end) + "'");
                return nullptr;
            }
        }
    }
    
    return nullptr;
}

ValPtr Parser::read_list(Quot quot, char end)
{
    ValPtrList val_list = {};
    ValPtr val = nullptr;
    while (*char_iter_ != end)
    {
        if (at_str_end()) // end of string
        {
            reg_err(val::Err::SYNTAX_ERR, "expected '" + to_str(end) + "' at end of input");
            return nullptr;
        }
        
        val = read_val(end);
        if (val)
            val_list.push_back(std::move(val));
    }
    
    skip(1);
    return val::create_list(std::move(val_list), quot);
}

ValPtr Parser::read_sym(Quot quot, char end)
{
    std::string res_str = "";
    while (!at_word_end(end))
    {
        if (!is_leagal_sym_char(*char_iter_)) // illegal symbol
        {
            reg_err(val::Err::SYNTAX_ERR, "illegal symbol '" + res_str + copy_word(end) + "'");
            return nullptr;
        }
        
        res_str += *char_iter_;
        char_iter_++;
    }
    
    return parse_sym(std::move(res_str), quot);
}

ValPtr Parser::read_str(char end)
{
    std::string res_str = "";
    while (*char_iter_ != '\"' && !at_str_end())
    {
        if (*char_iter_ == '\\') // escaped character
        {
            char_iter_++;
            char unescaped_char = unescape(*char_iter_);
            if (unescaped_char == '\0') // unknown escape sequence
            {
                unescaped_char = *char_iter_;
                reg_err(val::Err::SYNTAX_ERR,
                        "unknown escape sequence '\\" + to_str(unescaped_char)
                        + "' in string '\"" + res_str + "\\" + copy_word(end) + "\"'");
                return nullptr;
            }
            
            res_str += unescaped_char;
        }
        else // normal character
        {
            res_str += *char_iter_;
        }
        
        char_iter_++;
    }
    
    skip(1);
    return val::create_str(std::move(res_str));
}

ValPtr Parser::parse_sym(std::string str, Quot quot)
{
    ValPtr res_val = nullptr;
    char *int_end_ptr = nullptr;
    char *float_end_ptr = nullptr;
    int32_t int_val = std::strtol(str.c_str(), &int_end_ptr, 0);
    float float_val = std::strtof(str.c_str(), &float_end_ptr);
    if (int_end_ptr == str.c_str() && float_end_ptr == str.c_str()) // not number
    {
        res_val = val::create_sym(std::move(str), quot);
    }
    else if (int_end_ptr >= float_end_ptr) // int
    {
        if (*int_end_ptr == '\0') // whole string is int
            res_val = val::create_int_num(int_val);
        else
            reg_err(val::Err::SYNTAX_ERR, "symbol '" + str + "' should not start with number(s)");
    }
    else // float
    {
        if (*float_end_ptr == '\0') // whole string is float
            res_val = val::create_float_num(float_val);
        else
            reg_err(val::Err::SYNTAX_ERR, "symbol '" + str + "' should not start with number(s)");
    }
    
    return res_val;
}

std::string Parser::copy_word(char end)
{
    std::string res_str = "";
    while (!at_word_end(end))
    {
        res_str += *char_iter_;
        char_iter_++;
    }
    
    return res_str;
}

void Parser::skip(size_t len)
{
    for (size_t i = 0; i < len && !at_str_end(); i++)
        char_iter_++;
}

void Parser::skip_until(char end)
{
    while (*char_iter_ != end && !at_str_end())
        char_iter_++;
}

void Parser::skip_whitespace()
{
    while (isspace(*char_iter_))
        char_iter_++;
}

bool Parser::at_str_end() const
{
    return *char_iter_ == '\0';
}

bool Parser::at_word_end(char end) const
{
    return at_str_end() || *char_iter_ == end || std::isspace(*char_iter_) || *char_iter_ == ';';
}

void Parser::reg_err(val::Err::ErrType type, const std::string &desc)
{
    err_list_.push_back(val::create_err(type, desc, "<parser>"));
}

static bool is_reserved_char(char val)
{
    return RESERVED_CHARS.count(val) > 0;
}

static bool is_leagal_sym_char(char val)
{
    return std::isgraph(val) && !is_reserved_char(val);
}

static char unescape(char val)
{
    static const std::unordered_map<char, char> UNESCAPE_MAP =
    {
        {'\\', '\\'},
        {'a', '\a'},
        {'b', '\b'},
        {'f', '\f'},
        {'n', '\n'},
        {'r', '\r'},
        {'t', '\t'},
        {'v', '\v'},
        {'\'', '\''},
        {'\"', '\"'},
        {'?', '\?'}
    };
    
    auto found_val_iter = UNESCAPE_MAP.find(val);
    return found_val_iter != UNESCAPE_MAP.end() ? found_val_iter->second : '\0'; 
}

static std::string to_str(char val)
{
    return std::string(1, val);
}
