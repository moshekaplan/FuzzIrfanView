#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>

extern "C" __declspec(dllexport) void call_ReadRAW_W(wchar_t *fname);

typedef LPCWSTR* (WINAPIV *ReadRAW_W_t)(LPCWSTR, HWND, wchar_t*, wchar_t*);

HMODULE formats_dll_handle;
ReadRAW_W_t ReadRAW_W_handle;
wchar_t arg3_ini[2048] = { 0 };
wchar_t arg4_out[2048] = { 0 };

__declspec(noinline) void call_ReadRAW_W(wchar_t *fname) {
	// consider using the offset instead to minimize disk calls for reading/writing the INI
	ReadRAW_W_handle(fname, 0x0, arg3_ini, arg4_out);
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

	ReadRAW_W_handle = (ReadRAW_W_t)(GetProcAddress(formats_dll_handle, "ReadRAW_W"));
	if (ReadRAW_W_handle == NULL) {
		printf("Failed to get address for ReadRAW_W\n");
		return 1;
	}
	call_ReadRAW_W(argv[1]);
}