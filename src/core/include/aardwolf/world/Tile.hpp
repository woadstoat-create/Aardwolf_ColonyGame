#pragma once

#include <cstdint>
#include <type_traits>

namespace aardwolf::world
{
using TileId = std::uint16_t;

inline constexpr TileId kEmptyTileId = 0U;

enum class TileFlags : std::uint16_t
{
    None = 0U,
    BlocksMovement = 1U << 0U,
    BlocksVision = 1U << 1U,
    IsFloor = 1U << 2U,
    IsLiquid = 1U << 3U,
    SupportsConstruction = 1U << 4U
};

[[nodiscard]] constexpr TileFlags operator|(const TileFlags left, const TileFlags right) noexcept
{
    return static_cast<TileFlags>(static_cast<std::uint16_t>(left) |
                                  static_cast<std::uint16_t>(right));
}

[[nodiscard]] constexpr TileFlags operator&(const TileFlags left, const TileFlags right) noexcept
{
    return static_cast<TileFlags>(static_cast<std::uint16_t>(left) &
                                  static_cast<std::uint16_t>(right));
}

constexpr TileFlags& operator|=(TileFlags& left, const TileFlags right) noexcept
{
    left = left | right;
    return left;
}

[[nodiscard]] constexpr bool hasFlag(const TileFlags value, const TileFlags flag) noexcept
{
    return (value & flag) == flag;
}

struct Tile
{
    TileId id{kEmptyTileId};

    [[nodiscard]] constexpr bool isEmpty() const noexcept
    {
        return id == kEmptyTileId;
    }

    friend constexpr bool operator==(const Tile&, const Tile&) = default;
};

static_assert(sizeof(Tile) == sizeof(TileId));
static_assert(std::is_trivially_copyable_v<Tile>);
} // namespace aardwolf::world