#include "Food.h"
#include "GameAssert.h"
#include "GameLog.h"

Food::Food(Board* board, TextEffect* effect)
	: _board(board)
	, _effect(effect)
{
	// 처음에 실패하면 설정에 이슈 있음.
	GAME_ASSERT(TrySetRandomCoord(), "INVALID_BOARD_STATE_OR_SNAKE_START_BODY_SIZE");
	_isInitialized = true;
}

Food::~Food()
{
	if (_isInitialized)
	{
		Release();
	}
}

void Food::Tick(float deltaSeconds)
{
	if (!IsEaten())
	{
		return;
	}

	StartEffect();
	if (!TrySetRandomCoord())
	{
		// TODO: 더 이상 보드에 채울 수 없을 때의 적절한 처리 필요 (EX. 게임 오버)
	}
}

void Food::Render()
{
}

void Food::Release()
{
	if (!_isInitialized)
	{
		GAME_LOG_ERR("ALREADY_RELEASE_OR_FAILED_TO_INITIALIZE_FOOD");
		return;
	}

	_isInitialized = false;
}

bool Food::IsEaten()
{
	return _board->GetTileState(_boardCoord) != ETileState::FOOD;
}

bool Food::TrySetRandomCoord()
{
	if (_board->IsFullTile())
	{
		return false;
	}

	BoardCoord newBoardCoord;
	while (true) // TODO: 이거 대신 다른 로직 생각해보기. (내부에서 무한 루프를???)
	{
		newBoardCoord.offsetX = GetRandomValue(0, _board->GetColTileCount() - 1);
		newBoardCoord.offsetY = GetRandomValue(0, _board->GetRowTileCount() - 1);

		if (_board->GetTileState(newBoardCoord) == ETileState::NONE)
		{
			_boardCoord = newBoardCoord;
			_board->SetTileState(newBoardCoord, ETileState::FOOD);
			break;
		}
	}

	return true;
}

void Food::StartEffect()
{
	const Tile& tile = _board->GetTile(_boardCoord);
	Vector2 center{ tile.position.x + tile.size.x * 0.5f, tile.position.y + tile.size.y * 0.5f };
	_effect->StartEffect(center);
}
