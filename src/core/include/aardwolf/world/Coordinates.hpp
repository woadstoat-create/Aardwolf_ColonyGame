#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>

namespace aardwolf::world
{
using Coordinate = std::int32_t;

inline constexpr Coordinate kChunkWidth = 32;
inline constexpr Coordinate kChunkHeight = 32;

inline constexpr std::size_t kTilesPerChunk =
    static_cast<std::size_t>(kChunkWidth) * static_cast<std::size_t>(kChunkHeight);

struct WorldTileCoordinate
{
    Coordinate x{};
    Coordinate y{};
    Coordinate z{};

    friend constexpr bool operator==(const WorldTileCoordinate&,
                                     const WorldTileCoordinate&) = default;
};

struct ChunkCoordinate
{
    Coordinate x{};
    Coordinate y{};
    Coordinate z{};

    friend constexpr bool operator==(const ChunkCoordinate&, const ChunkCoordinate&) = default;
};

struct LocalTileCoordinate
{
    std::uint8_t x{};
    std::uint8_t y{};

    friend constexpr bool operator==(const LocalTileCoordinate&,
                                     const LocalTileCoordinate&) = default;
};

struct TileAddress
{
    ChunkCoordinate chunk{};
    LocalTileCoordinate local{};

    friend constexpr bool operator==(const TileAddress&, const TileAddress&) = default;
};

namespace detail
{
[[nodiscard]] constexpr Coordinate floorDivide(const Coordinate value,
                                               const Coordinate divisor) noexcept
{
    const Coordinate quotient = value / divisor;
    const Coordinate remainder = value % divisor;

    return quotient - ((remainder < 0) ? 1 : 0);
}

[[nodiscard]] constexpr Coordinate positiveModulo(const Coordinate value,
                                                  const Coordinate divisor) noexcept
{
    const Coordinate remainder = value % divisor;

    return (remainder < 0) ? remainder + divisor : remainder;
}
} // namespace detail

[[nodiscard]] constexpr bool isValid(const LocalTileCoordinate coordinate) noexcept
{
    return static_cast<Coordinate>(coordinate.x) < kChunkWidth &&
           static_cast<Coordinate>(coordinate.y) < kChunkHeight;
}

[[nodiscard]] constexpr TileAddress toTileAddress(const WorldTileCoordinate coordinate) noexcept
{
    return TileAddress{
        .chunk = ChunkCoordinate{.x = detail::floorDivide(coordinate.x, kChunkWidth),
                                 .y = detail::floorDivide(coordinate.y, kChunkHeight),
                                 .z = coordinate.z},
        .local = LocalTileCoordinate{
            .x = static_cast<std::uint8_t>(detail::positiveModulo(coordinate.x, kChunkWidth)),
            .y = static_cast<std::uint8_t>(detail::positiveModulo(coordinate.y, kChunkHeight))}};
}

[[nodiscard]] constexpr std::size_t toLinearIndex(const LocalTileCoordinate coordinate) noexcept
{
    return static_cast<std::size_t>(coordinate.y) * static_cast<std::size_t>(kChunkWidth) +
           static_cast<std::size_t>(coordinate.x);
}

[[nodiscard]] constexpr WorldTileCoordinate toWorldTileCoordinate(
    const ChunkCoordinate chunk, const LocalTileCoordinate local) noexcept
{
    return WorldTileCoordinate{.x = chunk.x * kChunkWidth + static_cast<Coordinate>(local.x),
                               .y = chunk.y * kChunkHeight + static_cast<Coordinate>(local.y),
                               .z = chunk.z};
}

struct ChunkCoordinateHash
{
    [[nodiscard]] std::size_t operator()(const ChunkCoordinate coordinate) const noexcept
    {
        std::size_t seed = std::hash<Coordinate>{}(coordinate.x);

        const auto combine = [&seed](const Coordinate value) noexcept
        {
            constexpr std::size_t kHashConstant = 0x9e3779b9U;

            seed ^= std::hash<Coordinate>{}(value) + kHashConstant + (seed << 6U) + (seed >> 2U);
        };

        combine(coordinate.y);
        combine(coordinate.z);

        return seed;
    }
};
} // namespace aardwolf::world