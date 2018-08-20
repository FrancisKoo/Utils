#include "stdafx.h"
#include "UrlEncode.h"

#include <assert.h>


NAMESPACE_DEFINE_BEGIN(Utils)

inline
unsigned char ToHex(unsigned char x) {
	return  x > 9 ? x + 55 : x + 48;
}
inline
unsigned char ToHexLower(unsigned char x) {
    return  x > 9 ? x + 87 : x + 48;
}
inline
unsigned char FromHex(unsigned char x) {
	unsigned char y;
	if (x >= 'A' && x <= 'Z') y = x - 'A' + 10;
	else if (x >= 'a' && x <= 'z') y = x - 'a' + 10;
	else if (x >= '0' && x <= '9') y = x - '0';
	else assert(0);
	return y;
}

std::string UrlEncode(const std::string& str) {
	std::string strTemp = "";
	size_t length = str.length();
	for (size_t i = 0; i < length; i++) {
		if (isalnum((unsigned char)str[i]) ||
			(str[i] == '-') ||
			(str[i] == '_') ||
			(str[i] == '.') ||
			(str[i] == '~'))
			strTemp += str[i];
		else if (str[i] == ' ')
			strTemp += "+";
		else {
			strTemp += '%';
			strTemp += ToHex((unsigned char)str[i] >> 4);
			strTemp += ToHex((unsigned char)str[i] % 16);
		}
	}
	return strTemp;
}
std::string UrlEncodeLower(const std::string& str) {
    std::string strTemp = "";
    size_t length = str.length();
    for (size_t i = 0; i < length; i++) {
        if (isalnum((unsigned char)str[i]) ||
            (str[i] == '-') ||
            (str[i] == '_') ||
            (str[i] == '.') ||
            (str[i] == '~'))
            strTemp += str[i];
        else if (str[i] == ' ')
            strTemp += "+";
        else {
            strTemp += '%';
            strTemp += ToHexLower((unsigned char)str[i] >> 4);
            strTemp += ToHexLower((unsigned char)str[i] % 16);
        }
    }
    return strTemp;
}
std::string UrlDecode(const std::string& str) {
	std::string strTemp = "";
	size_t length = str.length();
	for (size_t i = 0; i < length; i++) {
		if (str[i] == '+') strTemp += ' ';
		else if (str[i] == '%') {
			assert(i + 2 < length);
			unsigned char high = FromHex((unsigned char)str[++i]);
			unsigned char low = FromHex((unsigned char)str[++i]);
			strTemp += high * 16 + low;
		} else strTemp += str[i];
	}
	return strTemp;
}

NAMESPACE_DEFINE_END(Utils)