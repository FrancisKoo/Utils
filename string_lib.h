#pragma once

#include "utils.h"

#include <sstream>
#include <algorithm>

#include <regex>
#include <vector>
#include <string>
#include <functional>


NAMESPACE_DEFINE_BEGIN(Utils)

template<typename T>
using std_string = std::basic_string <T, std::char_traits<T>, std::allocator<T> >;

template<typename T>
using std_ostringstream =
    std::basic_ostringstream<T, std::char_traits<T>, std::allocator<T>>;

template<typename T>
inline std_string<T> get_backslash();
template<>
inline std_string<wchar_t> get_backslash<wchar_t>() { return L"/"; }
template<>
inline std_string<char> get_backslash<char>() { return "/"; }

template<typename T>
inline std_string<T> get_slash();
template<>
inline std_string<wchar_t> get_slash<wchar_t>() { return L"\\"; }
template<>
inline std_string<char> get_slash<char>() { return "\\"; }

//去除左空格
template<typename T>
inline std_string<T>& ltrim(std_string<T>& str) {
    str.erase(str.begin(),
        std::find_if(str.begin(), str.end(), [](int c) { return c != L' '; })
        );
    return str;
}

//去除右空格
template<typename T>
inline std_string<T>& rtrim(std_string<T>& str) {
    if (!str.empty()) {
        auto p = std::find_if(str.rbegin(), str.rend(), [](int c) { return c != ' '; });
        str.erase(p.base(), str.end());
    }
    return str;
}

//去除左右空格
template<typename T>
inline std_string<T>& trim(std_string<T>& str) {
    return ltrim(rtrim(str));
}


template<typename T>
inline bool is_have_chinese_charactor(std_string<T>& str) {
    return std::find_if(str.begin(), str.end(),
        [](TCHAR ch) { return ch >= 0x4E00 && ch <= 0x9FA5; })
        != str.end();
}

//大写转换
template<typename T>
inline std_string<T>& to_upper(std_string<T>& str) {
    std::transform(str.begin(), str.end(), str.begin(), [](T ch) { return ::toupper(ch); });
    return str;
}

template<typename T>
inline std_string<T> to_upper(const std_string<T>& str) {
    std_string<T> upper_str = str;
    std::transform(upper_str.begin(), upper_str.end(), upper_str.begin(), [](T ch) { return ::toupper(ch); });
    return upper_str;
}

//小写转换
template<typename T>
inline std_string<T>& to_lower(std_string<T>& str) {
    std::transform(str.begin(), str.end(), str.begin(), [](T ch) { return ::tolower(ch); });
    return str;
}

template<typename T>
inline std_string<T> to_lower(const std_string<T>& str) {
    std_string<T> lower_str = str;
    std::transform(lower_str.begin(), lower_str.end(), lower_str.begin(), [](T ch) { return ::tolower(ch); });
    return lower_str;
}

//判断是否以subString起始
template<typename T>
inline bool is_begin_with(const std_string<T>& str, const std_string<T>& subString) {
    if (str.empty() || subString.empty() || subString.length() > str.length()) {
        return false;
    }
    return str.compare(0, subString.size(), subString) == 0;
}

//判断是否以subString结尾
template<typename T>
inline bool is_end_with(const std_string<T>& str, const std_string<T>& subString) {
    if (str.empty() || subString.empty() || subString.length() > str.length()) {
        return false;
    }
    return str.compare(str.length() - subString.length(), subString.length(), subString) == 0;
}

//判断前后是否有空格
template<typename T>
inline bool is_begin_or_end_with_space(const std_string<T>& str) {
    return ::isspace(*str.begin()) || ::isspace(*str.end());
}

//格式化字符串
inline void sprintf(std::wostringstream& os, const wchar_t* s) {
    while (*s) {
        if (*s == '%' && *(++s) != '%')
            throw std::runtime_error("invalid format string: missing arguments");
        os << *s++;
    }
}

template<typename T, typename... Args>
inline
void sprintf(std::wostringstream& os, const wchar_t* s, T value, Args... args) {
    while (*s) {
        if (*s == '%' && *(++s) != '%') {
            os << value;
            sprintf(os, *s ? ++s : s, args...);
            return;
        }
        os << *s++;
    }
    throw std::logic_error("extra arguments provided to sprintf");
}

