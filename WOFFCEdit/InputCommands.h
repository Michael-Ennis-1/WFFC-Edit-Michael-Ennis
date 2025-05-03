#pragma once

struct InputCommands
{
	bool forward;
	bool back;
	bool right;
	bool left;
	bool rotRight;
	bool rotLeft;

	bool rightMouseDown;
	bool leftMouseDown;
	DirectX::SimpleMath::Vector2 MousePos = { 0, 0 };
};