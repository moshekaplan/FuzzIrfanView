#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>

extern "C" __declspec(dllexport) void call_Read_BIO_RAD_W(wchar_t *fname);

//Function definition: char* (*Read_BIO_RAD_W_handle)(LPCWSTR, unsigned short*, short*);
typedef LPCWSTR* (WINAPIV *Read_BIO_RAD_W_t)(LPCWSTR, int, wchar_t*, wchar_t*);

HMODULE formats_dll_handle;
Read_BIO_RAD_W_t Read_BIO_RAD_W_handle;
wchar_t arg3[2048] = { 0 };
wchar_t arg4[2048] = { 0 };

__declspec(noinline) void call_Read_BIO_RAD_W(wchar_t *fname) {
	Read_BIO_RAD_W_handle(fname, 0, arg3, arg4);
}

int wmain(int argc, wchar_t **argv)
{
	if (argc != 2) {
		printf("USAGE: %ws input-file\n", argv[0]);
		return 1;
	}

	formats_dll_handle = LoadLibraryA("Formats.DLL");
	if (formats_dll_handle == NULL) {
		printf("Failed to load Formats.DLL\n");
		return 1;
	}

	Read_BIO_RAD_W_handle = (Read_BIO_RAD_W_t)(GetProcAddress(formats_dll_handle, "Read_BIO_RAD_W"));
	if (Read_BIO_RAD_W_handle == NULL) {
		printf("Failed to get address for Read_BIO_RAD_W\n");
		return 1;
	}
	wcsncpy(arg3, L"None", 100);
	call_Read_BIO_RAD_W(argv[1]);
}