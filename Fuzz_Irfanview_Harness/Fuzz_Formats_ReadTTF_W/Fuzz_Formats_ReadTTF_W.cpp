#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>

extern "C" __declspec(dllexport) void call_ReadTTF_W(wchar_t *fname);

// Note: Ghidra detects this incorrectly in formats.dll
typedef LPCWSTR* (WINAPIV *ReadTTF_W_t)(LPCWSTR, int, int, wchar_t*, wchar_t*);

HMODULE formats_dll_handle;
ReadTTF_W_t ReadTTF_W_handle;
wchar_t arg4[2048] = { 0 };
wchar_t arg5[2048] = { 0 };

__declspec(noinline) void call_ReadTTF_W(wchar_t *fname) {
	ReadTTF_W_handle(fname, 0x400, 1000, arg4, arg5);
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

	ReadTTF_W_handle = (ReadTTF_W_t)(GetProcAddress(formats_dll_handle, "ReadTTF_W"));
	if (ReadTTF_W_handle == NULL) {
		printf("Failed to get address for ReadTTF_W\n");
		return 1;
	}

	// This uses ASCII printable by default. But can we make it smaller to improve speeds?
	wcsncpy(arg4, L"!\"#$%&'()*+,-./0123456789:;<=>?@.ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`.abcdefghijklmnopqrstuvwxyz{|}~", 500);
	call_ReadTTF_W(argv[1]);
}