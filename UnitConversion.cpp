#include "stdafx.h"
#include "UnitConversion.h"


NAMESPACE_DEFINE_BEGIN(Utils)

ByteUnit::ByteUnit() :
    ByteUnit(0) {
}

ByteUnit::ByteUnit(int64_t capacityValue) :
    capacityValue(capacityValue) {
}

ByteUnit::~ByteUnit() {
}

int64_t ByteUnit::ToKB() const {
    return capacityValue / 1024;
}

int64_t ByteUnit::ToMB() const {
    return ToKB() / 1024;
}

int64_t ByteUnit::ToGB() const {
    return ToMB() / 1024;
}

ByteUnit::operator int64_t() const {
    return capacityValue;
}

ByteUnit& ByteUnit::operator= (int64_t capacityValue) {
    this->capacityValue = capacityValue;
    return *this;
}

std::string ByteUnit::ToKBS() const {
    return std::to_string(ToKB());
}

std::string ByteUnit::ToMBS() const {
    return std::to_string(ToMB());
}

std::string ByteUnit::ToGBS() const {
    return std::to_string(ToGB());
}

std::wstring ByteUnit::ToKBWS() const {
    return std::to_wstring(ToKB());
}

std::wstring ByteUnit::ToMBWS() const {
    return std::to_wstring(ToMB());
}

std::wstring ByteUnit::ToGBWS() const {
    return std::to_wstring(ToGB());
}

NAMESPACE_DEFINE_END(Utils)