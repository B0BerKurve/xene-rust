#include "hinclude.h"
#include "global.h"
#include "notifications.h"

namespace notifications
{
	inline std::vector<notificationItem> active_notifications;
	void add_notification(std::string text, float length) {
		notificationItem item_notification;
		item_notification.time = global::getCurrentTimeMillis();
		item_notification.length = length;
		item_notification.text = text;
		active_notifications.push_back(item_notification);
	}
}