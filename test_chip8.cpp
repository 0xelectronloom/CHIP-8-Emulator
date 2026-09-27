#include <catch2/catch_test_macros.hpp>
#include "CHIP_8.h"

TEST_CASE("CHIP-8 starts at address 0x200")
{
    CHIP_8 chip8;

    REQUIRE(chip8.pc == 0x200);
}