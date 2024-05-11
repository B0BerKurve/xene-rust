#pragma once

#include "includes.hpp"
#include "game/classes.hpp"
#include "utils/returnspoofer.hpp"
#include "utils/memory.hpp"
//#include "logo.h"
#include "input.h"
#include <algorithm>
#include "global.h"
#include <cctype>

enum class DrawOperationType {
	FillBox,
	DrawPixel,
	Line,
	DrawOutlineCircle,
	Label,
	Outline,
	OutlineBox,
	DrawTexture,
	SetDrawColor
};

struct Entry {
	DrawOperationType operationType;
	Vector4 vector1;
	Vector4 vector2;
	Vector2 point1;
	Vector2 point2;
	std::string text;
	bool centered;
	float fontSize;
	float girth;
	int x, y, radius;
	uintptr_t texture;

	// Constructor overloads for each type of drawing operation
	Entry(DrawOperationType opType, Vector4 vec1, Vector4 vec2)
		: operationType(opType), vector1(vec1), vector2(vec2) {}

	Entry(DrawOperationType opType, int x, int y)
		: operationType(opType), x(x), y(y) {}

	Entry(DrawOperationType opType, Vector2 p1, Vector2 p2, Vector4 color)
		: operationType(opType), point1(p1), point2(p2), vector2(color) {}

	Entry(DrawOperationType opType, int centerX, int centerY, int rad, Vector4 color)
		: operationType(opType), x(centerX), y(centerY), radius(rad), vector2(color) {}

	Entry(DrawOperationType opType, Vector4 pos, std::string txt, Vector4 color, bool cent, float fSize)
		: operationType(opType), vector1(pos), text(txt), vector2(color), centered(cent), fontSize(fSize) {}

	Entry(DrawOperationType opType, Vector2 p1, Vector2 p2, Vector4 color, float girthValue)
		: operationType(opType), point1(p1), point2(p2), vector2(color), girth(girthValue) {}

};

namespace GUI {
	bool hover_element = false;
	int inputBlocker = 0;
	Vector2 menu_pos = Vector2{ 0, 0 };
	float offset_x = 0.0f;
	float offset_x2 = 0.0f;
	float offset_y = 0.0f;
	float offset_y2 = 0.0f;

	std::vector<Entry> postOperations;
	std::unordered_map<std::string, std::unordered_map<std::string, bool>> dropdown_states;

	Vector2 first_element_pos = Vector2{ 0, 0 };

	Vector2 last_element_pos = Vector2{ 0, 0 };
	Vector2 last_element_size = Vector2{ 0, 0 };

	//Colors
	//window
	Vector4 windowBackground = Vector4(0.19215686274509805, 0.2, 0.2196078431372549, 1);
	Vector4 windowBorder = Vector4(0.11764705882352941, 0.12156862745098039, 0.13333333333333333, 1);
	//vert button menu
	Vector4 vertButtonBackground = Vector4(0.11764705882352941, 0.12156862745098039, 0.13333333333333333, 1);
	Vector4 vertButtonBorder = Vector4(0.25098039215686274, 0.25882352941176473, 0.28627450980392155, 1);
	Vector4 vertButtonHover = Vector4(0.1803921568627451, 0.18823529411764706, 0.20784313725490197, 1);
	Vector4 vertButtonActive = Vector4(0.19215686274509805, 0.2, 0.2196078431372549, 1);
	//only displayed if enabled
	Vector4 vertButtonSidebarBackground = Vector4(0.11764705882352941, 0.12156862745098039, 0.13333333333333333, 1);
	//tab button menu
	Vector4 tabButtonBackground = Vector4(0.2196078431372549, 0.5215686274509804, 0.8235294117647058, 255);
	Vector4 tabButtonBorder = Vector4(0.13725490196078433, 0.13725490196078433, 0.13725490196078433, 1);
	Vector4 tabButtonHover = Vector4(0.19215686274509805, 0.396078431372549, 0.6039215686274509, 1);
	Vector4 tabButtonActive = Vector4(0.19215686274509805, 0.396078431372549, 0.6039215686274509, 1);
	//subtab button menu
	Vector4 subTabBackground = Vector4(0.2196078431372549, 0.5215686274509804, 0.8235294117647058, 255);
	Vector4 subTabBorder = Vector4(0.13725490196078433, 0.13725490196078433, 0.13725490196078433, 1);
	Vector4 subTabHover = Vector4(0.19215686274509805, 0.396078431372549, 0.6039215686274509, 1);
	Vector4 subTabActive = Vector4(0.19215686274509805, 0.396078431372549, 0.6039215686274509, 1);
	//checkbox
	Vector4 checkboxBorder = Vector4(0.13725490196078433, 0.13725490196078433, 0.13725490196078433, 1);
	Vector4 checkboxHover = Vector4(0.796078431372549, 0.796078431372549, 0.796078431372549, 1);
	Vector4 checkboxActive = Vector4(0.13725490196078433, 0.6470588235294118, 0.34901960784313724, 1);
	//danger checkbox
	Vector4 dangerCheckboxBorder = Vector4(0.13725490196078433, 0.13725490196078433, 0.13725490196078433, 1);
	Vector4 dangerCheckboxHover = Vector4(0.796078431372549, 0.796078431372549, 0.796078431372549, 1);
	Vector4 dangerCheckboxActive = Vector4(0.8627450980392157, 0.23529411764705882, 0.24313725490196078, 1);
	Vector4 dangerCheckboxText = Vector4(0.9411764705882353, 0.6980392156862745, 0.1568627450980392, 1);
	Vector4 dangerCheckboxTextPopup = Vector4(0.8627450980392157, 0.23529411764705882, 0.24313725490196078, 1);
	Vector4 dangerCheckboxPopupBackground = Vector4(0.11764705882352941, 0.12156862745098039, 0.13333333333333333, 1);
	Vector4 dangerCheckboxPopupBorder = Vector4(0.16862745098039217, 0.17647058823529413, 0.19215686274509805, 1);
	//slider colors
	Vector4 sliderBackground = Vector4(0.2196078431372549, 0.5215686274509804, 0.8235294117647058, 1);
	Vector4 sliderBorder = Vector4(0.13725490196078433, 0.13725490196078433, 0.13725490196078433, 1);
	Vector4 sliderHover = Vector4(0.19215686274509805, 0.396078431372549, 0.6039215686274509, 1);
	//button colors
	Vector4 buttonBackground = Vector4(0.2196078431372549, 0.5215686274509804, 0.8235294117647058, 1);
	Vector4 buttonBorder = Vector4(0.13725490196078433, 0.13725490196078433, 0.13725490196078433, 1);
	Vector4 buttonHover = Vector4(0.19215686274509805, 0.396078431372549, 0.6039215686274509, 1);
	//hotkey colors
	Vector4 hotkeyBackground = Vector4(0.2196078431372549, 0.5215686274509804, 0.8235294117647058, 1);
	Vector4 hotkeyBorder = Vector4(0.13725490196078433, 0.13725490196078433, 0.13725490196078433, 1);
	Vector4 hotkeyHover = Vector4(0.19215686274509805, 0.396078431372549, 0.6039215686274509, 1);
	//combobox colors
	Vector4 comboboxBackground = Vector4(0.2196078431372549, 0.5215686274509804, 0.8235294117647058, 1);
	Vector4 comboboxBorder = Vector4(0.13725490196078433, 0.13725490196078433, 0.13725490196078433, 1);
	Vector4 comboboxHover = Vector4(0.796078431372549, 0.796078431372549, 0.796078431372549, 1);
	Vector4 comboboxActive = Vector4(0.19215686274509805, 0.396078431372549, 0.6039215686274509, 1);
	//divider colors
	Vector4 dividerColor = Vector4(0.16862745098039217, 0.17647058823529413, 0.19215686274509805, 1);
	Vector4 dividerTextColor = Vector4(0.7490196078431373, 0.7568627450980392, 0.7803921568627451, 1);
	//Dropdown colors
	Vector4 dropdownBackground = Vector4(0.2196078431372549, 0.5215686274509804, 0.8235294117647058, 1);
	Vector4 dropdownBorder = Vector4(0.13725490196078433, 0.13725490196078433, 0.13725490196078433, 1);
	Vector4 dropdownBackgroundBox = Vector4(0.6f, 0.6f, 0.6f, 1);
	Vector4 dropdownBackgroundArrow = Vector4(1, 1, 1, 1);
	Vector4 dropdownText = Vector4(1, 1, 1, 1);
	Vector4 dropdownTextActive = Vector4(0.13725490196078433, 0.6470588235294118, 0.34901960784313724, 1);
	Vector4 dropdownOptionsBackground = Vector4(0.11764705882352941, 0.12156862745098039, 0.13333333333333333, 1);
	Vector4 dropdownOptionsBorder = Vector4(0.13725490196078433, 0.13725490196078433, 0.13725490196078433, 1);
	Vector4 dropdownOptionsHover = Vector4(0.1803921568627451, 0.18823529411764706, 0.20784313725490197, 1);
	Vector4 triangleColor = Vector4(0.11764705882352941, 0.12156862745098039, 0.13333333333333333, 1);

