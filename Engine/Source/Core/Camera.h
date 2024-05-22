#pragma once
#include "Core/../Core.h"
#include "Core/../CoreStructs/Vector.h"
class GE_API Camera
{

public:
	void StartShake(float duration);
	void UpdateCamera(float deltaTime);
	void MoveCamera(class Vector2 moveAmount);
	bool isMoving = false;
	int HasMovedDirection = 0;
	Vector2 PreviousOffset;
	float offsetX = 0;
	float offsetY = 0;

private:
	void ShakeCamera(float deltaTime);
	float xAcceleration;
	float xAccelerationStart = 10;
	bool isGoingLeft = true;
	float shakeDuration = 0.f;
};
