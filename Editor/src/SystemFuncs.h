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
	
	inline std::string OpenFileDialogue() {
		HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
		if (FAILED(hr)) return {};

		IFileOpenDialog* pFileOpen = nullptr;
		hr = CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFileOpen));

		std::wstring filePath;

		if (SUCCEEDED(hr))
		{
			hr = pFileOpen->Show(nullptr);

			if (SUCCEEDED(hr))
			{
				IShellItem* pItem = nullptr;
				hr = pFileOpen->GetResult(&pItem);

				if (SUCCEEDED(hr))
				{
					PWSTR pszFilePath = nullptr;
					hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

					if (SUCCEEDED(hr))
					{
						filePath = pszFilePath;
						CoTaskMemFree(pszFilePath);
					}
					pItem->Release();
				}
			}
			pFileOpen->Release();
		}

		CoUninitialize();

		if (filePath.empty()) return {};

		int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, filePath.c_str(), static_cast<int>(filePath.size()), nullptr, 0, nullptr, nullptr);
		std::string result(sizeNeeded, 0);
		WideCharToMultiByte(CP_UTF8, 0, filePath.c_str(), static_cast<int>(filePath.size()), result.data(), sizeNeeded, nullptr, nullptr);

		return result;
	}

}