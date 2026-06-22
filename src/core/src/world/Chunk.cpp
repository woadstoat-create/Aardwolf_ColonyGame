#include <aardwolf/world/Chunk.hpp>
#include <cassert>

namespace aardwolf::world
{
const Tile& Chunk::tileAt(const LocalTileCoordinate coordinate) const noexcept
{
    assert(isValid(coordinate));

    return tiles_[toLinearIndex(coordinate)];
}

bool Chunk::setTile(const LocalTileCoordinate coordinate, const Tile tile) noexcept
{
    assert(isValid(coordinate));

    Tile& current = tiles_[toLinearIndex(coordinate)];

    if (current == tile)
    {
        return false;
    }

    if (current.isEmpty() && !tile.isEmpty())
    {
        ++nonEmptyTileCount_;
    }
    else if (!current.isEmpty() && tile.isEmpty())
    {
        --nonEmptyTileCount_;
    }

    current = tile;
    ++revision_;

    return true;
}

bool Chunk::clear() noexcept
{
    if (isEmpty())
    {
        return false;
    }

    tiles_.fill(Tile{});
    nonEmptyTileCount_ = 0U;
    ++revision_;

    return true;
}

bool Chunk::isEmpty() const noexcept
{
    return nonEmptyTileCount_ == 0U;
}

std::uint16_t Chunk::nonEmptyTileCount() const noexcept
{
    return nonEmptyTileCount_;
}

std::uint64_t Chunk::revision() const noexcept
{
    return revision_;
}

std::span<const Tile, kTilesPerChunk> Chunk::tiles() const noexcept
{
    return tiles_;
}
} // namespace aardwolf::world