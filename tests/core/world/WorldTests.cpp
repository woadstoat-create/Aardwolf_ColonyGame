#include <aardwolf/world/World.hpp>
#include <catch2/catch_test_macros.hpp>

namespace
{
using aardwolf::world::ChunkCoordinate;
using aardwolf::world::Tile;
using aardwolf::world::World;
using aardwolf::world::WorldTileCoordinate;
} // namespace

TEST_CASE("Reading absent world data returns an empty tile", "[world][storage]")
{
    const World world{};

    CHECK(world.tileAt(WorldTileCoordinate{0, 0, 0}).isEmpty());

    CHECK(world.tileAt(WorldTileCoordinate{8000, -4000, -12}).isEmpty());

    CHECK(world.chunkCount() == 0U);
    CHECK(world.nonEmptyTileCount() == 0U);
}

TEST_CASE("Writing an empty tile does not allocate a chunk", "[world][storage]")
{
    World world{};

    CHECK_FALSE(world.setTile(WorldTileCoordinate{100, 200, 3}, Tile{}));

    CHECK(world.chunkCount() == 0U);
    CHECK(world.nonEmptyTileCount() == 0U);
}

TEST_CASE("Writing a non-empty tile allocates its chunk", "[world][storage]")
{
    World world{};
    const WorldTileCoordinate position{40, 70, -2};

    CHECK(world.setTile(position, Tile{5U}));

    CHECK(world.tileAt(position) == Tile{5U});
    CHECK(world.chunkCount() == 1U);
    CHECK(world.nonEmptyTileCount() == 1U);

    CHECK(world.containsChunk(ChunkCoordinate{1, 2, -2}));
}

TEST_CASE("Writing the same world tile is ignored", "[world][storage]")
{
    World world{};
    const WorldTileCoordinate position{4, 8, 1};

    CHECK(world.setTile(position, Tile{3U}));
    CHECK_FALSE(world.setTile(position, Tile{3U}));

    CHECK(world.chunkCount() == 1U);
    CHECK(world.nonEmptyTileCount() == 1U);
}

TEST_CASE("Multiple tiles in one chunk share storage", "[world][storage]")
{
    World world{};

    CHECK(world.setTile(WorldTileCoordinate{1, 1, 0}, Tile{1U}));

    CHECK(world.setTile(WorldTileCoordinate{20, 20, 0}, Tile{2U}));

    CHECK(world.chunkCount() == 1U);
    CHECK(world.nonEmptyTileCount() == 2U);

    const auto* chunk = world.findChunk(ChunkCoordinate{0, 0, 0});

    REQUIRE(chunk != nullptr);

    CHECK(chunk->nonEmptyTileCount() == 2U);
    CHECK(chunk->revision() == 2U);
}

TEST_CASE("Tiles in different chunks allocate separate storage", "[world][storage]")
{
    World world{};

    CHECK(world.setTile(WorldTileCoordinate{31, 0, 0}, Tile{1U}));

    CHECK(world.setTile(WorldTileCoordinate{32, 0, 0}, Tile{1U}));

    CHECK(world.setTile(WorldTileCoordinate{-1, 0, 0}, Tile{1U}));

    CHECK(world.chunkCount() == 3U);
    CHECK(world.nonEmptyTileCount() == 3U);
}

TEST_CASE("Z-levels occupy independent chunks", "[world][storage]")
{
    World world{};

    CHECK(world.setTile(WorldTileCoordinate{5, 5, 0}, Tile{1U}));

    CHECK(world.setTile(WorldTileCoordinate{5, 5, -1}, Tile{2U}));

    CHECK(world.setTile(WorldTileCoordinate{5, 5, -3}, Tile{3U}));

    CHECK(world.chunkCount() == 3U);
    CHECK(world.nonEmptyTileCount() == 3U);

    CHECK(world.tileAt(WorldTileCoordinate{5, 5, 0}) == Tile{1U});

    CHECK(world.tileAt(WorldTileCoordinate{5, 5, -1}) == Tile{2U});

    CHECK(world.tileAt(WorldTileCoordinate{5, 5, -3}) == Tile{3U});
}

TEST_CASE("Clearing the final tile removes its chunk", "[world][storage]")
{
    World world{};
    const WorldTileCoordinate position{-1, -33, 2};

    CHECK(world.setTile(position, Tile{6U}));
    CHECK(world.chunkCount() == 1U);

    CHECK(world.setTile(position, Tile{}));

    CHECK(world.tileAt(position).isEmpty());
    CHECK(world.chunkCount() == 0U);
    CHECK(world.nonEmptyTileCount() == 0U);

    CHECK_FALSE(world.containsChunk(ChunkCoordinate{-1, -2, 2}));
}

TEST_CASE("A chunk remains while it contains another tile", "[world][storage]")
{
    World world{};

    const WorldTileCoordinate first{1, 1, 0};
    const WorldTileCoordinate second{2, 2, 0};

    CHECK(world.setTile(first, Tile{1U}));
    CHECK(world.setTile(second, Tile{2U}));

    CHECK(world.setTile(first, Tile{}));

    CHECK(world.chunkCount() == 1U);
    CHECK(world.nonEmptyTileCount() == 1U);
    CHECK(world.tileAt(second) == Tile{2U});
}

TEST_CASE("Clearing the world removes all chunks", "[world][storage]")
{
    World world{16U};

    CHECK(world.setTile(WorldTileCoordinate{0, 0, 0}, Tile{1U}));

    CHECK(world.setTile(WorldTileCoordinate{100, 100, 4}, Tile{2U}));

    world.clear();

    CHECK(world.chunkCount() == 0U);
    CHECK(world.nonEmptyTileCount() == 0U);
}