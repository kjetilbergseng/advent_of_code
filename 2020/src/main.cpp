#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest/doctest.h>
#include <fmt/core.h>
#include <fmt/color.h>
#include "day1.h"

int main(int argc, char** argv) {
    //Unit tests setup
    doctest::Context ctx;
    ctx.setOption("abort-after", 5);
    ctx.applyCommandLine(argc, argv);
    ctx.setOption("no-breaks", true);
    int res = ctx.run();
    if (ctx.shouldExit()){
        return res;
    }
}
