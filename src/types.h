#pragma once

using S8  = char;
using S16 = short;
using S32 = int;
using S64 = long long;

using U8  = unsigned char;
using U16 = unsigned short;
using U32 = unsigned int;
using U64 = unsigned long long;


template<class Under, class Type>
class opaque_type {
public:
    explicit opaque_type(Under val)
        : value_(val) {}

    explicit operator Under() const noexcept {
        return value_;
    }

private:
    Under value_;
};

#define OPAQUE_TYPE(U, T)                     \
struct LineFlags : public opaque_type<U, T> { \
    using base = opaque_type<U, T>;           \
    using base::base;                         \
};