	int current_element = -1;
	Vector2 current_element_pos = Vector2{ 0, 0 };
	Vector2 current_element_size = Vector2{ 0, 0 };
	int elements_count = 0;

	bool sameLine = false;
	bool sameCol = false;
	bool checkboxPad = false;

	bool searching_hotkey = false;

	float hues = 0;
	std::string color_picker = "";
	Vector4 saved_color = {};

	int menuX = 0;
	int menuY = 0;

	int window_width = 0;
	int window_height = 0;
	std::string last_hotkey = "";
	std::clock_t hotkey_call = 0;

	float last_width = 0;
	float last_height = 0;
	bool do_sameline = false;

	Vector2 dragPos;
	bool golbalSliderDrag = false;
	bool tempFoundSlider = false;
	float count = 0;

	using ButtonCallback = void (*)();

	void drawUpTriangle(int x, int y, int size, Vector4 color) {
		for (int i = 0; i < size; ++i) {
			int width = i * 2 + 1;
			functions::fill_box(Vector4(x - i, y + i, width, 1), color);
		}
	}

	void drawDownTriangle(int x, int y, int size, Vector4 color) {
		for (int i = 0; i < size; ++i) {
			int width = (size - i) * 2 - 1;
			functions::fill_box(Vector4(x + i - size + 1, y + i, width, 1), color);
		}
	}


	void get_hotkeys() {
		if (global::getCurrentTimeMillis() - hotkey_call > 400) {
			if (!functions::get_key(KeyCode::Insert)) {
				if (!functions::get_key(KeyCode::Escape)) {
					float loop_count = 0;
					int minKeyCode = static_cast<int>(KeyCode::Backspace);
					int maxKeyCode = static_cast<int>(KeyCode::Mouse7);

					for (int i = minKeyCode; i <= maxKeyCode; ++i) {
						loop_count = loop_count + 1;
						KeyCode key = static_cast<KeyCode>(i);
						if (key != KeyCode::F13) {
							if (functions::get_key(key)) {
								std::get<KeyCode>(ui::element[last_hotkey].value) = key;
								last_hotkey = "";
								hotkey_call = 0;
								searching_hotkey = false;
							}
						}
					}
				}
				else {
					std::get<KeyCode>(ui::element[last_hotkey].value) = KeyCode::Unbound;
					last_hotkey = "";
					hotkey_call = 0;
					searching_hotkey = false;
				}
			}
			else {
				last_hotkey = "";
				hotkey_call = 0;
				searching_hotkey = false;
			}
		}
	}

	Vector2 CursorPos()
	{
		//POINT cursorPos;
		//o_getcursorpos(&cursorPos);
		//return fvector2d{ (float)cursorPos.x, (float)cursorPos.y };
		auto cps = functions::get_mouse_pos();
		return Vector2(cps.x, window_height - cps.y);
		//return cached::controller->get_mouse_position();
	}

	bool MouseInZone(Vector2 pos, Vector2 size)
	{
		Vector2 cursor_pos = CursorPos();

		if (cursor_pos.x > pos.x && cursor_pos.y > pos.y)
			if (cursor_pos.x < pos.x + size.x && cursor_pos.y < pos.y + size.y)
				return true;

		return false;
	}

	void SameLine()
	{
		sameLine = true;
	}

	void SameCol()
	{
		sameCol = true;
	}

	void yPaddingFix()
	{
		checkboxPad = true;
		//tbh idk what I messed up to break Y padding for certain objects but ill fix it later
	}

	bool Window(std::string name, Vector2* pos, Vector2 size, bool isOpen)
	{
		elements_count = 0;

		if (!isOpen)
			return false;

		menuX = size.x;
		menuY = size.y;

		bool isHovered = MouseInZone(Vector2{ pos->x, pos->y }, size);
		KeyCode key = functions::get_keyCode(current_);

		// bool pressed_mouse0 = event_ == EventType::KeyDown && key == KeyCode::Mouse1;
		bool pressed_mouse0 = GetAsyncKeyState(VK_LBUTTON);


		if (current_element != -1 && !pressed_mouse0)
		{
			current_element = -1;
		}

		if (hover_element && pressed_mouse0)
		{

		}
		else if ((isHovered || dragPos.x != 0) && !hover_element)
		{
			if (Input::IsMouseClicked(0, elements_count, true))
			{
				Vector2 cursorPos = CursorPos();

				cursorPos.x -= size.x;
				cursorPos.y -= size.y;

				if (dragPos.x == 0)
				{
					dragPos.x = (cursorPos.x - pos->x);
					dragPos.y = (cursorPos.y - pos->y);
				}

				// Manual clamping to stay within the screen bounds
				if (cursorPos.x - dragPos.x < 0)
				{
					pos->x = 0;
				}
				else if (cursorPos.x - dragPos.x > functions::get_screen_width() - size.x)
				{
					pos->x = functions::get_screen_width() - size.x;
				}
				else
				{
					pos->x = cursorPos.x - dragPos.x;
				}

				if (cursorPos.y - dragPos.y < 0)
				{
					pos->y = 0;
				}
				else if (cursorPos.y - dragPos.y > functions::get_screen_height() - size.y)
				{
					pos->y = functions::get_screen_height() - size.y;
				}
				else
				{
					pos->y = cursorPos.y - dragPos.y;
				}
			}
			else
			{
				dragPos = Vector2{ 0, 0 };
			}
		}
		else
		{
			hover_element = false;
		}

		offset_x = 0.0f; offset_y = 0.0f; offset_y2 = 0.0f;
		menu_pos = Vector2{ pos->x, pos->y };
		first_element_pos = Vector2{ 0, 0 };
		current_element_pos = Vector2{ 0, 0 };
		current_element_size = Vector2{ 0, 0 };

		if (event_ == EventType::Repaint) {

			functions::fill_box(Vector4(pos->x - 5, pos->y - 5, size.x + 10, size.y + 10), windowBorder); //outline window
			functions::fill_box(Vector4(pos->x, pos->y, size.x, size.y), windowBackground);


			//offset_y += 25.0f;
			//offset_y2 += 25.0f;

			// Title
			Vector2 titlePos = Vector2{ pos->x + 40, pos->y + 4 };
			functions::label(Vector4(titlePos.x, titlePos.y, 100, 20), name, Vector4(1, 1, 1, 1), false, 14);
		}

		return true;
	}

	bool ButtonTab(std::string name, Vector2 size, bool active)
	{
		elements_count++;
		bool return_bool = false;
		Vector2 padding = Vector2{ 14, 10 };
		Vector2 pos = Vector2{ menu_pos.x + padding.x + offset_x, menu_pos.y + padding.y };
		if (sameLine)
		{
			pos.x = last_element_pos.x + last_element_size.x + padding.x;
			pos.y = last_element_pos.y;
		}
		if (event_ == EventType::Repaint) {
			bool isHovered = MouseInZone(Vector2{ pos.x, pos.y }, Vector2{ size.x, size.y });
			if (isHovered && (color_picker == "")) {
				hover_element = true;
				if (Input::IsMouseClicked(0, elements_count, false))
					return_bool = true;
			}
			functions::fill_box(Vector4(pos.x - 2, pos.y - 2, size.x + 4, size.y + 4), tabButtonBorder);
			if (active)
			{
				functions::fill_box(Vector4(pos.x, pos.y, size.x, size.y), tabButtonActive);
			}
			else if (isHovered)
			{
				functions::fill_box(Vector4(pos.x, pos.y, size.x, size.y), tabButtonHover);
			}
			else
			{
				functions::fill_box(Vector4(pos.x, pos.y, size.x, size.y), tabButtonBackground);
			}
		}

		if (!sameLine) {
			offset_y += size.y + padding.y;
			offset_y2 += size.y + padding.y;
		}

		//Text
		if (event_ == EventType::Repaint) {
			//esp::render_text(Vector4{ pos.x, pos.y, size.x, size.y }, name, Vector3{ 1, 1, 1 }, true, 14, true);
			functions::label(Vector4(pos.x, pos.y, size.x, size.y), name, Vector4(1, 1, 1, 1), true, 14);
			//canvas->text_wrapper(fvector2d(textPos.x, textPos.y), flinearcolor{ 255,255,255,255 }, true, name);
		}
		sameLine = false;
		last_element_pos = pos;
		last_element_size = size;
		if (first_element_pos.x == 0.0f)
			first_element_pos = pos;

		return return_bool;
	}

