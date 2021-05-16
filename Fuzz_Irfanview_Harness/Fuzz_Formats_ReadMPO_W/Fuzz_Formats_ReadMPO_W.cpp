#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>

extern "C" __declspec(dllexport) void call_ReadMPO_W(wchar_t *fname);

typedef LPCWSTR* (WINAPIV *ReadMPO_W_t)(LPCWSTR, int, wchar_t*, int, wchar_t*, wchar_t*);

HMODULE formats_dll_handle;
ReadMPO_W_t ReadMPO_W_handle;
wchar_t arg3_ini[2048] = { 0 };
wchar_t arg5_out[2048] = { 0 };
wchar_t arg6_out[2048] = { 0 };

__declspec(noinline) void call_ReadMPO_W(wchar_t *fname) {
	/**
	FORMATS!ReadMPO_W
		00197914 00487990 image00400000+0x87990 - previous function

		00197918 005a8c00 image00400000 + 0x1a8c00 - W_filename
		0019791c 00000000 - image_number
		00197920 005a8780 image00400000 + 0x1a8780 - INI_path
		00197924 00000000 - NULL
		00197928 0019811c - pointer to null memory
		0019792c 0019852c - pointer to L"None"

		When viewing "Next page":
		00198d54 00487990 image00400000+0x87990

		00198d58 005a8c00 image00400000+0x1a8c00
		00198d5c 00000001
		00198d60 005a8780 image00400000+0x1a8780
		00198d64 00000000
		00198d68 005a9020 image00400000+0x1a9020
		00198d6c 005a9460 image00400000+0x1a9460

	*/
	ReadMPO_W_handle(fname, 0, arg3_ini, 0x0, arg5_out, arg6_out);
	// Note: This assumes two images are always available. Unclear what this will cause in practice.
	ReadMPO_W_handle(fname, 1, arg3_ini, 0x0, arg5_out, arg6_out);

	// Note: Files must end with extension .mpo to trigger
	// the mpo handler in Irfanview
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

	ReadMPO_W_handle = (ReadMPO_W_t)(GetProcAddress(formats_dll_handle, "ReadMPO_W"));
	if (ReadMPO_W_handle == NULL) {
		printf("Failed to get address for ReadMPO_W\n");
		return 1;
	}
	call_ReadMPO_W(argv[1]);
}