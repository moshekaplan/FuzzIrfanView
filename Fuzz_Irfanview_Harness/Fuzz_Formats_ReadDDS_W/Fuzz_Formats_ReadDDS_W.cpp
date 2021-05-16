#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>

extern "C" __declspec(dllexport) void call_ReadDDS_W(wchar_t *fname);

//Function definition: (LPCWSTR param_filename,uint param_null,ushort *param_out_status,short *param_4)
typedef LPCWSTR* (WINAPIV *ReadDDS_W_t)(LPCWSTR, wchar_t*, wchar_t*, wchar_t*);

HMODULE formats_dll_handle;
ReadDDS_W_t ReadDDS_W_handle;
wchar_t arg3[2048] = { 0 };
wchar_t arg4[2048] = { 0 };

__declspec(noinline) void call_ReadDDS_W(wchar_t *fname) {
	ReadDDS_W_handle(fname, 0, arg3, arg4);
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

	ReadDDS_W_handle = (ReadDDS_W_t)(GetProcAddress(formats_dll_handle, "ReadDDS_W"));
	if (ReadDDS_W_handle == NULL) {
		printf("Failed to get address for ReadDDS_W\n");
		return 1;
	}
	wcsncpy(arg4, L"1;", 100);
	call_ReadDDS_W(argv[1]);
}