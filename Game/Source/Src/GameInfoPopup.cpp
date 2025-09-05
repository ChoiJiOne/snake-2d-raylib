#include <raygui.h>

#include "GameApp.h"
#include "GameAssert.h"
#include "GameLog.h"
#include "GameInfoPopup.h"
#include "MainPhase.h"
#include "PhaseManager.h"

GameInfoPopup::GameInfoPopup(Vector2 center, Vector2 size, Snake* snake)
	: _snake(snake)
{
	GAME_CHECK(snake != nullptr);

	_round.x = center.x - size.x * 0.5f;
	_round.y = center.y - size.x * 0.5f;
	_round.width = size.x;
	_round.height = size.y;
}

GameInfoPopup::~GameInfoPopup()
{
	if (_isInitialized)
	{
		Release();
	}
}

void GameInfoPopup::Tick(float deltaSeconds)
{
	if (_snake->IsDead() && !_isShowMessageBox)
	{
		_isShowMessageBox = true;
	}
}

void GameInfoPopup::Render()
{
	if (!_isShowMessageBox)
	{
		return;
	}

	int32_t result = GuiMessageBox(_round, "GAME OVER", TextFormat("SCORE: %d\nLEVEL: %d", _snake->GetScore(), _snake->GetLevel()), "OK");
	if (result >= 0)
	{
		_isShowMessageBox = false;

		MainPhase* mainPhase = reinterpret_cast<MainPhase*>(PhaseManager::Get().GetRegisteredPhase("MainPhase"));
		mainPhase->SetGameOver(true);
	}
}

void GameInfoPopup::Release()
{
	if (!_isInitialized)
	{
		GAME_LOG_ERR("ALREADY_RELEASE_OR_FAILED_TO_INITIALIZE_GAME_OVER_POPPUP");
		return;
	}

	_isInitialized = false;
}