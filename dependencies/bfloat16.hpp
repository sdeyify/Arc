#ifndef BFLOAT16_HPP
#define BFLOAT16_HPP

#include <cstdint>
#include <cmath>
#include <limits>
#include <cstring>
#include <iostream>

class bfloat16 {
private:
    uint16_t data;

public:
    bfloat16() : data(0) {}

    // Convert from float
    bfloat16(float f) {
        uint32_t bits;
        std::memcpy(&bits, &f, sizeof(bits));
        // Round to nearest even by adding 0x00008000 before truncating
        bits += 0x00008000;
        data = static_cast<uint16_t>(bits >> 16);
    }

    // Convert to float
    operator float() const {
        uint32_t bits = static_cast<uint32_t>(data) << 16;
        float f;
        std::memcpy(&f, &bits, sizeof(f));
        return f;
    }

    // Arithmetic operators (convert to float, compute, convert back)
    bfloat16 operator+(const bfloat16& other) const {
        return bfloat16(static_cast<float>(*this) + static_cast<float>(other));
    }

    bfloat16 operator-(const bfloat16& other) const {
        return bfloat16(static_cast<float>(*this) - static_cast<float>(other));
    }

    bfloat16 operator*(const bfloat16& other) const {
        return bfloat16(static_cast<float>(*this) * static_cast<float>(other));
    }

    bfloat16 operator/(const bfloat16& other) const {
        return bfloat16(static_cast<float>(*this) / static_cast<float>(other));
    }

    // Comparison operators
    bool operator==(const bfloat16& other) const {
        return float(*this) == float(other);
    }

    bool operator!=(const bfloat16& other) const {
        return !(*this == other);
    }

    bool operator<(const bfloat16& other) const {
        return float(*this) < float(other);
    }

    bool operator<=(const bfloat16& other) const {
        return float(*this) <= float(other);
    }

    bool operator>(const bfloat16& other) const {
        return float(*this) > float(other);
    }

    bool operator>=(const bfloat16& other) const {
        return float(*this) >= float(other);
    }

    // I/O support
    friend std::ostream& operator<<(std::ostream& os, const bfloat16& bf) {
        os << static_cast<float>(bf);
        return os;
    }

    friend std::istream& operator>>(std::istream& is, bfloat16& bf) {
        float f;
        is >> f;
        bf = bfloat16(f);
        return is;
    }

    // Get raw bits (optional)
    uint16_t raw_bits() const { return data; }
};

#endif // BFLOAT16_HPP