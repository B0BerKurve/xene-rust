#pragma once
#include "includes.hpp"
#include "game/classes.hpp"
#include "utils/returnspoofer.hpp"
#include "utils/memory.hpp"

uintptr_t current_;
EventType event_;

namespace GUI
{
	namespace Input
	{
		bool mouseDown[5];
		bool mouseDownAlready[256];

		bool keysDown[256];
		bool keysDownAlready[256];

		bool IsAnyMouseDown()
		{
			if (mouseDown[0]) return true;
			if (mouseDown[1]) return true;
			if (mouseDown[2]) return true;
			if (mouseDown[3]) return true;
			if (mouseDown[4]) return true;

			return false;
		}

		bool IsMouseClicked(int button, int element_id, bool repeat)
		{
			if (mouseDown[button])
			{
				if (!mouseDownAlready[element_id])
				{
					mouseDownAlready[element_id] = true;
					return true;
				}
				if (repeat)
					return true;
			}
			else
			{
				mouseDownAlready[element_id] = false;
			}
			return false;
		}
		bool IsKeyPressed(int key, bool repeat)
		{
			if (keysDown[key])
			{
				if (!keysDownAlready[key])
				{
					keysDownAlready[key] = true;
					return true;
				}
				if (repeat)
					return true;
			}
			else
			{
				keysDownAlready[key] = false;
			}
			return false;
		}

		void Handle()
		{
			if (GetAsyncKeyState(0x01))
			//KeyCode key = functions::get_keyCode(current_);
			//if (event_ == EventType::KeyDown && key == KeyCode::Mouse1)
				mouseDown[0] = true;
			else
				mouseDown[0] = false;
		}
	}
}