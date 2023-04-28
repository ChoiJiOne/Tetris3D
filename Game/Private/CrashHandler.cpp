// @third party code - BEGIN
#include <SDL2/SDL.h>
// @third party code - END

#include "StringHelper.hpp"
#include "CrashHandler.h"

std::string CrashHandler::crashDumpFilePath_;
std::string CrashHandler::lastCrashErrorMessage_;
std::string CrashHandler::crashErrorSourceFileName_;
int32_t CrashHandler::crashErrorLine_;

LONG CrashHandler::DetectApplicationCrash(EXCEPTION_POINTERS* exceptionPointer)
{
	GenerateCrashDumpFile(exceptionPointer);

	return EXCEPTION_EXECUTE_HANDLER;
}

void CrashHandler::CrashErrorMessageBox()
{
	std::string crashErrorMessage = StringHelper::Format("Message : %s\n", lastCrashErrorMessage_.c_str());

	int32_t successed = SDL_ShowSimpleMessageBox(
		SDL_MessageBoxFlags::SDL_MESSAGEBOX_ERROR,
		"Detect Application Crash...",
		crashErrorMessage.c_str(),
		nullptr
	);

	if (successed)
	{
#if defined(DEBUG) || defined(_DEBUG)
		OutputDebugStringA("failed to show crash error message box...");
#endif
		return;
	}
}

void CrashHandler::GenerateCrashDumpFile(EXCEPTION_POINTERS* exceptionPointer)
{
	SYSTEMTIME currentSystemTime;
	GetLocalTime(&currentSystemTime);

	std::string crashDumpFilePath = StringHelper::Format(
		"%sWindows-%d-%d-%d-%d-%d-%d.dmp",
		crashDumpFilePath_.c_str(),
		static_cast<int32_t>(currentSystemTime.wYear),
		static_cast<int32_t>(currentSystemTime.wMonth),
		static_cast<int32_t>(currentSystemTime.wDay),
		static_cast<int32_t>(currentSystemTime.wHour),
		static_cast<int32_t>(currentSystemTime.wMinute),
		static_cast<int32_t>(currentSystemTime.wSecond)
	);

	HANDLE  crashDumpFileHandle = CreateFileA(
		crashDumpFilePath.c_str(),
		GENERIC_WRITE,
		FILE_SHARE_WRITE,
		nullptr,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		nullptr
	);

	if (crashDumpFileHandle == INVALID_HANDLE_VALUE)
	{
#if defined(DEBUG) || defined(_DEBUG)
		OutputDebugStringA("failed to generate crash dump file");
#endif
		return;
	}

	_MINIDUMP_EXCEPTION_INFORMATION exception;
	exception.ThreadId = GetCurrentThreadId();
	exception.ExceptionPointers = exceptionPointer;
	exception.ClientPointers = FALSE;

	BOOL successed = MiniDumpWriteDump(
		GetCurrentProcess(), 
		GetCurrentProcessId(), 
		crashDumpFileHandle, 
		MiniDumpWithFullMemory,
		&exception,
		nullptr, 
		nullptr
	);

	if (!successed)
	{
#if defined(DEBUG) || defined(_DEBUG)
		OutputDebugStringA("failed to generate crash dump file");
#endif
		return;
	}

	successed = CloseHandle(crashDumpFileHandle);
	if (!successed)
	{
#if defined(DEBUG) || defined(_DEBUG)
		OutputDebugStringA("failed to close crash dump file");
#endif
		return;
	}
}