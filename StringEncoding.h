#ifndef UTIL_STRINGENCODING_H
#define UTIL_STRINGENCODING_H

#include "utils.h"

#include <string>
#include <stdio.h>

NAMESPACE_DEFINE_BEGIN(Utils)

//Convert  utf8 to microsoft unicode
std::wstring UTF8ToUnicode(const char* str);
std::wstring UTF8ToUnicode(const std::string& str);


//Convert microsoft unicode to UTF-8.  Space to hold the returned string is
//obtained from malloc().
std::string UnicodeToUTF8(const wchar_t* zWideFilename);
std::string UnicodeToUTF8(const std::wstring& str);


/*
** Convert an ansi string to microsoft unicode, based on the
** current codepage settings for file apis.
**
** Space to hold the returned string is obtained
** from malloc.
*/
std::wstring MbcsToUnicode(const char* zFilename);
std::wstring MbcsToUnicode(const std::string& str);

/*
** Convert microsoft unicode to multibyte character string, based on the
** user's Ansi codepage.
**
** Space to hold the returned string is obtained from
** malloc().
*/
std::string UnicodeToMbcs(const wchar_t* zWideFilename);
std::string UnicodeToMbcs(const std::wstring& str);


/*
** Convert multibyte character string to UTF-8.  Space to hold the
** returned string is obtained from malloc().
*/
std::string MbcsToUTF8(const char* zFilename);
std::string MbcsToUTF8(const std::string& str);


/*
** Convert UTF-8 to multibyte character string.  Space to hold the
** returned string is obtained from malloc().
*/
std::string UTF8ToMbcs(const char* zFilename);
std::string UTF8ToMbcs(const std::string& str);


//将UNICODE 串输出成 转义符 " \uxxxx "
void UnicodeTransfer(std::string& out, const wchar_t* unicode, size_t buffLen);

//将" \uxxxx " 转义符 串输出成  unicode
void TransferUicode(std::string& out, const char* transfer, size_t buffLen);


NAMESPACE_DEFINE_END(Utils);
#endif