#pragma once
#include "ProjectConsts.h"
struct TMouse
{
	const float cX = 65535.0f / GetSystemMetrics(SM_CXVIRTUALSCREEN);
	const 	float cY = 65535.0f / GetSystemMetrics(SM_CYVIRTUALSCREEN);

	const int doX = GetSystemMetrics(SM_XVIRTUALSCREEN);
	const int doY = GetSystemMetrics(SM_YVIRTUALSCREEN);

	const float maxX = static_cast<float>(GetSystemMetrics(SM_CXVIRTUALSCREEN));
	const float maxY = static_cast<float>(GetSystemMetrics(SM_CYVIRTUALSCREEN));
	const float maxAngle = ANGLE_TO_SCREEN;

	POINT cursorPosB{ 0,0 };
	POINT cursorPos{ 0,0 };
	void SendMove(int x, int y)
	{
		INPUT input;
		ZeroMemory(&input, sizeof(INPUT));
		input.type = INPUT_MOUSE;
		input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_VIRTUALDESK;
		input.mi.dx = LONG((x - doX) * cX);
		input.mi.dy = LONG((y - doY) * cY);
		SendInput(1, &input, sizeof(INPUT));
	}

	void SendMoveHolddown(int x, int y)
	{
		INPUT input;
		ZeroMemory(&input, sizeof(INPUT));
		input.type = INPUT_MOUSE;
		input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_VIRTUALDESK | MOUSEEVENTF_LEFTDOWN;
		input.mi.dx = LONG((x - doX) * cX);
		input.mi.dy = LONG((y - doY) * cY);
		SendInput(1, &input, sizeof(INPUT));
	}


	const float d_X = static_cast<float>(toF(::GetSystemMetrics(SM_CXVIRTUALSCREEN)) / 2.0f) + toF(GetSystemMetrics(SM_XVIRTUALSCREEN));
	const float d_Y = static_cast<float>(toF(::GetSystemMetrics(SM_CYVIRTUALSCREEN)) / 2.0f) + toF(GetSystemMetrics(SM_YVIRTUALSCREEN));

	const float k_X = (toF(::GetSystemMetrics(SM_CXVIRTUALSCREEN)) / (maxAngle * 2.0f));
	const float k_Y = (toF(::GetSystemMetrics(SM_CYVIRTUALSCREEN)) / (maxAngle * 2.0f));

	float td_X = 0.0;
	float td_Y = 0.0;

	void toZero()
	{
		td_X = 0.0;
		td_Y = 0.0;
	}

	void SendMoveF(float ax, float ay)
	{
		SendMove(static_cast<int>(d_X + td_X + roundBase(ax, 1.0f) * k_X), static_cast<int>(d_Y + td_Y + roundBase(ay, 1.0f) * k_X));
	}
	void SendMoveHolddownF(float ax, float ay)
	{
		SendMoveHolddown(static_cast<int>(d_X + td_X + roundBase(ax, 1.0f) * k_X), static_cast<int>(d_Y + td_Y + roundBase(ay, 1.0f) * k_X));
	}

	void LeftClick()
	{
		INPUT    Input = { 0 };
		// left down 
		Input.type = INPUT_MOUSE;
		Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
		::SendInput(1, &Input, sizeof(INPUT));

		// left up
		::ZeroMemory(&Input, sizeof(INPUT));
		Input.type = INPUT_MOUSE;
		Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
		::SendInput(1, &Input, sizeof(INPUT));
	}
	void RightClick()
	{
		INPUT    Input = { 0 };
		// left down 
		Input.type = INPUT_MOUSE;
		Input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
		::SendInput(1, &Input, sizeof(INPUT));
		// left up
		::ZeroMemory(&Input, sizeof(INPUT));
		Input.type = INPUT_MOUSE;
		Input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
		::SendInput(1, &Input, sizeof(INPUT));
	}

	void rollUpClick()
	{
		INPUT    Input = { 0 };
		// left down 
		Input.type = INPUT_MOUSE;
		Input.mi.dwFlags = MOUSEEVENTF_WHEEL;
		Input.mi.mouseData = 120;
		::SendInput(1, &Input, sizeof(INPUT));
	}

	void rollDownClick()
	{
		INPUT    Input = { 0 };
		// left down 
		Input.type = INPUT_MOUSE;
		Input.mi.dwFlags = MOUSEEVENTF_WHEEL;
		Input.mi.mouseData = -120;
		::SendInput(1, &Input, sizeof(INPUT));
	}


	void LeftClickUp()
	{
		INPUT    Input = { 0 };

		::ZeroMemory(&Input, sizeof(INPUT));
		Input.type = INPUT_MOUSE;
		Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
		::SendInput(1, &Input, sizeof(INPUT));
	}

	void GetCursorPosition()
	{	//Sleep(2000);    // Wait for 2 seconds before get the cursor position.		
		GetCursorPos(&cursorPosB);
	}
} static mouse;