	void VertButtonBackground(Vector2 size)
	{
		if (event_ == EventType::Repaint) {
			functions::fill_box(Vector4(menu_pos.x, menu_pos.y, size.x, menuY), vertButtonSidebarBackground);
		}
	}

	bool VertButtonTab(std::string name, Vector2 size, bool active)
	{
		elements_count++;
		bool return_bool = false;
		Vector2 padding = Vector2{ 0, 0 };
		Vector2 pos = Vector2{ menu_pos.x + padding.x, menu_pos.y + padding.y + offset_y };

		if (sameCol)
		{
			pos.x = last_element_pos.x;
			pos.y = last_element_pos.y + last_element_size.y + padding.y;

		}

		if (event_ == EventType::Repaint) {
			bool isHovered = MouseInZone(Vector2{ pos.x, pos.y }, Vector2{ size.x, size.y });
			if (isHovered) {
				hover_element = true;
				if (Input::IsMouseClicked(0, elements_count, false))
					return_bool = true;
			}

			if (active)
			{
				//outline
				functions::fill_box(Vector4(pos.x, pos.y, size.x - 2, size.y - 2), vertButtonBorder);
				//active tab
				functions::fill_box(Vector4(pos.x, pos.y, size.x, size.y), vertButtonActive);

			}
			else if (isHovered)
			{
				//hovered tab
				functions::fill_box(Vector4(pos.x, pos.y, size.x, size.y), vertButtonHover);
			}
			else
			{
				//main fill
				functions::fill_box(Vector4(pos.x, pos.y, size.x, size.y), vertButtonBackground);
			}
		}
		if (!sameCol) {
			offset_x += size.x + padding.x;
			offset_x2 += size.x + padding.x;
		}

		//Text
		if (event_ == EventType::Repaint) {
			//esp::render_text(Vector4{ pos.x, pos.y, size.x, size.y }, name, Vector3{ 1, 1, 1 }, true, 14, true);
			functions::label(Vector4(pos.x, pos.y, size.x, size.y), name, Vector4(1, 1, 1, 1), true, 14);
			//canvas->text_wrapper(fvector2d(textPos.x, textPos.y), flinearcolor{ 255,255,255,255 }, true, name);
		}


		sameCol = false;
		last_element_pos = pos;
		last_element_size = size;
		if (first_element_pos.y == 0.0f)
			first_element_pos = pos;

		return return_bool;
	}

	int subTab(const std::vector<std::string>& tabs, int& selectedTabIndex)
	{
		elements_count++;
		Vector2 size = { 55, 24 };
		int returnIndex = -1;
		Vector2 padding = Vector2{ 14, 10 };

		// Calculate the total width of all tabs
		float totalTabsWidth = tabs.size() * (size.x + padding.x);

		// Calculate the starting x-position to center the tabs within the menu window
		float startX = ((menu_pos.x + (padding.x / 2)) + 275) - (tabs.size() * (size.x + padding.x) / 2) + offset_x + 12;  // Added offset_x for horizontal positioning

		// Iterate through the tabs and draw them
		float x_offset = 0;

		for (int i = 0; i < tabs.size(); ++i)
		{
			Vector2 pos = Vector2{ startX + x_offset, menu_pos.y + offset_y + padding.y };

			x_offset = x_offset + size.x + padding.x;
			// Check if the current tab is being hovered over
			bool isHovered = MouseInZone(Vector2{ pos.x, pos.y }, size);

			// Draw the tab
			if (event_ == EventType::Repaint)
			{
				functions::fill_box(Vector4(pos.x - 2, pos.y - 2, size.x + 4, size.y + 4), subTabBorder);
				if (isHovered)
				{
					functions::fill_box({ pos.x, pos.y, size.x, size.y }, subTabHover);
					hover_element = true;
					if (Input::IsMouseClicked(0, elements_count, false))
					{
						returnIndex = i;
					}
				}
				else
				{
					if (selectedTabIndex == i) {
						functions::fill_box({ pos.x, pos.y, size.x, size.y }, subTabActive);
					}
					else {
						functions::fill_box({ pos.x, pos.y, size.x, size.y }, subTabBackground);
					}
				}

				functions::label({ pos.x, pos.y - 1, size.x, size.y }, tabs[i], { 1, 1, 1, 1 }, true, 14);
			}
		}

		offset_y += size.y + padding.y;  // Update vertical offset for the next element

		return returnIndex;
	}

	void HSVtoRGB(float hue, float saturation, float value, float& r, float& g, float& b) {
		int hi = static_cast<int>(floor(hue / 60.0f)) % 6;
		float f = hue / 60.0f - static_cast<float>(hi);
		float p = value * (1.0f - saturation);
		float q = value * (1.0f - f * saturation);
		float t = value * (1.0f - (1.0f - f) * saturation);

		switch (hi) {
		case 0:
			r = value;
			g = t;
			b = p;
			break;
		case 1:
			r = q;
			g = value;
			b = p;
			break;
		case 2:
			r = p;
			g = value;
			b = t;
			break;
		case 3:
			r = p;
			g = q;
			b = value;
			break;
		case 4:
			r = t;
			g = p;
			b = value;
			break;
		case 5:
			r = value;
			g = p;
			b = q;
			break;
		}
	}

