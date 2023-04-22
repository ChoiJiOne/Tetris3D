#include "CrashHandler.h"
#include "StringHelper.hpp"

// @third party code - BEGIN
#include <SDL2/SDL.h>
// @third party code - END

std::string CrashHandler::crashDumpFilePath_;
std::vector<std::string> CrashHandler::crashCallStack_;
std::string CrashHandler::lastCrashErrorMessage_;
std::string CrashHandler::crashErrorSourceFileName_;
int32_t CrashHandler::crashErrorLine_;

LONG CrashHandler::DetectApplicationCrash(EXCEPTION_POINTERS* exceptionPointer)
{
	RecordCallStackFromCrash(exceptionPointer);
	GenerateCrashDumpFile(exceptionPointer);

	return EXCEPTION_EXECUTE_HANDLER;
}

void CrashHandler::CrashErrorMessageBox()
{
	std::string crashErrorMessage = StringHelper::Format("Message : %s\n", lastCrashErrorMessage_.c_str());

	for (const auto& stackElement : crashCallStack_)
	{
		crashErrorMessage += stackElement;
		crashErrorMessage += "\n";
	}

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
	SYSTEMTIME CurrentSystemTime;
	GetLocalTime(&CurrentSystemTime);

	std::string crashDumpFilePath = StringHelper::Format(
		"%sWindows-%d-%d-%d-%d-%d-%d.dmp",
		crashDumpFilePath_.c_str(),
		static_cast<int32_t>(CurrentSystemTime.wYear),
		static_cast<int32_t>(CurrentSystemTime.wMonth),
		static_cast<int32_t>(CurrentSystemTime.wDay),
		static_cast<int32_t>(CurrentSystemTime.wHour),
		static_cast<int32_t>(CurrentSystemTime.wMinute),
		static_cast<int32_t>(CurrentSystemTime.wSecond)
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
		MiniDumpNormal, 
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

void CrashHandler::RecordCallStackFromCrash(EXCEPTION_POINTERS* exceptionPointer)
{
	HANDLE currentProcess = GetCurrentProcess();
	HANDLE currentThread = GetCurrentThread();

	if (!SymInitialize(currentProcess, nullptr, TRUE))
	{
#if defined(DEBUG) || defined(_DEBUG)
		OutputDebugStringA("failed to record crash call stack");
#endif
		return;
	}

	CONTEXT* currentCrashContext = exceptionPointer->ContextRecord;

	STACKFRAME64 stackFrame = { };
	stackFrame.AddrPC.Offset = currentCrashContext->Rip;
	stackFrame.AddrPC.Mode = AddrModeFlat;
	stackFrame.AddrFrame.Offset = currentCrashContext->Rbp;
	stackFrame.AddrFrame.Mode = AddrModeFlat;
	stackFrame.AddrStack.Offset = currentCrashContext->Rsp;
	stackFrame.AddrStack.Mode = AddrModeFlat;

	std::vector<DWORD64> addresses;
	DWORD machineType = GetMachineType();

	while (StackWalk64(machineType, currentProcess, currentThread, &stackFrame, currentCrashContext, nullptr, SymFunctionTableAccess64, SymGetModuleBase64, nullptr))
	{
		if (stackFrame.AddrFrame.Offset == 0)
		{
			break;
		}
		else
		{
			addresses.push_back(stackFrame.AddrPC.Offset);
		}
	}

	std::vector<uint8_t> symbolBuffer(sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR));
	PSYMBOL_INFO symbol = reinterpret_cast<PSYMBOL_INFO>(&symbolBuffer[0]);
	symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
	symbol->MaxNameLen = MAX_SYM_NAME;

	for (auto& address : addresses)
	{
		if (SymFromAddr(currentProcess, address, nullptr, symbol))
		{
			std::string callStackElement = StringHelper::Format("%s-0x%0X", symbol->Name, symbol->Address);
			crashCallStack_.push_back(callStackElement);
		}
	}
}

DWORD CrashHandler::GetMachineType()
{
	HMODULE currentModule = GetModuleHandle(nullptr);

	PIMAGE_DOS_HEADER dosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(currentModule);
	PIMAGE_NT_HEADERS ntHeaders = reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<BYTE*>(currentModule) + dosHeader->e_lfanew);

	return static_cast<DWORD>(ntHeaders->FileHeader.Machine);
}