#pragma once

#include "test/unit_test.hh"
#include "test/collection.hh"


constexpr unsigned char TEST_INDENT = 4;
constexpr unsigned char UNIT_TEST_INDENT = 12;

using UnitTestArray = std::vector<UnitTest>;

using TCUnitTest    = TCollection<UnitTest, 8>;
using TCModule      = TCollection<TCUnitTest, 4>;
using TCLibrary     = TCollection<TCModule, 0>;


