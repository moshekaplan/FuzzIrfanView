#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>

extern "C" __declspec(dllexport) void call_ReadWAD3(char *fname);

//Function definition: char* ReadWAD3(LPCSTR param_fname,uint param_null,char *param_out);
typedef LPCWSTR* (WINAPIV *ReadWAD3_t)(char*, int, char*);

HMODULE formats_dll_handle;
ReadWAD3_t ReadWAD3_handle;
char arg3[2048] = { 0 };

__declspec(noinline) void call_ReadWAD3(char *fname) {
	ReadWAD3_handle(fname, 0, arg3);
}

int main(int argc, char **argv)
{
	if (argc != 2) {
		printf("USAGE: %s input-file\n", argv[0]);
		return 1;
	}

	formats_dll_handle = LoadLibraryA("FORMATS.DLL");
	if (formats_dll_handle == NULL) {
		printf("Failed to load FORMATS.DLL\n");
		return 1;
	}

	ReadWAD3_handle = (ReadWAD3_t)(GetProcAddress(formats_dll_handle, "ReadWAD3"));
	if (ReadWAD3_handle == NULL) {
		printf("Failed to get address for ReadWAD3_handle\n");
		return 1;
	}
	call_ReadWAD3(argv[1]);
}