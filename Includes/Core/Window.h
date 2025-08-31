#pragma once

#include <Windows.h>
#include <string>
#include "Engine.h"
#include "Type.h"

class Engine_API Window
{
	friend class Engine;

public:
	Window(
		HINSTANCE instance,
		WNDPROC messageProcedure,
		const std::wstring& title,
		uint32 width,
		uint32 height
	);

	~Window();

	// Getter/Setter.
	inline const uint32 Width() const { return width; }
	inline const uint32 Height() const { return height; }
	inline HWND Handle() const { return handle; }
	std::wstring Title() const { return title; }

	void SetWidthHeight(uint32 width, uint32 height);

private:
	std::wstring title = nullptr;
	std::wstring className = TEXT("Engine Class");
	uint32 width = 0;
	uint32 height = 0;
	HWND handle = nullptr;
	HINSTANCE instance = nullptr;
};