#include "Camera.h"
#include <iostream>
#include "Core/../CoreStructs/Vector.h"
void Camera::StartShake(float duration)
{
    if (isMoving)
        return;
    shakeDuration = duration;
    xAcceleration = xAccelerationStart;
    isMoving = true;
}

void Camera::UpdateCamera(float deltaTime)
{
    if (shakeDuration > 0)
        ShakeCamera(deltaTime);
}

void Camera::MoveCameraRight()
{
    MoveCamera(Vector2(1, 0));
}

void Camera::MoveCameraLeft()
{
    MoveCamera(Vector2(-1, 0));
}

void Camera::MoveCameraUp()
{
    MoveCamera(Vector2(0, -1));
}

void Camera::MoveCameraDown()
{
    MoveCamera(Vector2(0, 1));
}

void Camera::MoveCamera(Vector2 moveAmount)
{
    PreviousOffset = Vector2(offsetX + moveAmount.X, offsetY + moveAmount.Y).ToInt();
    offsetX += moveAmount.X;
    offsetY += moveAmount.Y;
    int moveXDirection = (int) offsetX - (int)(offsetX + moveAmount.X);
    int moveYDirection = (int) offsetY - (int)(offsetY + moveAmount.Y);

    if(moveXDirection > 0) HasMovedDirection = RIGHTDIRECTION * moveAmount.Length();
    else if(moveXDirection < 0) HasMovedDirection = LEFTDIRECTION * moveAmount.Length();
    else if(moveYDirection > 0) HasMovedDirection = UPDIRECTION * moveAmount.Length();
    else if(moveYDirection < 0) HasMovedDirection = DOWNDIRECTION * moveAmount.Length();
}

void Camera::ShakeCamera(float deltaTime)
{

    if (isGoingLeft)
    {
        offsetX = deltaTime * xAcceleration;
        offsetY = -deltaTime * xAcceleration;
        if (offsetX >= 0.01f)
        {
            isGoingLeft = false;
        }
        else
        {
            // xAcceleration += 0.5f * deltaTime;
        }
    }
    else
    {
        offsetX = -deltaTime * xAcceleration;
        offsetY = deltaTime * xAcceleration;
        if (offsetX <= -0.01f)
        {
            isGoingLeft = true;
        }
        else
        {
            // xAcceleration += 0.5f * deltaTime;
        }
    }
    shakeDuration -= deltaTime;
    if (shakeDuration <= 0)
    {
        isMoving = false;
        offsetX = 0;
        offsetY = 0;
    }
}
