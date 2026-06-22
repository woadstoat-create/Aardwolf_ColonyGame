#include <aardwolf/core/BuildInfo.hpp>
#include <catch2/catch_test_macros.hpp>
#include <string_view>

TEST_CASE("Project exposes its name", "[build-info]")
{
    CHECK(aardwolf::core::projectName() == std::string_view{"ColonySim"});
}

TEST_CASE("Project exposes its semantic version", "[build-info]")
{
    const aardwolf::core::Version expected{0u, 1u, 0u};

    CHECK(aardwolf::core::version() == expected);
}