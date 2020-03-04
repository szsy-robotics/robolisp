#include "repl.hpp"

#include <string>
#include <utility>

#include "env.hpp"
#include "parse.hpp"
#include "val.hpp"
#include "val/sym.hpp"
#include "val/builtin_op.hpp"
#include "op.hpp"
#include "op/lambda.hpp"
#include "op/defun.hpp"
#include "op/def.hpp"
#include "op/undef.hpp"
#include "op/let.hpp"
#include "op/set.hpp"
#include "op/forward.hpp"
#include "op/eval.hpp"
#include "op/if.hpp"
#include "op/cond.hpp"
#include "op/while.hpp"
#include "op/foreach.hpp"
#include "op/cons.hpp"
#include "op/list.hpp"
#include "op/first.hpp"
#include "op/rest.hpp"
#include "op/last.hpp"
#include "op/butlast.hpp"
#include "op/nth.hpp"
#include "op/take.hpp"
#include "op/drop.hpp"
#include "op/member.hpp"
#include "op/reverse.hpp"
#include "op/sort.hpp"
#include "op/append.hpp"
#include "op/logical_not.hpp"
#include "op/logical_and.hpp"
#include "op/logical_or.hpp"
#include "op/add.hpp"
#include "op/sub.hpp"
#include "op/mult.hpp"
#include "op/div.hpp"
#include "op/mod.hpp"
#include "op/exp.hpp"
#include "op/log.hpp"
#include "op/pow.hpp"
#include "op/equal.hpp"
#include "op/less.hpp"
#include "op/greater.hpp"
#include "op/less_equal.hpp"
#include "op/greater_equal.hpp"

using namespace robolisp::impl;

Repl::Repl(Output output) :
    output_ {std::move(output)}
{
    print_desc();
    
    def_builtin_op("lambda", op::create_lambda);
    def_builtin_op("def", op::create_def);
    def_builtin_op("defun", op::create_defun);
    def_builtin_op("undef", op::create_undef);
    def_builtin_op("let", op::create_let);
    def_builtin_op("set", op::create_set);
    def_builtin_op("forward", op::create_forward);
    def_builtin_op("eval", op::create_eval);
    def_builtin_op("if", op::create_if);
    def_builtin_op("cond", op::create_cond);
    def_builtin_op("while", op::create_while);
    def_builtin_op("foreach", op::create_foreach);
    def_builtin_op("cons", op::create_cons);
    def_builtin_op("list", op::create_list);
    def_builtin_op("first", op::create_first);
    def_builtin_op("rest", op::create_rest);
    def_builtin_op("last", op::create_last);
    def_builtin_op("butlast", op::create_butlast);
    def_builtin_op("nth", op::create_nth);
    def_builtin_op("take", op::create_take);
    def_builtin_op("drop", op::create_drop);
    def_builtin_op("member", op::create_member);
    def_builtin_op("reverse", op::create_reverse);
    def_builtin_op("sort", op::create_sort);
    def_builtin_op("append", op::create_append);
    def_builtin_op("!", op::create_logical_not);
    def_builtin_op("&&", op::create_logical_and);
    def_builtin_op("||", op::create_logical_or);
    def_builtin_op("+", op::create_add);
    def_builtin_op("-", op::create_sub);
    def_builtin_op("*", op::create_mult);
    def_builtin_op("/", op::create_div);
    def_builtin_op("%", op::create_mod);
    def_builtin_op("exp", op::create_exp);
    def_builtin_op("log", op::create_log);
    def_builtin_op("pow", op::create_pow);
    def_builtin_op("=", op::create_equal);
    def_builtin_op("<", op::create_less);
    def_builtin_op(">", op::create_greater);
    def_builtin_op("<=", op::create_less_equal);
    def_builtin_op(">=", op::create_greater_equal);
    
    def_val("dynamic-scoping-ena", val::create_sym(false));
    
    print_prompt();
}

std::string Repl::eval(const std::string &str)
{
    std::string res_str = "";
    Parser parser = Parser(str);
    ValPtr val = parser.parse();
    while (val)
    {
        std::string str = val::eval(std::move(val), &env_)->to_str() + "\n";
        res_str += str;
        output_(str);
        val = parser.parse();
    }
    
    print_prompt();
    return res_str;
}

void Repl::set_print_env_ena(bool val)
{
    print_env_ena_ = val;
}

void Repl::def_val(const std::string &key, ValPtr val)
{
    env_.def_global(val::Sym(key), std::move(val));
}

void Repl::def_builtin_op(const std::string &key, const OpCreator &op_creator)
{
    def_val(key, val::create_builtin_op(op_creator));
}

void Repl::print_desc() const
{
    output_("RoboLisp REPL\n");
}

void Repl::print_prompt() const
{
    if (print_env_ena_)
        output_("Environment: " + env_.to_str() + "\n");
    
    output_("> ");
}
