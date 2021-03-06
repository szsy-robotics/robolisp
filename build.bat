@ECHO OFF
CLS

set files=./src/robolisp/robolisp.cpp^
          ./src/robolisp/apply.cpp^
          ./src/robolisp/env.cpp^
          ./src/robolisp/parse.cpp^
          ./src/robolisp/repl.cpp^
          ./src/robolisp/val.cpp^
          ./src/robolisp/val/list.cpp^
          ./src/robolisp/val/int_num.cpp^
          ./src/robolisp/val/float_num.cpp^
          ./src/robolisp/val/sym.cpp^
          ./src/robolisp/val/str.cpp^
          ./src/robolisp/val/builtin_op.cpp^
          ./src/robolisp/val/lambda.cpp^
          ./src/robolisp/val/err.cpp^
          ./src/robolisp/val_vis.cpp^
          ./src/robolisp/op.cpp^
          ./src/robolisp/op/lambda.cpp^
          ./src/robolisp/op/def.cpp^
          ./src/robolisp/op/defun.cpp^
          ./src/robolisp/op/undef.cpp^
          ./src/robolisp/op/let.cpp^
          ./src/robolisp/op/set.cpp^
          ./src/robolisp/op/forward.cpp^
          ./src/robolisp/op/eval.cpp^
          ./src/robolisp/op/if.cpp^
          ./src/robolisp/op/cond.cpp^
          ./src/robolisp/op/while.cpp^
          ./src/robolisp/op/foreach.cpp^
          ./src/robolisp/op/cons.cpp^
          ./src/robolisp/op/list.cpp^
          ./src/robolisp/op/first.cpp^
          ./src/robolisp/op/rest.cpp^
          ./src/robolisp/op/last.cpp^
          ./src/robolisp/op/butlast.cpp^
          ./src/robolisp/op/nth.cpp^
          ./src/robolisp/op/take.cpp^
          ./src/robolisp/op/drop.cpp^
          ./src/robolisp/op/member.cpp^
          ./src/robolisp/op/reverse.cpp^
          ./src/robolisp/op/sort.cpp^
          ./src/robolisp/op/append.cpp^
          ./src/robolisp/op/logical_not.cpp^
          ./src/robolisp/op/logical_and.cpp^
          ./src/robolisp/op/logical_or.cpp^
          ./src/robolisp/op/add.cpp^
          ./src/robolisp/op/sub.cpp^
          ./src/robolisp/op/mult.cpp^
          ./src/robolisp/op/div.cpp^
          ./src/robolisp/op/mod.cpp^
          ./src/robolisp/op/exp.cpp^
          ./src/robolisp/op/log.cpp^
          ./src/robolisp/op/pow.cpp^
          ./src/robolisp/op/equal.cpp^
          ./src/robolisp/op/less.cpp^
          ./src/robolisp/op/greater.cpp^
          ./src/robolisp/op/less_equal.cpp^
          ./src/robolisp/op/greater_equal.cpp^
          ./src/main.cpp

g++ %files% -o rl.exe -std=gnu++17 -O3 -g0 -flto -fdata-sections -ffunction-sections -Wl,--gc-sections -fpermissive -fno-threadsafe-statics -fno-rtti -fno-exceptions -Wall -Wextra
