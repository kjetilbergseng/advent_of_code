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
    if (ctx.shouldExit())
        return res;
   
	try {
        return 0;
    }
    catch (const std::exception &e) {
        fmt::print(fg(fmt::color::crimson), "Error: {}\n", e.what());
    }
    catch (int i) {
        fmt::print(fg(fmt::color::crimson), "Error code:  {}\n", i);
    }
    catch (...) {
        fmt::print(fg(fmt::color::crimson), "Error: unknown error\n");
    }
}