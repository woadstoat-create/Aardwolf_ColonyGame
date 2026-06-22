#pragma once

#include <aardwolf/world/Coordinates.hpp>
#include <aardwolf/world/Tile.hpp>
#include <array>
#include <cstdint>
#include <span>

namespace aardwolf::world
{
class Chunk
{
  public:
    using Storage = std::array<Tile, kTilesPerChunk>;

    [[nodiscard]] const Tile& tileAt(LocalTileCoordinate coordinate) const noexcept;

    [[nodiscard]] bool setTile(LocalTileCoordinate coordinate, Tile tile) noexcept;

    [[nodiscard]] bool clear() noexcept;

    [[nodiscard]] bool isEmpty() const noexcept;
    [[nodiscard]] std::uint16_t nonEmptyTileCount() const noexcept;
    [[nodiscard]] std::uint64_t revision() const noexcept;

    [[nodiscard]] std::span<const Tile, kTilesPerChunk> tiles() const noexcept;

  private:
    Storage tiles_{};
    std::uint64_t revision_{};
    std::uint16_t nonEmptyTileCount_{};
};
} // namespace aardwolf::world