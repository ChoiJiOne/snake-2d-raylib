#pragma once

#include <vector>

#include <raylib.h>

#include "IActor.h"

enum class ETileState
{
	NONE = 0x00,
	BODY = 0x01,
	FOOD = 0x02,
};

struct Tile
{
	Vector2 position;
	Vector2 size;
	ETileState state;
};

struct BoardCoord
{
	int32_t offsetX = 0;
	int32_t offsetY = 0;
};

class Board : public IActor
{
public:
	Board(const Vector2& center, float tileSize, int32_t rowTileCount, int32_t colTileCount);
	virtual ~Board();

	DISALLOW_COPY_AND_ASSIGN(Board);

	virtual void Tick(float deltaSeconds) override;
	virtual void Render() override;
	virtual void Release() override;

	const float& GetTileSize() const { return _tileSize; }
	const int32_t& GetRowTileCount() const { return _rowTileCount; }
	const int32_t& GetColTileCount() const { return _colTileCount; }
	const ETileState& GetTileState(const BoardCoord& boardCoord) const;
	const Tile& GetTile(const BoardCoord& boardCoord) const;

	void SetTileState(const BoardCoord& boardCoord, const ETileState& tileState);

	bool IsFullTile();

private:
	std::vector<Tile> CreateTiles();

	bool IsValid(const BoardCoord& boardCoord) const;

	void RenderTiles();
	void RenderGrid();

private:
	Vector2 _center = { 0.0f, 0.0f };
	float _tileSize = 0.0f;
	int32_t _rowTileCount = 0;
	int32_t _colTileCount = 0;
	std::vector<Tile> _tiles;
};