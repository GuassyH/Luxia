#pragma once

#include <iostream>
#include <Windows.h>
#include <shobjidl.h>
#include <shellapi.h>

namespace Editor::SystemFuncs {
	inline void CopyToClipboard(std::string to_copy) {
		const char* cdata = to_copy.c_str();
		const size_t len = to_copy.size() + 1;
		HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len);
		memcpy(GlobalLock(hMem), cdata, len);
		GlobalUnlock(hMem);
		OpenClipboard(0);
		EmptyClipboard();
		SetClipboardData(CF_TEXT, hMem);
		CloseClipboard();
	}

	inline std::string PasteFromClipboard() {
		std::string result;

		if (!OpenClipboard(nullptr))
			return result;

		HANDLE hData = GetClipboardData(CF_TEXT);
		if (hData)
		{
			char* pszText = static_cast<char*>(GlobalLock(hData));
			if (pszText)
			{
				result = pszText;
				GlobalUnlock(hData);
			}
		}

		CloseClipboard();
		return result;
	}
	
}