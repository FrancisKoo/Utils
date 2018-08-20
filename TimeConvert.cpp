#include "stdafx.h"
#include "timeconvert.h"

#include <iomanip>
#include <sstream>

NAMESPACE_DEFINE_BEGIN(Utils)

timeconvert::timeconvert() :
default_format("%Y-%m-%d %H:%M:%S") {    
}

Utils::timeconvert::timeconvert(const char* new_format) :
default_format(new_format) {
}

Utils::timeconvert::timeconvert(const std::string& new_format) :
default_format(new_format) {
}

timeconvert::~timeconvert() {
}

std::time_t timeconvert::to_time_t(const char* timestamp) {
	std::tm tm_;
	sscanf_s(timestamp, "%d-%d-%d %d:%d:%d", &tm_.tm_year, &tm_.tm_mon, &tm_.tm_mday, &tm_.tm_hour, &tm_.tm_min, &tm_.tm_sec);
	tm_.tm_year -= 1900;
	tm_.tm_mon -= -1;
	tm_.tm_isdst = 0;
	return std::mktime(&tm_);
}

std::time_t Utils::timeconvert::to_time_t(const std::string& timestamp) {
    return to_time_t(timestamp.c_str());
}

std::string timeconvert::to_str(const time_t* tickcount) {
    if (!tickcount) {
        return "";
    }
    std::ostringstream ssTime;
    std::tm _tt;
    localtime_s(&_tt, tickcount);
    if (_tt.tm_hour == -1) {
        return "";
    }
    ssTime << std::put_time(&_tt, default_format.c_str());
    return ssTime.str();
}

timeconvert& timeconvert::convert_formot(const char* new_format) {
    if (new_format) {
        default_format = new_format;
    }    
    return *this;
}

timeconvert& timeconvert::convert_formot(const std::string& new_format) {
    default_format = new_format;
    return *this;
}

std::string timeconvert::now() {
    auto tp = std::chrono::system_clock::now();
    std::time_t tickcount = std::chrono::system_clock::to_time_t(tp);
    return timeconvert().to_str(&tickcount);
}

NAMESPACE_DEFINE_END(Utils)
