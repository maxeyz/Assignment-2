#pragma once

#include "constants.h"
#include "paddle.h"

bool isColliding(const Paddle& pad, const GameObject& obj);
int Max(int x, int y);
int Min(int x, int y);

void SpawnBall()
{
	const int objectId = Play::CreateGameObject(::TYPE_BALL, { DISPLAY_WIDTH / 2, DISPLAY_HEIGHT - 60 }, 4, "ball");
	GameObject& ball = Play::GetGameObject(objectId);
	ball.velocity = normalize({ 1, -1 }) * ballSpeed;
}

float StepFrame(float time)
{
	const std::vector<int> ballIds = Play::CollectGameObjectIDsByType(TYPE_BALL);
	const std::vector<int> brickIds = Play::CollectGameObjectIDsByType(TYPE_BRICK);

	for (int i = 0; i < brickIds.size(); i++) //Iterates Brick type objects.
	{
		Play::UpdateGameObject(Play::GetGameObject(brickIds[i]));
		Play::DrawObject(Play::GetGameObject(brickIds[i]));

		for (int j = 0; j < ballIds.size(); j++) // Brick collision.
		{
			if (Play::IsColliding(Play::GetGameObject(ballIds[j]), Play::GetGameObject(brickIds[i])))
			{
				Play::DestroyGameObject(brickIds[i]);
				Play::GetGameObject(ballIds[j]).velocity.y *= -1;
			}
		}
	}

	for (int i = 0; i < ballIds.size(); i++)	//Iterates Ball type objects.
	{
		Play::UpdateGameObject(Play::GetGameObject(ballIds[i]));
		Play::DrawObject(Play::GetGameObject(ballIds[i]));

		if (Play::GetGameObject(ballIds[i]).pos.x > DISPLAY_WIDTH || Play::GetGameObject(ballIds[i]).pos.x < 0)		//Window collision x-axis.
		{
			Play::GetGameObject(ballIds[i]).velocity.x *= -1;
		}
		else if (Play::GetGameObject(ballIds[i]).pos.y > DISPLAY_HEIGHT || Play::GetGameObject(ballIds[i]).pos.y < 0)		//Window collision y-axis.
		{
			Play::GetGameObject(ballIds[i]).velocity.y *= -1;
		}

		if (isColliding(pad, Play::GetGameObject(ballIds[i]))) //Collision with Pad.
		{
			GameObject &obj = Play::GetGameObject(ballIds[i]);
			obj.velocity.y *= -1;
		}
	}

	return time;
}

void SetupScene()
{
	for (int x = 20; x < DISPLAY_WIDTH - 20; x = x + 18)
	{
		for (int y = 30; y < 100; y = y + 12)
		{
			const int objectId = Play::CreateGameObject(ObjectType::TYPE_BRICK, { x, y }, 6, "brick");
		}
	}
}

void DrawPaddle(const Paddle& pad)
{
	Play::DrawRect({ pad.x - 60, pad.y - 5 }, { pad.x + 60, pad.y + 5 }, Play::cWhite, true);
}

void UpdatePaddle(Paddle& pad)
{
	if (Play::KeyDown(VK_LEFT))
	{
		pad.x = pad.x - PAD_SPEED;
	}
	else if (Play::KeyDown(VK_RIGHT))
	{
		pad.x = pad.x + PAD_SPEED;
	}
}

int Max(int x, int y)
{
	if (x > y)
		return x;
	else
		return y;
}

int Min(int x, int y)
{
	if (x < y)
		return x;
	else
		return y;
}

bool isColliding(const Paddle& pad, const GameObject& obj) //Checks if the ball is within the collision radius of the pad, if it is, return true.
{
	const float dx = obj.pos.x - Max(pad.x - 60, Min(obj.pos.x, pad.x + 60));
	const float dy = obj.pos.y - Max(pad.y - 5, Min(obj.pos.y, pad.y + 5));
	return (dx * dx + dy * dy) < (obj.radius * obj.radius);
}