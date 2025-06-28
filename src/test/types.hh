#pragma once

#include "test/unit_test.hh"
#include "test/collection.hh"


using UnitTestArray = std::vector<UnitTest>;

using TCUnitTest    = TCollection<UnitTest, 8>;
using TCModule      = TCollection<TCUnitTest, 4>;
using TCLibrary     = TCollection<TCModule, 0>;


