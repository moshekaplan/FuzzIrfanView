#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>

extern "C" __declspec(dllexport) void call_ReadG3_W(wchar_t *fname);

// Note: Ghidra detects this incorrectly in formats.dll
// Also note: If you're trying to manually trigger analysis of a g3 file,
// IrfanView relies on the .g3 extension to select the G3 parser in formats.dll
typedef LPCWSTR* (WINAPIV *ReadG3_W_t)(LPCWSTR, wchar_t*);

HMODULE formats_dll_handle;
ReadG3_W_t ReadG3_W_handle;
wchar_t arg2[2048] = { 0 };

__declspec(noinline) void call_ReadG3_W(wchar_t *fname) {
	ReadG3_W_handle(fname, arg2);
}

int wmain(int argc, wchar_t **argv)
{
	if (argc != 2) {
		printf("USAGE: %ws input-file\n", argv[0]);
		return 1;
	}

	formats_dll_handle = LoadLibraryA("FORMATS.DLL");
	if (formats_dll_handle == NULL) {
		printf("Failed to load FORMATS.DLL\n");
		return 1;
	}

	ReadG3_W_handle = (ReadG3_W_t)(GetProcAddress(formats_dll_handle, "ReadG3_W"));
	if (ReadG3_W_handle == NULL) {
		printf("Failed to get address for ReadG3_W\n");
		return 1;
	}

	call_ReadG3_W(argv[1]);
}