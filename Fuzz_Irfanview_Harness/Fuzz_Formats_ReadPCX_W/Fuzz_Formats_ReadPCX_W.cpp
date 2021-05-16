#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>

extern "C" __declspec(dllexport) void call_ReadPCX_W(wchar_t *fname);

//Function definition: char* (*ReadPCX_W_handle)(LPCWSTR, unsigned short*, short*);
typedef LPCWSTR* (WINAPIV *ReadPCX_W_t)(LPCWSTR, char*, int, wchar_t*, wchar_t*);

HMODULE formats_dll_handle;
ReadPCX_W_t ReadPCX_W_handle;
wchar_t arg4[2048] = { 0 };
wchar_t arg5[2048] = { 0 };

__declspec(noinline) void call_ReadPCX_W(wchar_t *fname) {
	ReadPCX_W_handle(fname, 0x0, 0x0, arg4, arg5);
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

	ReadPCX_W_handle = (ReadPCX_W_t)(GetProcAddress(formats_dll_handle, "ReadPCX_W"));
	if (ReadPCX_W_handle == NULL) {
		printf("Failed to get address for ReadPCX_W\n");
		return 1;
	}
	call_ReadPCX_W(argv[1]);
}