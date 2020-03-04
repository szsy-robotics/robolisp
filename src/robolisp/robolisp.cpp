#include "robolisp.hpp"

#include <iostream>
#include <string>

#include "repl.hpp"

using namespace robolisp;
using namespace impl;

void robolisp::test()
{
    Repl repl = Repl([](const std::string &str) { std::cout << str; });
    // repl.set_print_env_ena(true);
    std::string input = "";
    while (true)
    {
        std::getline(std::cin, input);
        repl.eval(input);
    }
}
