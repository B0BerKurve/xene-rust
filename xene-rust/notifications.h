#pragma once
#include <string>

struct notificationItem {
	std::string text;
	float time;
	float length;
};

namespace notifications
{
	extern std::vector<notificationItem> active_notifications;
	void add_notification(std::string text, float length);
}