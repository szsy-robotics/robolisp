#pragma once

#include <initializer_list>

#include "env.hpp"
#include "val.hpp"
#include "op.hpp"

namespace robolisp::impl
{

ValPtr apply(ValPtr func, val::List *args, Env *env);
ValPtr apply(ValPtr func, val::List *args, Env *env, bool *has_err);

}
// namespace robolisp::impl
