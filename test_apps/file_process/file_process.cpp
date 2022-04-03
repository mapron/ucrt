#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <stdio.h>

void openCloseFileCreateFile(const wchar_t* tempFilename, int msDelay)
{
	SECURITY_ATTRIBUTES security_attributes;
	security_attributes.nLength				 = sizeof(security_attributes);
	security_attributes.lpSecurityDescriptor = nullptr;
	security_attributes.bInheritHandle		 = false;

	auto fileHandle = CreateFileW(tempFilename,	 // file name
								  GENERIC_WRITE, // open for write
								  0,			 // do not share
								  &security_attributes,
								  CREATE_ALWAYS,		 // overwrite existing
								  FILE_ATTRIBUTE_NORMAL, // normal file
								  NULL);				 // no template

	const char buf[] = "aaa\n";
	::WriteFile(fileHandle, buf, sizeof(buf), NULL, NULL);
	Sleep(msDelay);

	::CloseHandle(fileHandle);
}

void openCloseFileFopen(const wchar_t* tempFilename, int msDelay)
{
	FILE* m_file = _wfopen(tempFilename, L"wb");

	printf("file opened\n");
	fflush(stdout);

	const char buf[] = "aaa\n";
	fwrite(buf, 1, sizeof(buf), m_file);

	Sleep(msDelay);
	fclose(m_file);
}

DWORD WINAPI FileCopyThread(LPVOID lpParameter)
{
	const wchar_t* tempFilename	 = L"./tem_file";
	const wchar_t* finalFilename = L"./out_file";
	{
		printf("file opening\n");
		fflush(stdout);

		openCloseFileFopen(tempFilename, 500);

		printf("file closed\n");
		fflush(stdout);
	}
	bool renameSuccess = (::MoveFileExW(tempFilename, finalFilename, MOVEFILE_REPLACE_EXISTING | MOVEFILE_COPY_ALLOWED) != 0);
	if (renameSuccess)
	{
		printf("Rename SUCCESS!\n");
		fflush(stdout);
	}
	else
	{
		printf("Rename FAILED!\n");
		fflush(stdout);
	}

	return 0;
}

int main(int argc, char** argv)
{
	HANDLE hThread = CreateThread(
		NULL,			// Thread attributes
		0,				// Stack size (0 = use default)
		FileCopyThread, // Thread start address
		NULL,			// Parameter to pass to the thread
		0,				// Creation flags
		NULL);			// Thread id
	if (hThread == NULL)
		return 1;

	Sleep(2);
	printf("Starting calc1.exe\n");
	fflush(stdout);
	system("calc1.exe");

	printf("Finished system(), joining\n");
	fflush(stdout);

	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);
	printf("Exiting main()\n");
	return 0;
}
