#pragma once
#include <pch.h>
enum class CLICK_RESPONSE {
	NONE,
	PROCESSED,
	SELECTED,
	GOT_RIGHT,
	GOT_LEFT
};

struct ClickResponse {
	bool clicked;
	CLICK_RESPONSE type;
};