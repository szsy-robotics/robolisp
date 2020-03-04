#include "str.hpp"

#include <string>
#include <unordered_map>
#include <utility>

#include "../val.hpp"
#include "../val_vis.hpp"

using namespace robolisp::impl;
using namespace val;

static std::string escape(const std::string &str);

Str::Str(std::string str) :
    str_ {std::move(str)}
{}

ValPtr Str::copy() const
{
    return ValPtr(new Str(*this));
}

ValPtr Str::acquire()
{
    return ValPtr(new Str(std::move(*this)));
}

std::string Str::to_str() const
{
    return "\"" + escape(str_) + "\"";
}

void Str::accept_vis(ValVis *val_vis)
{
    val_vis->visit(this);
}

const std::string &Str::get() const
{
    return str_;
}

std::string Str::take()
{
    return std::move(str_);
}

ValPtr robolisp::impl::val::create_str(std::string str)
{
    return ValPtr(new Str(std::move(str)));
}

static std::string escape(const std::string &str)
{
    static const std::unordered_map<char, std::string> ESCAPE_MAP =
    {
        {'\\', "\\\\"},
        {'\a', "\\a"},
        {'\b', "\\b"},
        {'\f', "\\f"},
        {'\n', "\\n"},
        {'\r', "\\r"},
        {'\t', "\\t"},
        {'\v', "\\v"},
        {'\'', "\\\'"},
        {'\"', "\\\""},
        {'\?', "\\?"}
    };
    
    std::string res_str = "";
    auto found_val_iter = ESCAPE_MAP.end();
    for (const char &char_val : str)
    {
        found_val_iter = ESCAPE_MAP.find(char_val);
        if (found_val_iter != ESCAPE_MAP.end())
            res_str += found_val_iter->second;
        else
            res_str += char_val;
    }
    
    return res_str;
}
