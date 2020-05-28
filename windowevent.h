#pragma once

class CWindowEvent
{
public:
	enum
	{
		KEY_PRESS,
		KEY_RELEASE,
		MOUSE_MOVE,

	} type;
	union
	{
		struct
		{
			bool lAlt;
			bool rAlt;
			bool lCtrl;
			bool rCtrl;
			int key;
		} keyboard;

		struct
		{
			int x;
			int y;
			int button;
		} mouse;
	};
};