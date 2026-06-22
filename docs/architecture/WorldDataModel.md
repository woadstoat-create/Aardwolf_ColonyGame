# World Data Model

## Coordinate systems

The world uses three coordinate types:

- `WorldTileCoordinate`: signed world-space X, Y and Z.
- `ChunkCoordinate`: signed chunk X and Y with one Z-level.
- `LocalTileCoordinate`: unsigned X and Y inside a chunk.

Chunks divide only the horizontal plane. Every chunk belongs to exactly one
Z-level.

## Chunk dimensions

The initial chunk dimensions are:

- Width: 32 tiles
- Height: 32 tiles
- Tile capacity: 1,024

The chunk size is a benchmark candidate and may be changed after profiling.

## Negative coordinates

World-to-chunk conversion uses floor division rather than C++'s normal
integer division.

Examples:

| World X | Chunk X | Local X |
|--------:|--------:|--------:|
| -33 | -2 | 31 |
| -32 | -1 | 0 |
| -1 | -1 | 31 |
| 0 | 0 | 0 |
| 31 | 0 | 31 |
| 32 | 1 | 0 |

## Tile representation

A tile stores a 16-bit `TileId`.

ID zero represents empty space.

Static properties such as movement blocking and atlas coordinates belong to
a shared tile-definition registry rather than being duplicated in every tile.

The raw tile payload is two bytes:

- 2 bytes per tile
- 2,048 bytes of raw tile data per 32 × 32 chunk

## Chunk representation

Each chunk contains:

- A contiguous row-major array of 1,024 tiles
- A non-empty tile count
- A revision number

The revision changes only when tile contents change.

Mutable tile storage is not exposed publicly. All writes pass through
`Chunk::setTile()`.

## Sparse world storage

The world stores chunks in an `std::unordered_map` keyed by
`ChunkCoordinate`.

An absent chunk behaves as empty space.

Writing an empty tile into absent space does not allocate a chunk. Clearing
the last non-empty tile automatically removes its chunk.

## Z-level previews

The lower-layer preview is not calculated by the world-storage layer.

The renderer or a dedicated visibility resolver will inspect up to three
levels beneath the selected Z-level. Keeping this logic outside `World`
prevents rendering policy from contaminating simulation storage.

## Threading

`World` and `Chunk` are not currently thread-safe.

Future worker jobs will operate on immutable chunk snapshots. Rendering and
GPU uploads will remain on the render thread.