	void Colorpicker(std::string name, Vector4* value, bool second, bool do_inline)
	{
		elements_count++;
		float second_window = 0;

		float size = 14;
		Vector2 padding = Vector2{ 10, 10 };
		// Calculate position
		Vector2 pos = Vector2{ menu_pos.x + padding.x + offset_x + (second ? 370 : 0), menu_pos.y + padding.y + (second ? offset_y2 : offset_y) };
		if (do_inline) { //last_width
			pos.x = last_width;
			pos.y = last_height;
		}
		if (sameLine)
		{
			pos.x = last_element_pos.x + last_element_size.x + padding.x;
			pos.y = last_element_pos.y;
		}
		if (checkboxPad)
		{
			pos.y += 35;  // Or whatever offset you want to add
			if (!do_inline) {
				if (second) {
					offset_y2 += 35;  // Also update the corresponding offset
				}
				else {
					offset_y += 35;  // Also update the corresponding offset
				}
			}
			checkboxPad = false;
		}
		
		if (event_ == EventType::Repaint) {
			bool isHovered = MouseInZone(Vector2{ pos.x, pos.y }, Vector2{ size, size });
			functions::fill_box(Vector4(pos.x - 2, pos.y - 2, size + 4, size + 4), checkboxBorder);
			functions::fill_box(Vector4(pos.x, pos.y, size, size), Vector4{ value->x, value->y, value->z, 1 });

			if (isHovered)
			{
				if (functions::get_key(KeyCode::LeftControl) && functions::get_key(KeyCode::C)) {
					saved_color.x = value->x;
					saved_color.y = value->y;
					saved_color.z = value->z;
					saved_color.w = value->w;
				}
				if (functions::get_key(KeyCode::LeftControl) && functions::get_key(KeyCode::V)) {
					value->x = saved_color.x;
					value->y = saved_color.y;
					value->z = saved_color.z;
					value->w = saved_color.w;
				}
				hover_element = true;
				if (Input::IsMouseClicked(0, elements_count, false)) {
					if (color_picker == "") {
						inputBlocker++;
						color_picker = name;
						float r = value->x;
						float g = value->y;
						float b = value->z;

						float maxComponent = r;
						float minComponent = r;

						if (g > maxComponent) maxComponent = g;
						if (b > maxComponent) maxComponent = b;

						if (g < minComponent) minComponent = g;
						if (b < minComponent) minComponent = b;

						float hue = 0.0f;
						float maxMinDiff = maxComponent - minComponent;

						if (maxMinDiff != 0.0f) {
							if (maxComponent == r) {
								hue = 60.0f * (0 + (g - b) / maxMinDiff);
							}
							else if (maxComponent == g) {
								hue = 60.0f * (2.0f + (b - r) / maxMinDiff);
							}
							else if (maxComponent == b) {
								hue = 60.0f * (4.0f + (r - g) / maxMinDiff);
							}
						}

						if (hue < 0) {
							hue += 360.0f;
						}

						hues = hue;
					}
				}
			}
			if (!do_inline) {
				if (second) {
					offset_y2 += size + padding.y;
				}
				else {
					offset_y += size + padding.y;
				}
			}

			//Text
			Vector2 textPos = Vector2{ pos.x + size + 5.0f, pos.y - 3 };
			if (!do_inline) {
				functions::label(Vector4(textPos.x, textPos.y, 150, 23), name, Vector4(1, 1, 1, 1), false, 12);
			}
			Vector2 cursor_pos = CursorPos();

			if (color_picker == name) {
				postOperations.push_back(Entry(DrawOperationType::FillBox, Vector4(pos.x + size + 5.0f, pos.y - 2, 133, 132), Vector4{ 0, 0, 0, 1 }));
				//functions::fill_box(Vector4(pos.x + size + 5.0f, pos.y - 2, 133, 132), Vector4{ 0, 0, 0, 1 }); // Clear the saturation selection box

				// Draw an outline for the saturation selection box
				postOperations.push_back(Entry(DrawOperationType::FillBox, Vector4(pos.x + size + 5.0f - 1, pos.y - 3 - 1, 133, 134), checkboxBorder));
				//functions::fill_box(Vector4(pos.x + size + 5.0f - 1, pos.y - 3 - 1, 133, 134), checkboxborder);

				// Draw the opacity slider
				Vector2 opacity_pos = { pos.x + size + 5.0f, pos.y + 135 };
				postOperations.push_back(Entry(DrawOperationType::FillBox, Vector4(opacity_pos.x, opacity_pos.y, 132, 2), dividerColor));
				//functions::fill_box(Vector4(opacity_pos.x, opacity_pos.y, 132, 2), dividerColor);

				// Calculate the range and position for the opacity slider
				float opacity_range = 1 - 0;
				float opacity_newValue = (value->w - 0) / opacity_range * 132;

				if (opacity_newValue < 0) opacity_newValue = 0;
				if (opacity_newValue > 132) opacity_newValue = 132;

				// Check if the cursor is over the opacity slider
				postOperations.push_back(Entry(DrawOperationType::FillBox, Vector4(opacity_pos.x, opacity_pos.y, 132, 23), Vector4{ checkboxBorder.x, checkboxBorder.y, checkboxBorder.z, value->w }));
				bool opacityHovered = MouseInZone(Vector2{ opacity_pos.x - 5, opacity_pos.y }, Vector2{ 132 + 5, 23 });
				if (opacityHovered) {
					hover_element = true;
					if (functions::get_key(KeyCode::Mouse1)) {
						// Update opacity based on cursor position
						float normalizedOpacity = (cursor_pos.x - opacity_pos.x) / 132;
						if (normalizedOpacity < 0) normalizedOpacity = 0;
						if (normalizedOpacity > 1) normalizedOpacity = 1;
						value->w = 0 + normalizedOpacity * opacity_range;
					}
				}
				bool global_hovered = MouseInZone(Vector2{ pos.x + size + 5.0f - 12, pos.y - 2 - 12 }, Vector2{ 133 + 47, 132 + 47 });

				// Draw the opacity slider knob
				postOperations.push_back(Entry(DrawOperationType::FillBox, Vector4(opacity_pos.x + opacity_newValue - 5, opacity_pos.y - 2, 4, 23), Vector4{ 20, 20, 20, 255 / 255 }));
				//functions::fill_box(Vector4(opacity_pos.x + opacity_newValue - 5, opacity_pos.y - 2, 4, 23), Vector4{ 20, 20, 20, 255 / 255 });

				// Draw the hue slider
				Vector2 hue_pos = { pos.x + size + 5.0f + 135, (pos.y - 2) };
				postOperations.push_back(Entry(DrawOperationType::FillBox, Vector4(hue_pos.x, hue_pos.y - 2, 2, 132), dividerColor));
				//functions::fill_box(Vector4(hue_pos.x, hue_pos.y - 2, 2, 132), dividerColor);

				// Calculate the range and position for the hue slider
				float hue_range = 360.0f - 0.0f;
				float hue_newValue = (hues - 0.0f) / hue_range * 132;

				if (hue_newValue < 0) hue_newValue = 0;
				if (hue_newValue > 132) hue_newValue = 132;

				// Draw the hue slider background gradient
				for (int i = 0; i < 132; i++) {
					float normalizedHue = i / 132.0f;
					float r, g, b;
					HSVtoRGB(normalizedHue * 360.0f, 1.0f, 1.0f, r, g, b);
					//functions::set_draw_color(Vector4{ r, g, b, 1 });
					//functions::draw_pixel(static_cast<int>(hue_pos.x), static_cast<int>(hue_pos.y + i));
					//postOperations.push_back(Entry(Vector4(static_cast<int>(hue_pos.x), static_cast<int>(hue_pos.y + i), 1000000, 1000000), Vector4{ r, g, b, 1 }));
					//functions::fill_box(Vector4(hue_pos.x, hue_pos.y + i, 23, 1), Vector4{ r, g, b, 1 });

					postOperations.push_back(Entry(DrawOperationType::FillBox, Vector4(hue_pos.x, hue_pos.y + i, 23, 1), Vector4{ r, g, b, 1 }));
				}

				// Draw the hue slider knob
				//functions::fill_box(Vector4(hue_pos.x, hue_pos.y + hue_newValue, 23, 4), Vector4{ 20, 20, 20, 255 / 255 });
				postOperations.push_back(Entry(DrawOperationType::FillBox, Vector4(hue_pos.x, hue_pos.y + hue_newValue, 23, 4), Vector4{ 20, 20, 20, 255 / 255 }));

				// Check if the cursor is over the hue slider
				bool hueSliderHovered = MouseInZone(Vector2{ hue_pos.x, hue_pos.y - 5 }, Vector2{ 23, 132 + 5 });
				if (hueSliderHovered) {
					if (functions::get_key(KeyCode::Mouse1)) {
						// Update hue based on cursor position
						float normalizedHue = (cursor_pos.y - hue_pos.y) / 132;
						if (normalizedHue < 0) normalizedHue = 0;
						if (normalizedHue > 1) normalizedHue = 1;
						hues = 0.0f + normalizedHue * hue_range;
					}
				}

				// Draw saturation slider
				postOperations.push_back(Entry(DrawOperationType::FillBox, Vector4(pos.x + size + 5.0f, pos.y - 2, 132, 132), Vector4{ value->x, 1, 1, 1 }));
				//functions::fill_box(Vector4(pos.x + size + 5.0f, pos.y - 2, 132, 132), Vector4{ value->x, 1, 1, 1 });

				bool rgb_hovered = false;

				int colorBoxSize = 6; // Size of each color box
				int numBoxesPerRow = 132 / colorBoxSize; // Number of boxes in a row

				for (int y = 0; y < numBoxesPerRow; y++) {
					for (int x = 0; x < numBoxesPerRow; x++) {
						// Calculate the position of the current color box
						int posX = static_cast<int>(pos.x + size + 5.0f) + x * colorBoxSize;
						int posY = static_cast<int>(pos.y - 2) + y * colorBoxSize;

						// Calculate the color for this box based on HSV
						float hue = hues;
						float saturation = static_cast<float>(x) / (numBoxesPerRow - 1); // Normalize saturation
						float values = 1.0f - static_cast<float>(y) / (numBoxesPerRow - 1); // Normalize value
						float r, g, b;
						HSVtoRGB(hue, saturation, values, r, g, b);

						// Draw the color box
						postOperations.push_back(Entry(DrawOperationType::FillBox, Vector4(posX, posY, colorBoxSize, colorBoxSize), Vector4{ r, g, b, 1 }));

						bool isSelected = (r == value->x && g == value->y && b == value->z);
						if (isSelected) {
							postOperations.push_back(Entry(DrawOperationType::FillBox, Vector4(posX, posY, colorBoxSize, colorBoxSize), Vector4{ 0.8f, 0.8f, 0.8f, 0.7f }));
						}


						// Check for click events
						if (cursor_pos.x >= posX && cursor_pos.x <= posX + colorBoxSize &&
							cursor_pos.y >= posY && cursor_pos.y <= posY + colorBoxSize) {
							rgb_hovered = true;

							if (functions::get_key(KeyCode::Mouse1)) {
								// Set the selected color
								value->x = r;
								value->y = g;
								value->z = b;

							}
						}
					}
				}
				if (!opacityHovered && !hueSliderHovered && !rgb_hovered && !global_hovered) {
					if (Input::IsMouseClicked(0, elements_count, false)) {
						color_picker = "";
						inputBlocker--;
					}
				}
				else {
					hover_element = true;
				}
				if (functions::get_key(KeyCode::Escape) || functions::get_key(KeyCode::Insert)) {
					color_picker = "";
					inputBlocker--;
				}
			}
		}

		sameLine = false;
		last_element_pos = pos;
		//last_element_size = size;
		if (first_element_pos.x == 0.0f)
			first_element_pos = pos;
	}

