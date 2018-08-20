#pragma once

#include <string>
#include <chrono>
#include<ctime>
#include "Utils.h"

NAMESPACE_DEFINE_BEGIN(Utils)

class timeconvert {
public:
    timeconvert();
    explicit timeconvert(const char* new_format);
    explicit timeconvert(const std::string& new_format);

    ~timeconvert();

    std::time_t to_time_t(const char* timestamp);
    std::time_t to_time_t(const std::string& timestamp);
	
    std::string to_str(const time_t* tickcount);

    timeconvert& convert_formot(const char* new_format);
    timeconvert& convert_formot(const std::string& new_format);

    static std::string now();
private:
    std::string default_format;
};

NAMESPACE_DEFINE_END(Utils)
