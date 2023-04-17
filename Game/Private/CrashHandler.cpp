#include "CrashHandler.h"
#include "StringHelper.hpp"

std::string CrashHandler::crashDumpFilePath_;
std::vector<std::string> CrashHandler::crashCallStack_;
std::string CrashHandler::lastCrashErrorMessage_;
std::string CrashHandler::crashErrorSourceFileName_;
int32_t CrashHandler::crashErrorLine_;

LONG CrashHandler::DetectApplicationCrash(EXCEPTION_POINTERS* exceptionPointer)
{
	return 0;
}