	void DropDown(std::string name, const std::vector<std::string>& options, std::unordered_map<std::string, bool>& dropdownState, bool second, bool do_inline) {
		elements_count++;
		float second_window = 0;

		float size = 20;
		Vector2 padding = Vector2{ 10, 10 };
		// Calculate position
		//Vector2 pos = Vector2{ menu_pos.x + padding.x + offset_x + (second ? 370 : 0), menu_pos.y + padding.y + (second ? offset_y2 : offset_y) };
		Vector2 pos = Vector2{ menu_pos.x + ((padding.x*3) - 1) + offset_x + (second ? 370 : 0), menu_pos.y + padding.y + (second ? offset_y2 : offset_y) };
		Vector2 textPos = { pos.x + 5, pos.y + 2 };
		if (do_inline) {
			pos.x = last_width;
			pos.y = last_height;
		}
		if (sameLine)
		{
			pos.x = last_element_pos.x + last_element_size.x + padding.x;
			pos.y = last_element_pos.y;

		}
		if (checkboxPad) //////
		{
			pos.y += 30;  // Or whatever offset you want to add
			if (!do_inline) {
				if (second) {
					offset_y2 += 30;  // Also update the corresponding offset
				}
				else {
					offset_y += 30;  // Also update the corresponding offset
				}
			}
			checkboxPad = false;
		}
		else {
			if (second) {
				offset_y2 += size + padding.y;  // Also update the corresponding offset
			}
			else {
				offset_y += size + padding.y;  // Also update the corresponding offset
			}
		}


		
		if (event_ == EventType::Repaint) {
			float option_height = 20;
			// Draw dropdown box border
			functions::fill_box(Vector4(pos.x - 1, pos.y - 1, 122, size + 2), dropdownBorder);
			// Draw main dropdown box
			functions::fill_box(Vector4(pos.x, pos.y, 120, size), dropdownBackground);
			// Draw darker square at the end of the dropdown box
			functions::fill_box(Vector4(pos.x + 100, pos.y, 20, size), dropdownBackgroundBox);
			// Draw arrow symbol
			if (dropdownState["_open"]) {
				drawUpTriangle(pos.x + 109, pos.y + 6, 8, triangleColor);  // Adjust the position and size as needed
			}
			else {
				drawDownTriangle(pos.x + 109, pos.y + 6, 8, triangleColor);  // Adjust the position and size as needed
			}
			// Draw text label
			functions::label(Vector4(textPos.x, textPos.y - 2, 120, 23), name, dropdownText, false, 12);

			// Check if dropdown should be open
			bool isHovered = MouseInZone(Vector2{ pos.x, pos.y }, Vector2{ 120, 20 });
			if (isHovered && Input::IsMouseClicked(0, elements_count, false)) {
				bool wasOpen = dropdownState["_open"];
				dropdownState["_open"] = !dropdownState["_open"];
				inputBlocker += (dropdownState["_open"] != wasOpen) ? (dropdownState["_open"] ? 1 : -1) : 0;
			}

			// If dropdown is open, display options
			if (dropdownState["_open"]) {
				for (size_t i = 0; i < options.size(); ++i) {
					Vector2 option_pos = { pos.x, pos.y + (i + 1) * size };
					postOperations.push_back(Entry(DrawOperationType::FillBox, Vector4(option_pos.x - 1, option_pos.y, 122, size), dropdownOptionsBorder));
					postOperations.push_back(Entry(DrawOperationType::FillBox, Vector4(option_pos.x, option_pos.y, 120, size), dropdownOptionsBackground));

					// Check if this option is hovered or clicked
					bool optionHovered = MouseInZone(Vector2{ option_pos.x, option_pos.y }, Vector2{ 120, size });
					if (optionHovered) {
						postOperations.push_back(Entry(DrawOperationType::FillBox, Vector4(option_pos.x, option_pos.y, 120, size - 1), dropdownOptionsHover));
						if (Input::IsMouseClicked(0, elements_count, false)) {
							// Toggle option state
							dropdownState[options[i]] = !dropdownState[options[i]];
						}
					}

					// Choose color based on selection state
					Vector4 textColor = dropdownState[options[i]] ? dropdownTextActive : dropdownText;

					// Draw option text
					postOperations.push_back(Entry(DrawOperationType::Label, Vector4(option_pos.x + 5, option_pos.y, 140, option_height), options[i], textColor, false, 12));
				}
			}

			if (dropdownState["_open"]) {
				bool globalHovered = MouseInZone(Vector2{ pos.x, pos.y }, Vector2{ 120, (options.size() * size) + size });
				if (!globalHovered) {
					if (Input::IsMouseClicked(0, elements_count, false)) {
						dropdownState["_open"] = false;
						inputBlocker--;
						hover_element = false;
					}
				}
				else {
					hover_element = true;
				}
			}

			// Close dropdown if Escape or Insert is pressed
			if (functions::get_key(KeyCode::Escape) || functions::get_key(KeyCode::Insert)) {
				//inputBlocker--;
				if (dropdownState["_open"]) {
					dropdownState["_open"] = false;
					inputBlocker--;
					hover_element = false;
				}
			}
		}

		sameLine = false;
		last_element_pos = Vector2(pos.x, pos.y - 5);
		//last_element_size = size;
		if (first_element_pos.x == 0.0f);
			first_element_pos = pos;
	}


	bool runonce = true;
	void Logo() {
		elements_count++;
		float second_window = 0;

		float size = 14;
		Vector2 padding = Vector2{ 10, 10 };
		// Calculate position
		Vector2 pos = Vector2{ menu_pos.x + padding.x + offset_x, menu_pos.y + padding.y };
		Vector2 textPos = { pos.x + 5, pos.y + 2 }; // Define textPos here


		if (event_ == EventType::Repaint && runonce) {
			//functions::draw_texture({ pos.x, pos.y, 50.0f, 50.0f }, (uintptr_t)xene_logo);
			auto texture = functions::get_white_texture();
			uint8_t* textureData = reinterpret_cast<uint8_t*>(texture);

			for (size_t i = 0; i < 16; i++) {
				//printf("0x%02X ", textureData[i]);
			}

			runonce = false;
		}

		sameLine = false;
		last_element_pos = pos;
		//last_element_size = size;
		if (first_element_pos.x == 0.0f)
			first_element_pos = pos;
	}

	std::string active_textbox;
	bool textbox_found_key = false;
	KeyCode textbox_active_key = KeyCode::Unbound;
	//std::clock_t textbox_last = global::getCurrentTimeMillis();

