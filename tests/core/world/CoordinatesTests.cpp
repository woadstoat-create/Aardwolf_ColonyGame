#include <aardwolf/world/Coordinates.hpp>
#include <catch2/catch_test_macros.hpp>

namespace
{
using aardwolf::world::ChunkCoordinate;
using aardwolf::world::Coordinate;
using aardwolf::world::kTilesPerChunk;
using aardwolf::world::LocalTileCoordinate;
using aardwolf::world::TileAddress;
using aardwolf::world::toLinearIndex;
using aardwolf::world::toTileAddress;
using aardwolf::world::toWorldTileCoordinate;
using aardwolf::world::WorldTileCoordinate;
} // namespace

TEST_CASE("Positive world coordinates convert to chunk addresses", "[world][coordinates]")
{
    SECTION("Origin")
    {
        const TileAddress actual = toTileAddress(WorldTileCoordinate{0, 0, 0});

        const TileAddress expected{ChunkCoordinate{0, 0, 0}, LocalTileCoordinate{0U, 0U}};

        CHECK(actual == expected);
    }

    SECTION("Last tile in the origin chunk")
    {
        const TileAddress actual = toTileAddress(WorldTileCoordinate{31, 31, 0});

        const TileAddress expected{ChunkCoordinate{0, 0, 0}, LocalTileCoordinate{31U, 31U}};

        CHECK(actual == expected);
    }

    SECTION("First tile in the next chunk")
    {
        const TileAddress actual = toTileAddress(WorldTileCoordinate{32, 32, 0});

        const TileAddress expected{ChunkCoordinate{1, 1, 0}, LocalTileCoordinate{0U, 0U}};

        CHECK(actual == expected);
    }
}

TEST_CASE("Negative world coordinates use floor-based chunk addressing", "[world][coordinates]")
{
    SECTION("Negative one maps to local tile thirty-one")
    {
        const TileAddress actual = toTileAddress(WorldTileCoordinate{-1, -1, -2});

        const TileAddress expected{ChunkCoordinate{-1, -1, -2}, LocalTileCoordinate{31U, 31U}};

        CHECK(actual == expected);
    }

    SECTION("Negative chunk boundary")
    {
        const TileAddress actual = toTileAddress(WorldTileCoordinate{-32, -32, 4});

        const TileAddress expected{ChunkCoordinate{-1, -1, 4}, LocalTileCoordinate{0U, 0U}};

        CHECK(actual == expected);
    }

    SECTION("One tile beyond the negative boundary")
    {
        const TileAddress actual = toTileAddress(WorldTileCoordinate{-33, -33, 4});

        const TileAddress expected{ChunkCoordinate{-2, -2, 4}, LocalTileCoordinate{31U, 31U}};

        CHECK(actual == expected);
    }
}

TEST_CASE("Coordinate conversion round-trips", "[world][coordinates]")
{
    for (Coordinate z = -2; z <= 2; ++z)
    {
        for (Coordinate y = -70; y <= 70; ++y)
        {
            for (Coordinate x = -70; x <= 70; ++x)
            {
                const WorldTileCoordinate original{x, y, z};
                const TileAddress address = toTileAddress(original);

                const WorldTileCoordinate reconstructed =
                    toWorldTileCoordinate(address.chunk, address.local);

                CHECK(reconstructed == original);
            }
        }
    }
}

TEST_CASE("Local coordinates convert to row-major indices", "[world][coordinates]")
{
    CHECK(toLinearIndex(LocalTileCoordinate{0U, 0U}) == 0U);
    CHECK(toLinearIndex(LocalTileCoordinate{31U, 0U}) == 31U);
    CHECK(toLinearIndex(LocalTileCoordinate{0U, 1U}) == 32U);

    CHECK(toLinearIndex(LocalTileCoordinate{31U, 31U}) == kTilesPerChunk - 1U);
}