#pragma once

#include <aardwolf/world/Chunk.hpp>
#include <aardwolf/world/Coordinates.hpp>
#include <aardwolf/world/Tile.hpp>
#include <cstddef>
#include <unordered_map>

namespace aardwolf::world
{
class World
{
  public:
    explicit World(std::size_t expectedChunkCount = 0U);

    World(const World&) = delete;
    World& operator=(const World&) = delete;

    World(World&&) = default;
    World& operator=(World&&) = default;

    [[nodiscard]] Tile tileAt(WorldTileCoordinate coordinate) const noexcept;

    [[nodiscard]] bool setTile(WorldTileCoordinate coordinate, Tile tile);

    [[nodiscard]] const Chunk* findChunk(ChunkCoordinate coordinate) const noexcept;

    [[nodiscard]] bool containsChunk(ChunkCoordinate coordinate) const noexcept;

    [[nodiscard]] std::size_t chunkCount() const noexcept;
    [[nodiscard]] std::size_t nonEmptyTileCount() const noexcept;

    void reserveChunks(std::size_t expectedChunkCount);
    void clear() noexcept;

  private:
    using ChunkMap = std::unordered_map<ChunkCoordinate, Chunk, ChunkCoordinateHash>;

    ChunkMap chunks_{};
    std::size_t nonEmptyTileCount_{};
};
} // namespace aardwolf::world