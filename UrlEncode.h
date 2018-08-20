#pragma once

#include "utils.h"

#include <string>

NAMESPACE_DEFINE_BEGIN(Utils)


std::string UrlEncode(const std::string& str);
std::string UrlDecode(const std::string& str);
std::string UrlEncodeLower(const std::string& str);

NAMESPACE_DEFINE_END(Utils)