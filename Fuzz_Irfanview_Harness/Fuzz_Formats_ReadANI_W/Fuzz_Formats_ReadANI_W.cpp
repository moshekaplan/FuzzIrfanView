#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>

extern "C" __declspec(dllexport) void call_ReadANI_W(wchar_t *fname);

//Function definition: ReadANI_W(LPCWSTR param_filename,uint param_0x0,int param_0x1,int param_0x00,uint param_0x1000, ushort *param_out);
typedef LPCWSTR* (WINAPIV *ReadANI_W_t)(LPCWSTR, int, int, char *, int, wchar_t*);

HMODULE formats_dll_handle;
ReadANI_W_t ReadANI_W_handle;
char arg4[0x2500] = { 0 };
wchar_t arg6[2048] = { 0 };

__declspec(noinline) void call_ReadANI_W(wchar_t *fname) {
	// Interestingly, this gets called twice - but only for some files
	ReadANI_W_handle(fname, 0, 1, NULL, 0x1000, arg6);
	//ReadANI_W_handle(fname, 0, 0, arg4, 0x1000, arg6);
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

	ReadANI_W_handle = (ReadANI_W_t)(GetProcAddress(formats_dll_handle, "ReadANI_W"));
	if (ReadANI_W_handle == NULL) {
		printf("Failed to get address for ReadANI_W\n");
		return 1;
	}
	call_ReadANI_W(argv[1]);
}