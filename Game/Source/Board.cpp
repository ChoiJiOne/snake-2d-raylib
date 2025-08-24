#include "Board.h"
#include "GameAssert.h"
#include "GameLog.h"

Board::Board(const Vector2& center, float tileSize, int32_t rowTileCount, int32_t colTileCount)
	: _center(center)
	, _tileSize(tileSize)
	, _rowTileCount(rowTileCount)
	, _colTileCount(colTileCount)
{
	_tiles = CreateTiles();
	_isInitialized = true;
}

Board::~Board()
{
	if (_isInitialized)
	{
		Release();
	}
}

void Board::Tick(float deltaSeconds)
{
}

void Board::Render()
{
	RenderTiles();
	RenderGrid();
}

void Board::Release()
{
	if (!_isInitialized)
	{
		GAME_LOG_ERR("ALREADY_RELEASE_OR_FAILED_TO_INITIALIZE_BOARD");
		return;
	}

	_isInitialized = false;
}

const ETileState& Board::GetTileState(const BoardCoord& boardCoord) const
{
	GAME_CHECK(IsValid(boardCoord));
	int32_t offset = boardCoord.offsetX + boardCoord.offsetY * _colTileCount;
	return _tiles[offset].state;
}

void Board::SetTileState(const BoardCoord& boardCoord, const ETileState& tileState)
{
	GAME_CHECK(IsValid(boardCoord));
	int32_t offset = boardCoord.offsetX + boardCoord.offsetY * _colTileCount;
	_tiles[offset].state = tileState;
}

bool Board::IsFullTile()
{
	for (const auto& tile : _tiles)
	{
		if (tile.state == ETileState::NONE)
		{
			return false;
		}
	}

	return true;
}

std::vector<Tile> Board::CreateTiles()
{
	std::vector<Tile> tiles;

	float x = _center.x - _tileSize * static_cast<float>(_colTileCount) * 0.5f;
	float y = _center.y - _tileSize * static_cast<float>(_rowTileCount) * 0.5f;
	for (int row = 0; row < _rowTileCount; ++row)
	{
		for (int col = 0; col < _colTileCount; ++col)
		{
			Tile tile;
			tile.position = Vector2
			{ 
				x + static_cast<float>(col) * _tileSize, 
				y + static_cast<float>(row) * _tileSize 
			};
			tile.size = Vector2{ _tileSize, _tileSize };
			tile.state = ETileState::NONE;
			tiles.emplace_back(tile);
		}
	}

	return tiles;
}

bool Board::IsValid(const BoardCoord& boardCoord) const
{
	return (0 <= boardCoord.offsetX && boardCoord.offsetX < _colTileCount) 
		&& (0 <= boardCoord.offsetY && boardCoord.offsetY < _rowTileCount);
}

void Board::RenderTiles()
{
	for (const auto& tile : _tiles)
	{
		if (tile.state == ETileState::BODY)
		{
			DrawRectangleV(tile.position, tile.size, RED);
		}
		else if (tile.state == ETileState::FOOD)
		{
			DrawRectangleV(tile.position, tile.size, BLUE);
		}
	}
}

void Board::RenderGrid()
{
	float x = _center.x - _tileSize * static_cast<float>(_colTileCount) * 0.5f;
	float y = _center.y - _tileSize * static_cast<float>(_rowTileCount) * 0.5f;

	for (int row = 0; row <= _rowTileCount; ++row)
	{
		DrawLineV(
			Vector2{ x + _tileSize *                              0.0f, y + row * _tileSize },
			Vector2{ x + _tileSize * static_cast<float>(_colTileCount), y + row * _tileSize },
			BLACK
		);
	}

	for (int col = 0; col <= _colTileCount; ++col)
	{
		DrawLineV(
			Vector2{ x + col * _tileSize, y + _tileSize *                              0.0f },
			Vector2{ x + col * _tileSize, y + _tileSize * static_cast<float>(_rowTileCount) },
			BLACK
		);
	}
}