	void TextBox(std::string name, std::string& text_input, bool second)
	{
		elements_count++;
		float second_window = 0;

		float width = 140;
		float height = 20;
		Vector2 padding = Vector2{ 10, 10 };

		Vector2 pos = Vector2{ menu_pos.x + padding.x + offset_x + (second ? 370 : 0), (menu_pos.y + padding.y + (second ? offset_y2 : offset_y) + 3) };

		if (sameLine)
		{
			pos.x = last_element_pos.x + last_element_size.x + padding.x;
			pos.y = last_element_pos.y;
		}

		if (checkboxPad)
		{
			pos.y += 35;
			if (second) {
				offset_y2 += 35;
			}
			else {
				offset_y += 35;
			}
			checkboxPad = false;
		}

		if (event_ == EventType::Repaint) {
			bool isHovered = MouseInZone(Vector2{ pos.x, pos.y + 25 }, Vector2{ width, 20 });

			if (isHovered && inputBlocker == 0) {
				//functions::fill_box(Vector4(text_box_pos.x + 2, text_box_pos.y, width - 4, 20), windowBackground);
				hover_element = true;
				if (Input::IsMouseClicked(0, elements_count, false)) {
					active_textbox = name;
					//vars = str;
				}
			}
			if (!isHovered) {
				if (active_textbox != "" && active_textbox == name) {
					if (functions::get_key(KeyCode::Escape) || functions::get_key(KeyCode::Insert) || Input::IsMouseClicked(0, elements_count, false)) {
						active_textbox = "";
						textbox_found_key = false;
					}
				}
			}

			if (active_textbox == name) {
				//functions::label(Vector4(pos.x - 3, pos.y - 3, 156, 23), name, checkboxBorder, false, 12);
				auto currentTime = global::getCurrentTimeMillis();
				functions::fill_box(Vector4(pos.x - 2, pos.y + 23, width + 4, height + 4), sliderBackground);
				//if (currentTime > (textbox_last + 4)) {
					int minAlphaKeyCode = static_cast<int>(KeyCode::A);  // 97
					int maxAlphaKeyCode = static_cast<int>(KeyCode::Z);  // 122
					//textbox_found_key = false;
					bool active_key = false;
					KeyCode found_key;

					for (int i = minAlphaKeyCode; i <= maxAlphaKeyCode; ++i) {
						KeyCode key = static_cast<KeyCode>(i);
						//std::cout << "looping keycodes" << std::endl;
						if (functions::get_key(key)) {
							active_key = true;
							found_key = key;
							break;
						}
					}

					if (!active_key) {
						int minNumKeyCode = static_cast<int>(KeyCode::Alpha0);  // 48
						int maxNumKeyCode = static_cast<int>(KeyCode::Alpha9);  // 57

						for (int i = minNumKeyCode; i <= maxNumKeyCode; ++i) {
							KeyCode key = static_cast<KeyCode>(i);
							//std::cout << "looping keycodes" << std::endl;
							if (functions::get_key(key)) {
								active_key = true;
								found_key = key;
								break;
							}
						}
					}

					if (textbox_active_key == KeyCode::Backspace) {
						if (!functions::get_key(KeyCode::Backspace)) {
							textbox_active_key = KeyCode::Asterisk;
						}
					}
					if (textbox_active_key == KeyCode::Space) {
						if (!functions::get_key(KeyCode::Space)) {
							textbox_active_key = KeyCode::Asterisk;
						}
					}

					if (found_key != textbox_active_key) {
						if (active_key) {
							if (13 > text_input.size()) {
								text_input = text_input + global::to_lowercase(keyCodeToString[found_key]);
							}
							textbox_active_key = found_key;
						}
						else {
							//std::cout << "aaaaa" << std::endl;
							if (functions::get_key(KeyCode::Space) && textbox_active_key != KeyCode::Space) {
								if (13 > text_input.size()) {
									text_input = text_input + " ";
									textbox_active_key = KeyCode::Space;
								}
							}
							else if (functions::get_key(KeyCode::Backspace) && textbox_active_key != KeyCode::Backspace) {
								if (!text_input.empty()) {
									text_input = text_input.substr(0, text_input.size() - 1);
									std::cout << "backspace" << std::endl;
									textbox_active_key = KeyCode::Backspace;
								}
							}
							else {
								if (textbox_active_key != KeyCode::Backspace && textbox_active_key != KeyCode::Space) {
									textbox_active_key = KeyCode::Asterisk;
								}
							}
						}
					}
					//textbox_last = currentTime;
				//}
			}
			else {
				//functions::label(Vector4(pos.x - 3, pos.y - 3, 156, 23), name, sliderBackground, false, 12);
				functions::fill_box(Vector4(pos.x - 2, pos.y + 23, width + 4, height + 4), checkboxBorder);
			}

			// Draw the name text
			functions::label(Vector4(pos.x, pos.y + 5, 500, 23), name, Vector4(1, 1, 1, 1), false, 12);

			// Draw the text box
			functions::fill_box(Vector4(pos.x, pos.y + 25, width, height), vertButtonSidebarBackground);

			// You can add more logic here to handle text input into the box
			// For now, let's just display whatever is in text_input
			functions::label(Vector4(pos.x + 5, pos.y + 25, width - 10, height), text_input + "_", Vector4(1, 1, 1, 1), false, 12);

			if (!sameLine) {
				if (second) {
					offset_y2 += height + padding.y + 25;  // added 25 to include the name's height
				}
				else {
					offset_y += height + padding.y + 25;  // added 25 to include the name's height
				}
			}
		}

		sameLine = false;
		last_element_pos = pos;
		last_element_size = Vector2{ width, height + 25 };  // added 25 to include the name's height
		if (first_element_pos.x == 0.0f)
			first_element_pos = pos;
	}

	void BigRectangleWithStrings(std::string name, std::vector<std::string>& string_array, bool second, std::string& vars)
	{
		elements_count++;
		float second_window = 0;

		float width = 245;
		float height = 310;
		Vector2 padding = Vector2{ 10, 10 };

		Vector2 pos = Vector2{ menu_pos.x + padding.x + offset_x + (second ? 370 : 0), (menu_pos.y + padding.y + (second ? offset_y2 : offset_y)) + 15 };

		if (sameLine)
		{
			pos.x = last_element_pos.x + last_element_size.x + padding.x;
			pos.y = last_element_pos.y;
		}

		if (checkboxPad)
		{
			pos.y += 35;
			if (second) {
				offset_y2 += 35;
			}
			else {
				offset_y += 35;
			}
			checkboxPad = false;
		}

		if (event_ == EventType::Repaint) {
			functions::fill_box(Vector4(pos.x - 3, pos.y - 3, width + 6, height + 6), windowBorder);
			functions::fill_box(Vector4(pos.x, pos.y, width, height), vertButtonSidebarBackground);

			float box_offset_y = 2;
			for (const auto& str : string_array) {
				Vector2 text_box_pos = { pos.x, pos.y + box_offset_y };
				bool isHovered = MouseInZone(Vector2{ text_box_pos.x + 2, text_box_pos.y }, Vector2{ width - 4, 20 });

				if (isHovered && inputBlocker == 0) {
					hover_element = true;
					functions::fill_box(Vector4(text_box_pos.x + 2, text_box_pos.y, width - 4, 20), windowBackground);
					if (Input::IsMouseClicked(0, elements_count, false)) {
						vars = str;
					}
				}
				if (str == vars) {
					functions::fill_box(Vector4(text_box_pos.x + 2, text_box_pos.y, width - 4, 20), windowBackground);
				}

				functions::label(Vector4(text_box_pos.x + 8, text_box_pos.y, 150, 23), str, Vector4(1, 1, 1, 1), false, 12);
				box_offset_y += 20;
			}

			if (!sameLine) {
				if (second) {
					offset_y2 += height + padding.y + 5;
				}
				else {
					offset_y += height + padding.y + 5;
				}
			}

			Vector2 textPos = Vector2{ pos.x, pos.y - 10 };
			functions::label(Vector4(textPos.x, textPos.y - 12, 150, 23), name, Vector4(1, 1, 1, 1), false, 12);
		}

		sameLine = false;
		last_element_pos = pos;
		last_element_size = Vector2{ width, height };
		if (first_element_pos.x == 0.0f)
			first_element_pos = pos;
	}

	void Checkbox(std::string name, bool* value, bool second, bool ignore_colorpicker)
	{
		elements_count++;
		float second_window = 0;

		float size = 14;
		Vector2 padding = Vector2{ 10, 10 };
		// Calculate position
		Vector2 pos = Vector2{ menu_pos.x + padding.x + offset_x + (second ? 370 : 0), menu_pos.y + padding.y + (second ? offset_y2 : offset_y) };
		if (sameLine)
		{
			pos.x = last_element_pos.x + last_element_size.x + padding.x;
			pos.y = last_element_pos.y;
		}
		if (checkboxPad)
		{
			pos.y += 35;  // Or whatever offset you want to add
			if (second) {
				offset_y2 += 35;  // Also update the corresponding offset
			}
			else {
				offset_y += 35;  // Also update the corresponding offset
			}
			checkboxPad = false;
		}
		
		if (event_ == EventType::Repaint) {
			bool isHovered = MouseInZone(Vector2{ pos.x, pos.y }, Vector2{ size, size });
			functions::fill_box(Vector4(pos.x - 2, pos.y - 2, size + 4, size + 4), checkboxBorder);
			if (isHovered && inputBlocker == 0)
			{
				hover_element = true;
				functions::fill_box(Vector4(pos.x, pos.y, size, size), checkboxHover);
				if (Input::IsMouseClicked(0, elements_count, false))
					*value = !*value;

				//drawFilledRect(fvector2d{ pos.x, pos.y }, size, size, flinearcolor{ 0.08, 0.08, 0.08 ,255 });

			}
			else
			{
				functions::fill_box(Vector4(pos.x, pos.y, size, size), Vector4{ 1 , 1 , 1 , 1 });

				//drawFilledRect(fvector2d{ pos.x, pos.y }, size, size, flinearcolor{ 0.08, 0.08, 0.08 ,255 });
			}

			if (!sameLine) {
				if (second) {
					offset_y2 += size + padding.y;
				}
				else {
					offset_y += size + padding.y;
				}
			}

			if (*value)
			{
				functions::fill_box(Vector4(pos.x + 3, pos.y + 3, size - 6, size - 6), checkboxActive);

				//drawFilledRect(fvector2d{ pos.x + 3, pos.y + 3 }, size - 6, size - 6, flinearcolor{ 0.27, 0.00, 0.43,255 });
				//drawFilledRect(fvector2d{ pos.x + 9, pos.y + 9 }, size - 18, size - 18, Colors::Checkbox_Hovered);
			}



			//Text
			Vector2 textPos = Vector2{ pos.x + size + 5.0f, pos.y - 3 };
			//if (!TextOverlapedFromActiveElement(textPos))
			//TextLeft(name, textPos, flinearcolor{ 1.0f, 1.0f, 1.0f, 1.0f }, false);

			last_width = pos.x + size + 5;
			last_height = pos.y;
			if (!ignore_colorpicker) {
				textPos.x = textPos.x + size + 5;
			}
			functions::label(Vector4(textPos.x, textPos.y, 150, 23), name, Vector4(1, 1, 1, 1), false, 12);

			//canvas->text_wrapper(Vector2(textPos.x, textPos.y), flinearcolor{ 255,255,255,255 }, false, name);

		}

		sameLine = false;
		last_element_pos = pos;
		//last_element_size = size;
		if (first_element_pos.x == 0.0f)
			first_element_pos = pos;
	}

