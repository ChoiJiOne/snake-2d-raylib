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
	if (_snake->IsDead() && _state == EState::PLAY)
	{
		_title = "GAME OVER";
		_info = TextFormat("SCORE: %d\nLEVEL: %d", _snake->GetScore(), _snake->GetLevel());
		_buttons = "OK";

		_state = EState::GAME_OVER;
	}

	if (!IsWindowFocused() && _snake->IsAlive())
	{
		_snake->Stop();

		_title = "GAME PAUSE";
		_info = "CONTINUE?";
		_buttons = "OK;CANCEL";
		_state = EState::PAUSE;
	}
}

void GameInfoPopup::Render()
{
	if (_state == EState::PLAY)
	{
		return;
	}

	int32_t result = GuiMessageBox(_round, _title.c_str(), _info.c_str(), _buttons.c_str());
	if (result >= 0)
	{
		switch (_state)
		{
		case EState::PAUSE:
			if (result == 0 || result == 1) // OK
			{
				_snake->WakeUp();
				_state = EState::PLAY;
			}
			else // CANCEL
			{
				MainPhase* mainPhase = reinterpret_cast<MainPhase*>(PhaseManager::Get().GetRegisteredPhase("MainPhase"));
				mainPhase->SetGameOver(true);
			}
			break;

		case EState::GAME_OVER:
			MainPhase* mainPhase = reinterpret_cast<MainPhase*>(PhaseManager::Get().GetRegisteredPhase("MainPhase"));
			mainPhase->SetGameOver(true);
			break;
		}
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