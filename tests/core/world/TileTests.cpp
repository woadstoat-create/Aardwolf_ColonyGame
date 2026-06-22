#include <aardwolf/world/Tile.hpp>
#include <catch2/catch_test_macros.hpp>
#include <type_traits>

namespace
{
using aardwolf::world::hasFlag;
using aardwolf::world::kEmptyTileId;
using aardwolf::world::Tile;
using aardwolf::world::TileFlags;
} // namespace

TEST_CASE("Default tiles are empty", "[world][tile]")
{
    const Tile tile{};

    CHECK(tile.id == kEmptyTileId);
    CHECK(tile.isEmpty());
}

TEST_CASE("Tiles with non-zero IDs are not empty", "[world][tile]")
{
    const Tile tile{42U};

    CHECK(tile.id == 42U);
    CHECK_FALSE(tile.isEmpty());
}

TEST_CASE("Tiles compare by ID", "[world][tile]")
{
    CHECK(Tile{4U} == Tile{4U});
    CHECK_FALSE(Tile{4U} == Tile{7U});
}

TEST_CASE("Tile flags combine without occupying tile instances", "[world][tile]")
{
    const TileFlags flags =
        TileFlags::IsFloor | TileFlags::SupportsConstruction | TileFlags::BlocksVision;

    CHECK(hasFlag(flags, TileFlags::IsFloor));

    CHECK(hasFlag(flags, TileFlags::SupportsConstruction));

    CHECK(hasFlag(flags, TileFlags::BlocksVision));
    CHECK_FALSE(hasFlag(flags, TileFlags::IsLiquid));
}

TEST_CASE("Tile remains a compact trivial value type", "[world][tile]")
{
    STATIC_CHECK(sizeof(Tile) == 2U);
    STATIC_CHECK(std::is_trivially_copyable_v<Tile>);
}