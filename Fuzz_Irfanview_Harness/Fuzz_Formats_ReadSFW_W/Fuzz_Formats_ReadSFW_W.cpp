#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>

/*
Note that the file must begin with SFW94A for IrfanView to 
detect the file as being an SFW file and call Formats.dll's ReadSFW_W
*/

extern "C" __declspec(dllexport) void call_ReadSFW_W(wchar_t *fname);

typedef LPCWSTR* (WINAPIV *ReadSFW_W_t)(LPCWSTR, wchar_t*, wchar_t*);

HMODULE formats_dll_handle;
ReadSFW_W_t ReadSFW_W_handle;
wchar_t arg2[2048] = { 0 };
wchar_t arg3[2048] = { 0 };

__declspec(noinline) void call_ReadSFW_W(wchar_t *fname) {
	ReadSFW_W_handle(fname, arg2, arg3);
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

	ReadSFW_W_handle = (ReadSFW_W_t)(GetProcAddress(formats_dll_handle, "ReadSFW_W"));
	if (ReadSFW_W_handle == NULL) {
		printf("Failed to get address for ReadSFW_W\n");
		return 1;
	}
	call_ReadSFW_W(argv[1]);
}