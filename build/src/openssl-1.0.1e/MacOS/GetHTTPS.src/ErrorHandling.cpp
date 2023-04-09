#include "ErrorHandling.hpp"
#include "CPStringUtils.hpp"

static constexpr int kErrorMessageBufferSize = 512;
static char gErrorMessageBuffer[kErrorMessageBufferSize];

char* const gErrorMessage = gErrorMessageBuffer;
const int gErrorMessageMaxLength = kErrorMessageBufferSize;

void SetErrorMessage(const char* const theErrorMessage) {
    if (theErrorMessage != nullptr) {
        strncpy(gErrorMessage, theErrorMessage, gErrorMessageMaxLength);
    }
}

void SetErrorMessageAndAppendLongInt(const char* const theErrorMessage, const long theLongInt) {
    if (theErrorMessage != nullptr) {
        snprintf(gErrorMessage, gErrorMessageMaxLength, "%s%ld", theErrorMessage, theLongInt);
    }
}

void SetErrorMessageAndCStrAndLongInt(const char* const theErrorMessage, const char* const theCStr, const long theLongInt) {
    if (theErrorMessage != nullptr && theCStr != nullptr) {
        snprintf(gErrorMessage, gErrorMessageMaxLength, "%s%s%ld", theErrorMessage, theCStr, theLongInt);
    }
}

void SetErrorMessageAndCStr(const char* const theErrorMessage, const char* const theCStr) {
    if (theErrorMessage != nullptr && theCStr != nullptr) {
        snprintf(gErrorMessage, gErrorMessageMaxLength, "%s%s", theErrorMessage, theCStr);
    }
}

void AppendCStrToErrorMessage(const char* const theErrorMessage) {
    if (theErrorMessage != nullptr) {
        strncat(gErrorMessage, theErrorMessage, gErrorMessageMaxLength - strlen(gErrorMessage) - 1);
    }
}

void AppendLongIntToErrorMessage(const long theLongInt) {
    char longIntBuffer[32];
    snprintf(longIntBuffer, sizeof(longIntBuffer), "%ld", theLongInt);
    strncat(gErrorMessage, longIntBuffer, gErrorMessageMaxLength - strlen(gErrorMessage) - 1);
}

char* const GetErrorMessage() {
    return gErrorMessage;
}

OSErr GetErrorMessageInNewHandle(Handle* const inoutHandle) {
    if (inoutHandle == nullptr) {
        return -1;
    }
    *inoutHandle = NewHandle(strlen(gErrorMessage) + 1);
    if (*inoutHandle == nullptr) {
        return MemError();
    }
    strncpy(*inoutHandle, gErrorMessage, GetHandleSize(*inoutHandle));
    return noErr;
}

OSErr GetErrorMessageInExistingHandle(Handle inoutHandle) {
    if (inoutHandle == nullptr) {
        return -1;
    }
    if (GetHandleSize(inoutHandle) < strlen(gErrorMessage) + 1) {
        SetHandleSize(inoutHandle, strlen(gErrorMessage) + 1);
        if (MemError() != noErr) {
            return MemError();
        }
    }
    strncpy(*inoutHandle, gErrorMessage, GetHandleSize(inoutHandle));
    return noErr;
}

OSErr AppendErrorMessageToHandle(Handle inoutHandle) {
    if (inoutHandle == nullptr) {
        return -1;
    }
    const size_t currentSize = GetHandleSize(inoutHandle);
    const size_t errorMessageSize = strlen(gErrorMessage) + 1;
    SetHandleSize(inoutHandle, currentSize + errorMessageSize);
    if (MemError() != noErr) {
        return MemError();
    }
    strncat(*inoutHandle, gErrorMessage, errorMessageSize);
    return noErr;
}

#ifdef __EXCEPTIONS_ENABLED__

void ThrowErrorMessageException() {
    throw CMyException(gErrorMessage);
}

#endif
