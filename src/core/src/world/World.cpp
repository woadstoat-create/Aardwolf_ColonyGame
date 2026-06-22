#include <aardwolf/world/World.hpp>

namespace aardwolf::world
{
World::World(const std::size_t expectedChunkCount)
{
    reserveChunks(expectedChunkCount);
}

Tile World::tileAt(const WorldTileCoordinate coordinate) const noexcept
{
    const TileAddress address = toTileAddress(coordinate);
    const auto iterator = chunks_.find(address.chunk);

    if (iterator == chunks_.end())
    {
        return Tile{};
    }

    return iterator->second.tileAt(address.local);
}

bool World::setTile(const WorldTileCoordinate coordinate, const Tile tile)
{
    const TileAddress address = toTileAddress(coordinate);
    auto iterator = chunks_.find(address.chunk);

    if (iterator == chunks_.end())
    {
        if (tile.isEmpty())
        {
            return false;
        }

        auto [insertedIterator, inserted] = chunks_.try_emplace(address.chunk);

        (void)inserted;
        iterator = insertedIterator;
    }

    Chunk& chunk = iterator->second;
    const Tile previous = chunk.tileAt(address.local);

    if (!chunk.setTile(address.local, tile))
    {
        return false;
    }

    if (previous.isEmpty() && !tile.isEmpty())
    {
        ++nonEmptyTileCount_;
    }
    else if (!previous.isEmpty() && tile.isEmpty())
    {
        --nonEmptyTileCount_;
    }

    if (chunk.isEmpty())
    {
        chunks_.erase(iterator);
    }

    return true;
}

const Chunk* World::findChunk(const ChunkCoordinate coordinate) const noexcept
{
    const auto iterator = chunks_.find(coordinate);

    return (iterator == chunks_.end()) ? nullptr : &iterator->second;
}

bool World::containsChunk(const ChunkCoordinate coordinate) const noexcept
{
    return chunks_.find(coordinate) != chunks_.end();
}

std::size_t World::chunkCount() const noexcept
{
    return chunks_.size();
}

std::size_t World::nonEmptyTileCount() const noexcept
{
    return nonEmptyTileCount_;
}

void World::reserveChunks(const std::size_t expectedChunkCount)
{
    chunks_.reserve(expectedChunkCount);
}

void World::clear() noexcept
{
    chunks_.clear();
    nonEmptyTileCount_ = 0U;
}
} // namespace aardwolf::world