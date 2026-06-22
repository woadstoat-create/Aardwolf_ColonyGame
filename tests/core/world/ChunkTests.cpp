#include <aardwolf/world/Chunk.hpp>
#include <catch2/catch_test_macros.hpp>

namespace
{
using aardwolf::world::Chunk;
using aardwolf::world::kTilesPerChunk;
using aardwolf::world::LocalTileCoordinate;
using aardwolf::world::Tile;
} // namespace

TEST_CASE("New chunks contain only empty tiles", "[world][chunk]")
{
    const Chunk chunk{};

    CHECK(chunk.isEmpty());
    CHECK(chunk.nonEmptyTileCount() == 0U);
    CHECK(chunk.revision() == 0U);
    CHECK(chunk.tiles().size() == kTilesPerChunk);

    CHECK(chunk.tileAt(LocalTileCoordinate{0U, 0U}).isEmpty());

    CHECK(chunk.tileAt(LocalTileCoordinate{31U, 31U}).isEmpty());
}

TEST_CASE("Setting a tile updates chunk state", "[world][chunk]")
{
    Chunk chunk{};
    const LocalTileCoordinate position{7U, 12U};

    CHECK(chunk.setTile(position, Tile{4U}));

    CHECK(chunk.tileAt(position) == Tile{4U});
    CHECK_FALSE(chunk.isEmpty());
    CHECK(chunk.nonEmptyTileCount() == 1U);
    CHECK(chunk.revision() == 1U);
}

TEST_CASE("Writing the same tile does not change the revision", "[world][chunk]")
{
    Chunk chunk{};
    const LocalTileCoordinate position{7U, 12U};

    CHECK(chunk.setTile(position, Tile{4U}));
    CHECK_FALSE(chunk.setTile(position, Tile{4U}));

    CHECK(chunk.nonEmptyTileCount() == 1U);
    CHECK(chunk.revision() == 1U);
}

TEST_CASE("Replacing one non-empty tile preserves the count", "[world][chunk]")
{
    Chunk chunk{};
    const LocalTileCoordinate position{2U, 3U};

    CHECK(chunk.setTile(position, Tile{1U}));
    CHECK(chunk.setTile(position, Tile{2U}));

    CHECK(chunk.tileAt(position) == Tile{2U});
    CHECK(chunk.nonEmptyTileCount() == 1U);
    CHECK(chunk.revision() == 2U);
}

TEST_CASE("Clearing a tile updates chunk state", "[world][chunk]")
{
    Chunk chunk{};
    const LocalTileCoordinate position{2U, 3U};

    CHECK(chunk.setTile(position, Tile{1U}));
    CHECK(chunk.setTile(position, Tile{}));

    CHECK(chunk.tileAt(position).isEmpty());
    CHECK(chunk.isEmpty());
    CHECK(chunk.nonEmptyTileCount() == 0U);
    CHECK(chunk.revision() == 2U);
}

TEST_CASE("Clearing a chunk records one logical revision", "[world][chunk]")
{
    Chunk chunk{};

    CHECK(chunk.setTile(LocalTileCoordinate{0U, 0U}, Tile{1U}));

    CHECK(chunk.setTile(LocalTileCoordinate{31U, 31U}, Tile{2U}));

    CHECK(chunk.nonEmptyTileCount() == 2U);
    CHECK(chunk.revision() == 2U);

    CHECK(chunk.clear());

    CHECK(chunk.isEmpty());
    CHECK(chunk.nonEmptyTileCount() == 0U);
    CHECK(chunk.revision() == 3U);

    CHECK_FALSE(chunk.clear());
    CHECK(chunk.revision() == 3U);
}

TEST_CASE("Chunk tile storage is contiguous", "[world][chunk]")
{
    const Chunk chunk{};
    const auto tiles = chunk.tiles();

    REQUIRE(tiles.size() > 1U);

    CHECK(&tiles[1] == &tiles[0] + 1);
}