	void DangerCheckbox(std::string name, bool* value, bool second, bool ignore_colorpicker)
	{
		elements_count++;
		float second_window = 0;

		float size = 14;
		Vector2 padding = Vector2{ 10, 10 };
		// Calculate position
		Vector2 pos = Vector2{ menu_pos.x + padding.x + offset_x + (second ? 370 : 0), menu_pos.y + padding.y + (second ? offset_y2 : offset_y) };
		if (sameLine)
		{
			pos.x = last_element_pos.x + last_element_size.x + padding.x;
			pos.y = last_element_pos.y;
		}
		if (checkboxPad)
		{
			pos.y += 35;  // Or whatever offset you want to add
			if (second) {
				offset_y2 += 35;  // Also update the corresponding offset
			}
			else {
				offset_y += 35;  // Also update the corresponding offset
			}
			checkboxPad = false;
		}
		
		if (event_ == EventType::Repaint) {
			bool isHovered = MouseInZone(Vector2{ pos.x, pos.y }, Vector2{ size, size });
			functions::fill_box(Vector4(pos.x - 2, pos.y - 2, size + 4, size + 4), dangerCheckboxBorder);
			if (isHovered && inputBlocker == 0)
			{
				hover_element = true;
				functions::fill_box(Vector4(pos.x, pos.y, size, size), dangerCheckboxHover);

				//Hover Popup
				Vector2 dangertextPos = Vector2{ pos.x + 10, pos.y - 20 };

				functions::fill_box(Vector4(pos.x + 6, pos.y - 21, 50, 18), dangerCheckboxPopupBorder);
				functions::fill_box(Vector4(pos.x + 7, pos.y - 20, 48, 16), dangerCheckboxPopupBackground);
				functions::label(Vector4(dangertextPos.x, dangertextPos.y - 2, 150, 23), "Unsafe!", dangerCheckboxTextPopup, false, 12);

				if (Input::IsMouseClicked(0, elements_count, false))
					*value = !*value;
			}
			else
			{
				functions::fill_box(Vector4(pos.x, pos.y, size, size), Vector4{ 1 , 1 , 1 , 1 });

			}

			if (!sameLine) {
				if (second) {
					offset_y2 += size + padding.y;
				}
				else {
					offset_y += size + padding.y;
				}
			}

			if (*value)
			{
				functions::fill_box(Vector4(pos.x + 3, pos.y + 3, size - 6, size - 6), dangerCheckboxActive);
			}



			//Text
			Vector2 textPos = Vector2{ pos.x + size + 5.0f, pos.y - 3 };

			last_width = pos.x + size + 5;
			last_height = pos.y;
			if (!ignore_colorpicker) {
				textPos.x = textPos.x + size + 5;
			}
			functions::label(Vector4(textPos.x, textPos.y, 150, 23), name, dangerCheckboxText, false, 12);

		}

		sameLine = false;
		last_element_pos = pos;
		//last_element_size = size;
		if (first_element_pos.x == 0.0f)
			first_element_pos = pos;
	}

	void Divider(std::string name, bool second)
	{
		elements_count++;
		float second_window = 0;

		float size = 150 - (name.size() * 7.5f);
		Vector2 padding = Vector2{ 10, 21 };
		// Calculate position
		Vector2 pos = Vector2{ menu_pos.x + padding.x + offset_x + (second ? 370 : 0) + (name.size() * 7.5f), menu_pos.y + padding.y + (second ? offset_y2 : offset_y) };
		//Vector2 pos2 = Vector2{ menu_pos.x + padding.x + offset_x + (second ? 370 : 0), menu_pos.y + padding.y + (second ? offset_y2 : offset_y) };
		if (sameLine)
		{
			pos.x = last_element_pos.x + last_element_size.x + padding.x;
			pos.y = last_element_pos.y;
			//pos2.x = last_element_pos.x + last_element_size.x + padding.x;
			//pos2.y = last_element_pos.y;
		}
		if (checkboxPad)
		{
			pos.y += 35;
			//pos2.y += 35;// Or whatever offset you want to add
			if (second) {
				offset_y2 += 35;  // Also update the corresponding offset
			}
			else {
				offset_y += 35;  // Also update the corresponding offset
			}
			checkboxPad = false;
		}
		
		if (event_ == EventType::Repaint) {
			functions::fill_box(Vector4(pos.x, pos.y, size, 3), dividerColor);
			//functions::fill_box(Vector4(pos2.x, pos2.y, 10, 3), dividerColor);
			if (!sameLine) {
				if (second) {
					offset_y2 += padding.y + 10;
				}
				else {
					offset_y += padding.y + 10;
				}
			}



			//Text
			Vector2 textPos = Vector2{ pos.x - (name.size() * 7.5f), pos.y - 10.0f };
			functions::label(Vector4(textPos.x, textPos.y, 150, 23), name, dividerTextColor, false, 14);

		}

		sameLine = false;
		last_element_pos = pos;
		last_element_size = Vector2(size, 3);
		if (first_element_pos.x == 0.0f)
			first_element_pos = pos;
	}

	void Slider(std::string name, float* value, bool second, float minValue, float maxValue, bool do_text)
	{
		elements_count++;
		float second_window = 0;

		float size = 140;  // Adjust the size of the slider as needed.
		Vector2 padding;

		if (do_text) {
			padding = Vector2{ 15, 28 };
		}
		else {
			padding = Vector2{ 15, 12 };
		}

		Vector2 pos = Vector2{ menu_pos.x + padding.x + offset_x + (second ? 370 : 0) + 14, menu_pos.y + padding.y + (second ? offset_y2 : offset_y) };

		// Calculate the slider value based on the mouse position
		float range = maxValue - minValue;  // Calculate the range of the slider
		float newValue = (*value - minValue) / range * size;

		if (newValue < 0) newValue = 0;
		if (newValue > size) newValue = size;

		Vector2 cursor_pos = CursorPos();

		if (event_ == EventType::Repaint) {
			bool isHovered = MouseInZone(Vector2{ pos.x - 12, pos.y }, Vector2{ size + 24, 20 });
			functions::fill_box(Vector4(pos.x - 2, pos.y - 2, size + 4, 20 + 4), sliderBorder);
			if (isHovered && inputBlocker == 0) {
				functions::fill_box(Vector4(pos.x, pos.y, size, 20), sliderHover);
				hover_element = true;
				if (functions::get_key(KeyCode::Mouse1)) {
					float normalizedValue = (cursor_pos.x - pos.x) / size;
					if (normalizedValue < 0) normalizedValue = 0;
					if (normalizedValue > 1) normalizedValue = 1;
					*value = minValue + normalizedValue * range;
				}
			}
			else {
				functions::fill_box(Vector4(pos.x, pos.y, size, 20), sliderBackground);
			}


			// Draw the slider handle at the calculated position
			functions::fill_box(Vector4(pos.x + newValue - 5, pos.y, 10, 23), Vector4{ 20, 20, 20, 255 / 255 });

			if (do_text) {
				functions::label(Vector4(pos.x, pos.y - 21, size, 23), name, Vector4(1, 1, 1, 1), false, 12);
			}
			if (second) {
				functions::label(Vector4(((pos.x + newValue) - 7) - (8 * (std::to_string(static_cast<int>(*value)).length())), pos.y, 50, 23), std::to_string(static_cast<int>(*value)), Vector4(1, 1, 1, 1), false, 12);
			}
			else {
				functions::label(Vector4((pos.x + newValue - 5) + 12, pos.y, 50, 23), std::to_string(static_cast<int>(*value)), Vector4(1, 1, 1, 1), false, 12);
			}
		}

		if (second) {
			if (do_text) {
				offset_y2 += 46;
			}
			else {
				offset_y2 += 30;
			}
		}
		else {
			if (do_text) {
				offset_y += 46;
			}
			else {
				offset_y += 30;
			}
		}

		sameLine = false;
		last_element_pos = pos;
		if (first_element_pos.x == 0.0f)
			first_element_pos = pos;
	}