template<typename T, typename... Args>
inline
std::wstring sprintf(const wchar_t* s, T value, Args... args) {
    std::wostringstream os;
    while (*s) {
        if (*s == '%' && *(++s) != '%') {
            os << value;
            sprintf(os, *s ? ++s : s, args...); // 即便当 *s == 0 也会产生调用，以检测更多的类型参数。
            return os.str();
        }
        os << *s++;
    }
    throw std::logic_error("extra arguments provided to sprintf");
}


//格式化字符串
inline void sprintf(std::ostringstream& os, const char* s) {
    while (*s) {
        if (*s == '%' && *(++s) != '%')
            throw std::runtime_error("invalid format string: missing arguments");
        os << *s++;
    }
}

template<typename T, typename... Args>
inline
void sprintf(std::ostringstream& os, const char* s, T value, Args... args) {
    while (*s) {
        if (*s == '%' && *(++s) != '%') {
            os << value;
            sprintf(os, *s ? ++s : s, args...);
            return;
        }
        os << *s++;
    }
    throw std::logic_error("extra arguments provided to sprintf");
}

template<typename T, typename... Args>
inline
std::string sprintf(const char* s, T value, Args... args) {
    std::ostringstream os;
    while (*s) {
        if (*s == '%' && *(++s) != '%') {
            os << value;
            sprintf(os, *s ? ++s : s, args...); // 即便当 *s == 0 也会产生调用，以检测更多的类型参数。
            return os.str();
        }
        os << *s++;
    }
    throw std::logic_error("extra arguments provided to sprintf");
}

template<typename T>
inline std_string<T> replace_all(const std_string<T>& source, const std_string<T>& from, const std_string<T>& to) {
    typedef typename std_string<T>::size_type size_type;
    size_type pos = 0;
    size_type srcLen = from.size();
    size_type dstLen = to.size();

    std_string<T> str = source;
    while ((pos = str.find(from, pos)) != std_string<T>::npos) {
        str.replace(pos, srcLen, to);
        pos += dstLen;
    }
    return str;
}

template<typename T>
inline std_string<T> get_file_name(const std_string<T>& path) {
    std_string<T> file = 
        Utils::replace_all(path, get_slash<T>(), get_backslash<T>());

    auto pos = file.rfind(get_backslash<T>());
    if (pos != std_string<T>::npos) {
        auto s = file.substr(pos + 1);
        return s;
    }
    return file;
}

//是否是字符串
inline
bool is_alpha(const char* str) {
    if (!str)
        return false;
    auto fun = [](int ch) {
        if ((ch >= 'a' && ch <='z')
            || (ch >= 'A' && ch <= 'Z')) {
            return true;
        }
        return false;
    };
    while (fun(*str)) str++;
    return *str == '\0';
}

//是否是字母或数字
inline
bool is_alnum(const char* str) {
    if (!str)
        return false;

    auto fun = [](int ch) {
        if ((ch >= 'a' && ch <= 'z')
            || (ch >= 'A' && ch <= 'Z')
            || (ch >= '0' && ch <= '9')) {
            return true;
        }
        return false;
    };

    while (fun(*str)) str++;
    return *str == '\0';
}

//是否是0-9数字
inline
bool is_digit(const char* str) {
    if (!str)
        return false;

    auto fun = [](int ch) {
        return ch >= '0' && ch <= '9';
    };
    while (fun(*str)) str++;
    return *str == '\0';
}

inline
bool is_digit(const wchar_t* str) {
    if (!str)
        return false;

    auto fun = [](int ch) {
        return ch >= '0' && ch <= '9';
    };
    while (fun(*str)) str++;
    return *str == '\0';
}

inline
std::vector<std::string> split(const std::string& str, const std::string split_char) {
    using namespace std;
    vector<std::string> vec;
    if (!str.empty()) {
        regex reg(split_char);
        sregex_token_iterator it(str.begin(), str.end(), reg, -1);
        sregex_token_iterator end;
        while (it != end) {
            vec.push_back(*it++);
        }
    }
    return vec;
}

inline
std::vector<std::wstring> split(const std::wstring& str, const std::wstring split_char) {
    using namespace std;
    vector<std::wstring> vec;
    if (!str.empty()) {
        wregex reg(split_char);
        wsregex_token_iterator it(str.begin(), str.end(), reg, -1);
        wsregex_token_iterator end;
        while (it != end) {
            vec.push_back(*it++);
        }
    }
    return vec;
}

NAMESPACE_DEFINE_END(Utils)