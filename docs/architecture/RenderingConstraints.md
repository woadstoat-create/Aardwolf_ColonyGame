# Rendering Architecture Constraints

## World

- The world uses orthogonal square tiles.
- World data is divided into chunks.
- The initial candidate chunk size is 32 × 32 tiles.
- Chunk size remains configurable until profiling provides evidence.
- The world supports multiple Z-levels.
- Completely empty chunks should not require full tile allocation.

## Layer preview

- The selected Z-level is rendered normally.
- Empty tiles may reveal visible terrain from lower Z-levels.
- Lower-level preview is capped at three levels beneath the selected level.
- Lower levels use progressively reduced opacity.
- Layer resolution should be cached rather than repeatedly scanning downward every frame.

## Rendering

- Simulation tile data is independent of rendering data.
- Render chunks cache generated vertex geometry.
- One sprite object per tile is prohibited.
- Rendering and GPU resource updates remain on the main/render thread.
- Worker threads may later generate CPU-side mesh data from immutable snapshots.
- Optimisation decisions must be supported by profiling.