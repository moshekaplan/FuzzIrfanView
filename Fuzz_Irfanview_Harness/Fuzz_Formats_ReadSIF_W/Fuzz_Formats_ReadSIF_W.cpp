#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>

extern "C" __declspec(dllexport) void call_ReadSIF_W(wchar_t *fname);

typedef LPCWSTR* (WINAPIV *ReadSIF_W_t)(LPCWSTR, wchar_t*, wchar_t*);

HMODULE formats_dll_handle;
ReadSIF_W_t ReadSIF_W_handle;
wchar_t arg2[2048] = { 0 };
wchar_t arg3[2048] = { 0 };

__declspec(noinline) void call_ReadSIF_W(wchar_t *fname) {
	ReadSIF_W_handle(fname, arg2, arg3);
	// Note: To trigger the SIF handler in i_view32.exe, the file
	// must begin with "SIF"
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

	ReadSIF_W_handle = (ReadSIF_W_t)(GetProcAddress(formats_dll_handle, "ReadSIF_W"));
	if (ReadSIF_W_handle == NULL) {
		printf("Failed to get address for ReadSIF_W\n");
		return 1;
	}
	call_ReadSIF_W(argv[1]);
}