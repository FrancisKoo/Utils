#include "stdafx.h"

#include "StringEncoding.h"
#include <string>
#include <malloc.h>

#if (defined(WIN32)) || (defined(_WIN32) || defined(WINCE) )
#include <windows.h>
#endif //#if (defined(WIN32)) || (defined(_WIN32))


NAMESPACE_DEFINE_BEGIN(Utils)

void UnicodeTransfer(std::string& out, const wchar_t* unicode, size_t buffLen) {
	out = "";
	char buff[8];
	const wchar_t* szE = unicode + buffLen;
	const wchar_t* c = unicode;
	while (c < szE) {
		if ((*c) >= 0 && (*c) <= 127) {
			out += *((unsigned char*)c);
		} else {
			sprintf_s(buff, 8, "%4x", *c);
			out += "\\u";
			out += buff;
		}
		c++;
	}
}

void UnicodeToUTF8(const wchar_t* wstr, std::string& outstr) {
#ifdef WIN32
	outstr = "";
	int nByte = 0;
	char* zFilename = 0;

	if (!wstr)
		return;

	nByte = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, 0, 0, 0, 0);
	zFilename = new char[nByte + 1];
	if (zFilename == 0) {
		return;
	}
	nByte = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, zFilename, nByte + 1, 0, 0);
	if (nByte > 0) {
		zFilename[nByte] = '\0';
		outstr = zFilename;
	}
	delete[]zFilename;
#endif
	return;
}

void UTF8ToUnicode(const char* utf8, std::wstring& woutstr) {
#ifdef WIN32
	woutstr = L"";
	int nSize = 0;
	wchar_t *zWideFilename = 0;

	if (!utf8)
		return;

	nSize = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
	zWideFilename = new wchar_t[nSize + 1];
	if (zWideFilename == 0) {
		return;
	}
	nSize = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, zWideFilename, nSize);
	if (nSize > 0) {
		zWideFilename[nSize] = 0;
		woutstr = zWideFilename;
	}
	delete[]zWideFilename;
#endif
	return;
}

std::wstring  UTF8ToUnicode(const char* zFilename) {
	std::wstring strFileName = L"";
#ifdef WIN32
	wchar_t *zWideFilename;
	int nSize = MultiByteToWideChar(CP_UTF8, 0, zFilename, -1, NULL, 0);
	//nSize *= sizeof(wchar_t);
	zWideFilename = new wchar_t[nSize + 1];
	if (zWideFilename == 0) {
		return strFileName;
	}
	nSize = MultiByteToWideChar(CP_UTF8, 0, zFilename, -1, zWideFilename, nSize);
	if (nSize > 0) {
		zWideFilename[nSize] = 0;
		strFileName = zWideFilename;
	}
	delete[]zWideFilename;
#endif
	return strFileName;
}

std::wstring UTF8ToUnicode(const std::string& str) {
    return UTF8ToUnicode(str.c_str());
}

/*
** Convert microsoft unicode to UTF-8.  Space to hold the returned string is
** obtained from sqliteMalloc().
*/
std::string UnicodeToUTF8(const wchar_t* zWideFilename) {
	std::string strFileName = "";
#ifdef WIN32
	int nByte;
	char* zFilename;
	nByte = WideCharToMultiByte(CP_UTF8, 0, zWideFilename, -1, 0, 0, 0, 0);
	zFilename = new char[nByte + 1];
	if (zFilename == 0) {
		return strFileName;
	}
	nByte = WideCharToMultiByte(CP_UTF8, 0, zWideFilename, -1, zFilename, nByte + 1, 0, 0);
	if (nByte > 0) {
		zFilename[nByte] = '\0';
		strFileName = zFilename;
	}
	delete[]zFilename;
#endif
	return strFileName;
}

std::string UnicodeToUTF8(const std::wstring& str) {
    return UnicodeToUTF8(str.c_str());
}

