#include "ErrorHandling.hpp"
#include "CPStringUtils.hpp"

#ifdef __EXCEPTIONS_ENABLED__
#include "CMyException.hpp"
#endif

static char gErrorMessageBuffer[512];
char* gErrorMessage = gErrorMessageBuffer;
int gErrorMessageMaxLength = sizeof(gErrorMessageBuffer);

void SetErrorMessage(const char* theErrorMessage)
{
    if (theErrorMessage != nullptr)
    {
        CopyCStrToCStr(theErrorMessage, gErrorMessage, gErrorMessageMaxLength);
    }
}

void SetErrorMessageAndAppendLongInt(const char* theErrorMessage, const long theLongInt)
{
    if (theErrorMessage != nullptr)
    {
        CopyCStrAndInsert1LongIntIntoCStr(theErrorMessage, theLongInt, gErrorMessage, gErrorMessageMaxLength);
    }
}

void SetErrorMessageAndCStrAndLongInt(const char* theErrorMessage, const char* theCStr, const long theLongInt)
{
    if (theErrorMessage != nullptr)
    {
        CopyCStrAndInsertCStrLongIntIntoCStr(theErrorMessage, theCStr, theLongInt, gErrorMessage, gErrorMessageMaxLength);
    }
}

void SetErrorMessageAndCStr(const char* theErrorMessage, const char* theCStr)
{
    if (theErrorMessage != nullptr)
    {
        CopyCStrAndInsertCStrLongIntIntoCStr(theErrorMessage, theCStr, -1, gErrorMessage, gErrorMessageMaxLength);
    }
}

void AppendCStrToErrorMessage(const char* theErrorMessage)
{
    if (theErrorMessage != nullptr)
    {
        ConcatCStrToCStr(theErrorMessage, gErrorMessage, gErrorMessageMaxLength);
    }
}

void AppendLongIntToErrorMessage(const long theLongInt)
{
    ConcatLongIntToCStr(theLongInt, gErrorMessage, gErrorMessageMaxLength);
}

char* GetErrorMessage(void)
{
    return gErrorMessage;
}

OSErr GetErrorMessageInNewHandle(Handle* inoutHandle)
{
    return CopyCStrToNewHandle(gErrorMessage, inoutHandle);
}

OSErr GetErrorMessageInExistingHandle(Handle inoutHandle)
{
    return CopyCStrToExistingHandle(gErrorMessage, inoutHandle);
}

OSErr AppendErrorMessageToHandle(Handle inoutHandle)
{
    return AppendCStrToHandle(gErrorMessage, inoutHandle, nullptr);
}

#ifdef __EXCEPTIONS_ENABLED__

void ThrowErrorMessageException(void)
{
    ThrowDescriptiveException(gErrorMessage);
}

#endif