	void Button(std::string name, ButtonCallback callback, bool second)
	{
		elements_count++;
		float second_window = 0;

		Vector2 buttonSize = { 120, 24 };


		Vector2 padding = Vector2{ 10, 14 };
		Vector2 pos = Vector2{ menu_pos.x + padding.x + offset_x + (second ? 370 : 0) + 19, menu_pos.y + padding.y + (second ? offset_y2 : offset_y) };
		if (sameLine)
		{
			pos.x = last_element_pos.x + last_element_size.x + padding.x;
			pos.y = last_element_pos.y;
		}
		
		bool isHovered = MouseInZone(Vector2{ pos.x, pos.y }, Vector2{ buttonSize.x, buttonSize.y });

		if (event_ == EventType::Repaint) {
			//std::cout << "calling" << std::endl;
			functions::fill_box(Vector4(pos.x - 2, pos.y - 2, buttonSize.x + 4, buttonSize.y + 4), buttonBorder);
			if (isHovered && inputBlocker == 0) {
				hover_element = true;
				if (Input::IsMouseClicked(0, elements_count, false)) {
					callback();
				}
				functions::fill_box({ pos.x, pos.y, buttonSize.x, buttonSize.y }, buttonHover);
			}
			else {
				functions::fill_box({ pos.x, pos.y, buttonSize.x, buttonSize.y }, buttonBackground);
			}
			functions::label({ pos.x, pos.y - 1, buttonSize.x, buttonSize.y }, name, { 255, 255, 255, 255 }, true, 14);

			if (!sameLine) {
				if (second) {
					offset_y2 += buttonSize.y + padding.y;
				}
				else {
					offset_y += buttonSize.y + padding.y;
				}
			}

		}
		sameLine = false;
		last_element_pos = pos;

		if (first_element_pos.x == 0.0f)
			first_element_pos = pos;

	}

	void Hotkey(std::string name, std::string element, bool second)
	{
		elements_count++;
		float second_window = 0;

		Vector2 buttonSize = { 120, 24 };


		Vector2 padding = Vector2{ 10, 12 };
		Vector2 pos = Vector2{ menu_pos.x + padding.x + offset_x + (second ? 370 : 0) + 19, menu_pos.y + padding.y + (second ? offset_y2 : offset_y) };
		if (sameLine)
		{
			pos.x = last_element_pos.x + last_element_size.x + padding.x;
			pos.y = last_element_pos.y;
		}
		
		bool isHovered = MouseInZone(Vector2{ pos.x, pos.y }, Vector2{ buttonSize.x, buttonSize.y });
		if (event_ == EventType::Repaint) {
			functions::fill_box(Vector4(pos.x - 2, pos.y - 2, buttonSize.x + 4, buttonSize.y + 4), hotkeyBorder);
			if (isHovered && inputBlocker == 0) {
				hover_element = true;
				if (!searching_hotkey && Input::IsMouseClicked(0, elements_count, false)) {
					hotkey_call = global::getCurrentTimeMillis();
					last_hotkey = element;
					searching_hotkey = true;
				}
				functions::fill_box({ pos.x, pos.y, buttonSize.x, buttonSize.y }, hotkeyHover);
			}
			else {
				functions::fill_box({ pos.x, pos.y, buttonSize.x, buttonSize.y }, hotkeyBackground);
			}
			if (searching_hotkey && last_hotkey == element) {
				functions::label({ pos.x, pos.y - 1, buttonSize.x, buttonSize.y }, "PRESS KEY", {1, 1, 1, 1}, true, 14);
			}
			else {
				functions::label({ pos.x, pos.y - 1, buttonSize.x, buttonSize.y }, name, {1, 1, 1, 1}, true, 14);
			}
			if (!sameLine) {
				if (second) {
					offset_y2 += buttonSize.y + padding.y;
				}
				else {
					offset_y += buttonSize.y + padding.y;
				}
			}

		}
		sameLine = false;
		last_element_pos = pos;
		//last_element_size = size;
		if (first_element_pos.x == 0.0f)
			first_element_pos = pos;

		if (searching_hotkey) {
			get_hotkeys();
		}
	}

	void ComboBox(const std::vector<std::string>& options, int& selectedValue, bool second)
	{
		elements_count++;
		Vector2 buttonSize = { 24, 24 };
		Vector2 padding = Vector2{ 10, 10 };
		Vector2 pos = Vector2{ menu_pos.x + padding.x + offset_x + 14 + (second ? 370 : 0) + 5, menu_pos.y + padding.y + (second ? offset_y2 : offset_y) };

		// Check if the selectedValue is within valid range
		if (selectedValue < 0)
			selectedValue = 0;
		else if (selectedValue >= static_cast<int>(options.size()))
			selectedValue = static_cast<int>(options.size()) - 1;

		if (event_ == EventType::Repaint)
		{
			// "-" button
			bool isMinusButtonHovered = MouseInZone({ pos.x, pos.y }, { buttonSize.x, buttonSize.y });

			// "+" button
			bool isPlusButtonHovered = MouseInZone({ (pos.x - 3) + 100, pos.y, }, { buttonSize.x, buttonSize.y });

			// whole box
			bool isHovered = MouseInZone({ pos.x, pos.y, }, { 120, buttonSize.y });

			if (isHovered && inputBlocker == 0) {
				hover_element = true;
			}

			functions::fill_box(Vector4(pos.x - 2, pos.y - 2, 120 + 4, buttonSize.y + 4), comboboxBorder);
			if ((isMinusButtonHovered || isPlusButtonHovered) && inputBlocker == 0) {
				functions::fill_box({ (pos.x), pos.y, 120, buttonSize.y }, comboboxActive);
			}
			else {
				functions::fill_box({ (pos.x), pos.y, 120, buttonSize.y }, comboboxBackground);
			}


			// Handle button clicks
			if (isMinusButtonHovered && inputBlocker == 0)
			{
				functions::label({ pos.x, pos.y - 1, buttonSize.x, buttonSize.y }, "-", comboboxHover, true, 12);
				if (Input::IsMouseClicked(0, elements_count, false)) {
					if (selectedValue > 0)
						selectedValue--;
				}
			}
			else {
				functions::label({ pos.x, pos.y - 1, buttonSize.x, buttonSize.y }, "-", { 1, 1, 1, 1 }, true, 12);
			}

			if (isPlusButtonHovered && inputBlocker == 0)
			{
				functions::label({ (pos.x - 3) + 100, pos.y - 1, buttonSize.x, buttonSize.y }, "+", comboboxHover, true, 12);
				if (Input::IsMouseClicked(0, elements_count, false)) {
					if (selectedValue < static_cast<int>(options.size()) - 1)
						selectedValue++;
				}
			}
			else {
				functions::label({ (pos.x - 3) + 100, pos.y - 1, buttonSize.x, buttonSize.y }, "+", { 1, 1, 1, 1 }, true, 12);
			}

			// Display the selected option
			functions::label({ (pos.x) + 24, pos.y, 76, buttonSize.y }, options[selectedValue], { 1, 1, 1, 1 }, true, 12);
		}

		if (second) {
			offset_y2 += buttonSize.y + padding.y;
		}
		else {
			offset_y += buttonSize.y + padding.y;
		}
	}


	void ExecutePostOperations() {
		for (const Entry& entry : postOperations) {
			switch (entry.operationType) {
			case DrawOperationType::FillBox:
				functions::fill_box(entry.vector1, entry.vector2);
				break;
			case DrawOperationType::DrawPixel:
				functions::draw_pixel(entry.x, entry.y);
				break;
			case DrawOperationType::Line:
				functions::line(entry.point1, entry.point2, entry.vector2);
				break;
			case DrawOperationType::DrawOutlineCircle:
				functions::draw_outline_circle(entry.x, entry.y, entry.radius, entry.vector2);
				break;
			case DrawOperationType::Label:
				functions::label(entry.vector1, entry.text, entry.vector2, entry.centered, entry.fontSize);
				break;
			case DrawOperationType::Outline:
				functions::outline(entry.vector1, entry.text, entry.vector2, entry.centered, entry.fontSize);
				break;
			case DrawOperationType::OutlineBox:
				functions::outline_box(entry.point1, entry.point2, entry.vector2, entry.girth);
				break;
			case DrawOperationType::DrawTexture:
				functions::draw_texture(entry.vector1, entry.texture);
				break;
			case DrawOperationType::SetDrawColor:
				functions::set_draw_color(entry.vector2);
				break;
			default:
				break;
			}
		}
		postOperations.clear();
	}
}