/*
** Convert an ansi string to microsoft unicode, based on the
** current codepage settings for file apis.
**
** Space to hold the returned string is obtained
** from sqliteMalloc.
*/
std::wstring MbcsToUnicode(const char* zFilename) {
	std::wstring strFileName = L"";
#ifdef WIN32
	wchar_t *zMbcsFilename;

	int codepage = AreFileApisANSI() ? CP_ACP : CP_OEMCP;

	int nByte = MultiByteToWideChar(codepage, 0, zFilename, -1, NULL, 0);
	zMbcsFilename = new wchar_t[nByte + 1];
	if (zMbcsFilename == 0) {
		return strFileName;
	}
	nByte = MultiByteToWideChar(codepage, 0, zFilename, -1, zMbcsFilename, nByte);
	if (nByte > 0) {
		zMbcsFilename[nByte] = 0;
		strFileName = zMbcsFilename;
	}
	delete[] zMbcsFilename;
#endif
	return strFileName;

}

std::wstring MbcsToUnicode(const std::string& str) {
    return MbcsToUnicode(str.c_str());
}

/*
** Convert microsoft unicode to multibyte character string, based on the
** user's Ansi codepage.
**
** Space to hold the returned string is obtained from
** sqliteMalloc().
*/
std::string UnicodeToMbcs(const wchar_t* zWideFilename) {
	std::string strFileName = "";
#ifdef WIN32
	int nByte;
	char* zFilename;
	int codepage = AreFileApisANSI() ? CP_ACP : CP_OEMCP;

	nByte = WideCharToMultiByte(codepage, 0, zWideFilename, -1, 0, 0, 0, 0);
	zFilename = new char[nByte + 1];
	if (zFilename == 0) {
		return strFileName;
	}
	nByte = WideCharToMultiByte(codepage, 0, zWideFilename, -1, zFilename, nByte + 1, 0, 0);
	if (nByte > 0) {
		zFilename[nByte] = '\0';
		strFileName = zFilename;
	}
	delete[]zFilename;
#endif
	return strFileName;
}

std::string UnicodeToMbcs(const std::wstring& str) {
    return UnicodeToMbcs(str.c_str());
}

/*
** Convert multibyte character string to UTF-8.  Space to hold the
** returned string is obtained from sqliteMalloc().
*/
std::string MbcsToUTF8(const char* zFilename) {
	std::wstring zTmpWide = MbcsToUnicode(zFilename);
	if (zTmpWide.empty()) {
		return std::string("");
	}
	return UnicodeToUTF8(zTmpWide.c_str());
}

std::string MbcsToUTF8(const std::string& str) {
    return MbcsToUTF8(str.c_str());
}

/*
** Convert UTF-8 to multibyte character string.  Space to hold the
** returned string is obtained from sqliteMalloc().
*/
std::string UTF8ToMbcs(const char* zFilename) {
	std::wstring zTmpWide = UTF8ToUnicode(zFilename);
	if (zTmpWide.empty()) {
		return std::string("");
	}
	return UnicodeToMbcs(zTmpWide.c_str());
}

std::string UTF8ToMbcs(const std::string& str) {
    return UTF8ToMbcs(str.c_str());
}

static bool __stdcall decodeUnicode(const char*& current, const char* end, wchar_t& unicode) {
	if (end - current < 4)
		return false;

	unicode = 0;
	char c;

	for (int index = 0; index < 4; ++index) {
		c = *current++;
		unicode *= 16;
		if (c >= '0'  &&  c <= '9')
			unicode += c - '0';
		else if (c >= 'a'  &&  c <= 'f')
			unicode += c - 'a' + 10;
		else if (c >= 'A'  &&  c <= 'F')
			unicode += c - 'A' + 10;
		else {
			return false;
		}
	}
	return true;
}

void TransferUicode(std::string& out, const char* transfer, size_t buffLen) {
#ifdef WIN32
	wchar_t unicode = 0;
	char c, escape, cbuf[4] = {0};
	int codepage = AreFileApisANSI() ? CP_ACP : CP_OEMCP;

	const char* current = transfer;
	const char* end = transfer + buffLen;

	while (current != end) {
		c = *current++;
		if (c == '\\') {
			if (current == end)
				break;

			escape = *current++;
			if (escape == 'u') {
				if (!decodeUnicode(current, end, unicode))
					return;

				memset(cbuf, 0, sizeof(char) * 4);
				WideCharToMultiByte(codepage, 0, &unicode, 1, cbuf, 4, 0, 0);
				out += cbuf;
			}
		} else {
			out += c;
		}
	} // while
#endif
}

NAMESPACE_DEFINE_END(Utils)