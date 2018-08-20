#pragma once

#include "utils.h"

#include <string>
#include <cstdint>
NAMESPACE_DEFINE_BEGIN(Utils)

class ByteUnit {
public:
    ByteUnit();
    explicit ByteUnit(int64_t capacityValue);
    ~ByteUnit();
    int64_t ToKB() const;
    int64_t ToMB() const;
    int64_t ToGB() const;

    std::string ToKBS() const;
    std::string ToMBS() const;
    std::string ToGBS() const;

    std::wstring ToKBWS() const;
    std::wstring ToMBWS() const;
    std::wstring ToGBWS() const;

    operator int64_t() const;
    ByteUnit& operator= (int64_t capacityValue);
protected:
    int64_t capacityValue;
};

NAMESPACE_DEFINE_END(Utils)