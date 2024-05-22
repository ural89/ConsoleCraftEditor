#include "PlayerController.h"
#include "Core/GameObject.h"

#include "Core/Input.h"
#include "Core/Scene.h"
PlayerController::PlayerController(GameObject &go, int playerNo) : Component(go)
{
	this->playerNo = playerNo;
}

PlayerController::PlayerController(GameObject &go) : Component(go)
{
}

void PlayerController::Init()
{
	inputEvent = std::bind(&PlayerController::OnKeyDown, this, std::placeholders::_1);
	Input::AddListener(inputEvent);

}

void PlayerController::Update(float deltaTime)
{
}

void PlayerController::OnKeyDown(int input)
{
	if(owner)
	{
		if (!owner->GetCurrentScene().isPaused)
		{
			if (playerNo == 0)
			{
				if (tolower(Input::GetKeyDown()) == 'd')
				{
					owner->transform.MovePosition(moveSpeed, 0);
				}
				if (tolower(Input::GetKeyDown()) == 'a')
				{
					owner->transform.MovePosition(-moveSpeed, 0);
				}
				if (tolower(Input::GetKeyDown()) == 's')
				{
					owner->transform.MovePosition(0, moveSpeed);
				}
				if (tolower(Input::GetKeyDown()) == 'w')
				{
					owner->transform.MovePosition(0, -moveSpeed);
				}
			}
			if (playerNo == 1)
			{
				if (tolower(Input::GetKeyDown()) == 'k')
				{
					owner->transform.MovePosition(1, 0);
				}
				if (tolower(Input::GetKeyDown()) == 'h')
				{
					owner->transform.MovePosition(-1, 0);
				}
				if (tolower(Input::GetKeyDown()) == 'j')
				{
					owner->transform.MovePosition(0, 1);
				}
				if (tolower(Input::GetKeyDown()) == 'u')
				{
					owner->transform.MovePosition(0, -1);
				}
			}
		}
	}
}

void PlayerController::RemoveListenerForInput()
{
	Input::RemoveListener(inputEvent);
}
