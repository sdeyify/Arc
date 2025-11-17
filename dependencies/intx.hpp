#include <iostream>
#include <type_traits>
#include <cstdint>
#include <limits>
#include <stdexcept>

namespace intx
{
    /// The dyn_intx class that allows you to store integers of custom sizes whether they're signed or unsigned.
    /// Unlike the intx class, this class allows you to access the size and the sign of your integers from runtime values.
    class dyn_intx {
        uint64_t data;
        unsigned bits;
        bool is_signed;

        // Mask based on bits count
        uint64_t mask() const {
            if (bits == 64) return ~uint64_t(0);
            return (uint64_t(1) << bits) - 1;
        }

        // Sign extend if signed
        int64_t sign_extend(uint64_t val) const {
            if (!is_signed) return static_cast<int64_t>(val);
            uint64_t sign_bit = uint64_t(1) << (bits - 1);
            if (val & sign_bit) {
                // negative number
                uint64_t extended = val | (~mask());
                return static_cast<int64_t>(extended);
            }
            return static_cast<int64_t>(val);
        }

        void apply_mask() {
            data &= mask();
        }

    public:
        dyn_intx(unsigned bits_, bool is_signed_, int64_t val = 0)
            : bits(bits_), is_signed(is_signed_) 
        {
            if (bits < 1 || bits > 64)
                throw std::invalid_argument("Bits must be between 1 and 64");
            data = static_cast<uint64_t>(val);
            apply_mask();
        }

        // Copy constructor
        dyn_intx(const dyn_intx&) = default;

        // Assignment
        dyn_intx& operator=(const dyn_intx&) = default;

        // Convert to int64_t or uint64_t as appropriate
        int64_t to_int64() const { return sign_extend(data); }
        uint64_t to_uint64() const { return data; }

        // Arithmetic operators
        dyn_intx operator+(const dyn_intx& rhs) const {
            if (bits != rhs.bits || is_signed != rhs.is_signed)
                throw std::runtime_error("Bit width or signedness mismatch");

            uint64_t res;
            if (is_signed) {
                int64_t a = sign_extend(data);
                int64_t b = rhs.sign_extend(rhs.data);
                res = static_cast<uint64_t>(a + b);
            } else {
                res = data + rhs.data;
            }
            dyn_intx result(bits, is_signed, 0);
            result.data = res & mask();
            return result;
        }

        dyn_intx operator-(const dyn_intx& rhs) const {
            if (bits != rhs.bits || is_signed != rhs.is_signed)
                throw std::runtime_error("Bit width or signedness mismatch");

            uint64_t res;
            if (is_signed) {
                int64_t a = sign_extend(data);
                int64_t b = rhs.sign_extend(rhs.data);
                res = static_cast<uint64_t>(a - b);
            } else {
                res = data - rhs.data;
            }
            dyn_intx result(bits, is_signed, 0);
            result.data = res & mask();
            return result;
        }

        dyn_intx operator*(const dyn_intx& rhs) const {
            if (bits != rhs.bits || is_signed != rhs.is_signed)
                throw std::runtime_error("Bit width or signedness mismatch");

            uint64_t res;
            if (is_signed) {
                int64_t a = sign_extend(data);
                int64_t b = rhs.sign_extend(rhs.data);
                res = static_cast<uint64_t>(a * b);
            } else {
                res = data * rhs.data;
            }
            dyn_intx result(bits, is_signed, 0);
            result.data = res & mask();
            return result;
        }

        // Division and modulo (with checks)
        dyn_intx operator/(const dyn_intx& rhs) const {
            if (bits != rhs.bits || is_signed != rhs.is_signed)
                throw std::runtime_error("Bit width or signedness mismatch");
            if (rhs.data == 0)
                throw std::runtime_error("Division by zero");

            uint64_t res;
            if (is_signed) {
                int64_t a = sign_extend(data);
                int64_t b = rhs.sign_extend(rhs.data);
                res = static_cast<uint64_t>(a / b);
            } else {
                res = data / rhs.data;
            }
            dyn_intx result(bits, is_signed, 0);
            result.data = res & mask();
            return result;
        }

        dyn_intx operator%(const dyn_intx& rhs) const {
            if (bits != rhs.bits || is_signed != rhs.is_signed)
                throw std::runtime_error("Bit width or signedness mismatch");
            if (rhs.data == 0)
                throw std::runtime_error("Modulo by zero");

            uint64_t res;
            if (is_signed) {
                int64_t a = sign_extend(data);
                int64_t b = rhs.sign_extend(rhs.data);
                res = static_cast<uint64_t>(a % b);
            } else {
                res = data % rhs.data;
            }
            dyn_intx result(bits, is_signed, 0);
            result.data = res & mask();
            return result;
        }

        // Bitwise operators
        dyn_intx operator&(const dyn_intx& rhs) const {
            if (bits != rhs.bits)
                throw std::runtime_error("Bit width mismatch");
            return dyn_intx(bits, is_signed, data & rhs.data);
        }

        dyn_intx operator|(const dyn_intx& rhs) const {
            if (bits != rhs.bits)
                throw std::runtime_error("Bit width mismatch");
            return dyn_intx(bits, is_signed, data | rhs.data);
        }

        dyn_intx operator^(const dyn_intx& rhs) const {
            if (bits != rhs.bits)
                throw std::runtime_error("Bit width mismatch");
            return dyn_intx(bits, is_signed, data ^ rhs.data);
        }

        dyn_intx operator~() const {
            return dyn_intx(bits, is_signed, (~data) & mask());
        }

        // Shift operators (logical for unsigned, arithmetic for signed)
        dyn_intx operator<<(unsigned shift) const {
            if (shift >= bits) return dyn_intx(bits, is_signed, 0);
            return dyn_intx(bits, is_signed, (data << shift) & mask());
        }

        dyn_intx operator>>(unsigned shift) const {
            if (shift >= bits) {
                if (is_signed && (sign_extend(data) < 0)) {
                    return dyn_intx(bits, is_signed, mask()); // all ones (negative)
                } else {
                    return dyn_intx(bits, is_signed, 0);
                }
            }
            if (is_signed) {
                int64_t val = sign_extend(data);
                int64_t shifted = val >> shift; // arithmetic shift
                return dyn_intx(bits, is_signed, static_cast<uint64_t>(shifted));
            } else {
                return dyn_intx(bits, is_signed, data >> shift);
            }
        }

        // Comparison operators
        bool operator==(const dyn_intx& rhs) const {
            return bits == rhs.bits && is_signed == rhs.is_signed && data == rhs.data;
        }

        bool operator!=(const dyn_intx& rhs) const { return !(*this == rhs); }

        bool operator<(const dyn_intx& rhs) const {
            if (bits != rhs.bits || is_signed != rhs.is_signed)
                throw std::runtime_error("Bit width or signedness mismatch");
            if (is_signed)
                return sign_extend(data) < sign_extend(rhs.data);
            else
                return data < rhs.data;
        }

        bool operator>(const dyn_intx& rhs) const { return rhs < *this; }
        bool operator<=(const dyn_intx& rhs) const { return !(*this > rhs); }
        bool operator>=(const dyn_intx& rhs) const { return !(*this < rhs); }

        void print() const {
            if (is_signed) {
                std::cout << sign_extend(data);
            } else {
                std::cout << data;
            }
        }
    };

    /// The intx class that allows you to store integers of custom sizes whether they're signed or unsigned.
    /// \tparam Bits the width of the custom sized integer.
    /// \tparam Signed whether the integer is signed or not.
    template <unsigned Bits, bool Signed = false>
    class intx {
        static_assert(Bits > 0 && Bits <= 64, "Bits must be between 1 and 64 inclusive");

    private:
        using StorageType = typename std::conditional_t<
            Bits <= 8, uint8_t,
            typename std::conditional_t<
                Bits <= 16, uint16_t,
                typename std::conditional_t<
                    Bits <= 32, uint32_t,
                    uint64_t
                >
            >
        >;

        StorageType data;

        static constexpr StorageType mask = (Bits == 64) ? ~StorageType(0) : (StorageType(1) << Bits) - 1;

        // Sign extend for signed numbers
        static int64_t sign_extend(StorageType val) {
            if constexpr (!Signed) {
                return static_cast<int64_t>(val);
            } else {
                StorageType sign_bit = StorageType(1) << (Bits - 1);
                if (val & sign_bit) {
                    // negative number
                    return static_cast<int64_t>(val | (~mask));
                } else {
                    return static_cast<int64_t>(val);
                }
            }
        }

        // Mask input value to fit bits
        static StorageType mask_value(StorageType val) {
            return val & mask;
        }

    public:
        // Constructors
        intx() : data(0) {}

        template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
        intx(T val) {
            if constexpr (Signed) {
                int64_t sval = static_cast<int64_t>(val);
                // Mask lower bits, sign extend
                data = mask_value(static_cast<StorageType>(sval));
            } else {
                data = mask_value(static_cast<StorageType>(val));
            }
        }

        // Copy constructor and assignment
        intx(const intx&) = default;
        intx& operator=(const intx&) = default;

        // Conversion operators
        operator int64_t() const {
            if constexpr (Signed) {
                return sign_extend(data);
            } else {
                return static_cast<int64_t>(data);
            }
        }

        operator uint64_t() const {
            return static_cast<uint64_t>(data);
        }

        // Arithmetic operators

        intx operator+(const intx& other) const {
            if constexpr (Signed) {
                int64_t res = sign_extend(data) + sign_extend(other.data);
                return intx(res);
            } else {
                StorageType res = data + other.data;
                return intx(mask_value(res));
            }
        }

        intx operator-(const intx& other) const {
            if constexpr (Signed) {
                int64_t res = sign_extend(data) - sign_extend(other.data);
                return intx(res);
            } else {
                StorageType res = data - other.data;
                return intx(mask_value(res));
            }
        }

        intx operator*(const intx& other) const {
            if constexpr (Signed) {
                int64_t res = sign_extend(data) * sign_extend(other.data);
                return intx(res);
            } else {
                StorageType res = data * other.data;
                return intx(mask_value(res));
            }
        }

        intx operator/(const intx& other) const {
            if constexpr (Signed) {
                int64_t denom = sign_extend(other.data);
                if (denom == 0) throw std::runtime_error("Division by zero");
                int64_t res = sign_extend(data) / denom;
                return intx(res);
            } else {
                if (other.data == 0) throw std::runtime_error("Division by zero");
                StorageType res = data / other.data;
                return intx(mask_value(res));
            }
        }

        intx operator%(const intx& other) const {
            if constexpr (Signed) {
                int64_t denom = sign_extend(other.data);
                if (denom == 0) throw std::runtime_error("Modulo by zero");
                int64_t res = sign_extend(data) % denom;
                return intx(res);
            } else {
                if (other.data == 0) throw std::runtime_error("Modulo by zero");
                StorageType res = data % other.data;
                return intx(mask_value(res));
            }
        }

        // Compound assignment operators

        intx& operator+=(const intx& other) {
            *this = *this + other;
            return *this;
        }

        intx& operator-=(const intx& other) {
            *this = *this - other;
            return *this;
        }

        intx& operator*=(const intx& other) {
            *this = *this * other;
            return *this;
        }

        intx& operator/=(const intx& other) {
            *this = *this / other;
            return *this;
        }

        intx& operator%=(const intx& other) {
            *this = *this % other;
            return *this;
        }

        // Bitwise operators

        intx operator&(const intx& other) const {
            return intx(data & other.data);
        }

        intx operator|(const intx& other) const {
            return intx(data | other.data);
        }

        intx operator^(const intx& other) const {
            return intx(data ^ other.data);
        }

        intx operator~() const {
            return intx(mask_value(~data));
        }

        intx& operator&=(const intx& other) {
            data &= other.data;
            return *this;
        }

        intx& operator|=(const intx& other) {
            data |= other.data;
            return *this;
        }

        intx& operator^=(const intx& other) {
            data ^= other.data;
            return *this;
        }

        // Bitshift operators

        intx operator<<(unsigned shift) const {
            if (shift >= Bits) return intx(0);
            if constexpr (Signed) {
                int64_t val = sign_extend(data);
                int64_t res = val << shift;
                return intx(res);
            } else {
                StorageType res = data << shift;
                return intx(mask_value(res));
            }
        }

        intx operator>>(unsigned shift) const {
            if (shift >= Bits) {
                if constexpr (Signed) {
                    // arithmetic shift for signed
                    int64_t val = sign_extend(data);
                    // shift beyond bits means all bits are sign bit
                    if (val < 0) return intx(-1);
                    else return intx(0);
                } else {
                    return intx(0);
                }
            }
            if constexpr (Signed) {
                int64_t val = sign_extend(data);
                int64_t res = val >> shift; // arithmetic shift
                return intx(res);
            } else {
                StorageType res = data >> shift;
                return intx(mask_value(res));
            }
        }

        intx& operator<<=(unsigned shift) {
            *this = *this << shift;
            return *this;
        }

        intx& operator>>=(unsigned shift) {
            *this = *this >> shift;
            return *this;
        }

        // Comparison operators

        bool operator==(const intx& other) const {
            return data == other.data;
        }

        bool operator!=(const intx& other) const {
            return data != other.data;
        }

        bool operator<(const intx& other) const {
            if constexpr (Signed) {
                return sign_extend(data) < sign_extend(other.data);
            } else {
                return data < other.data;
            }
        }

        bool operator>(const intx& other) const {
            if constexpr (Signed) {
                return sign_extend(data) > sign_extend(other.data);
            } else {
                return data > other.data;
            }
        }

        bool operator<=(const intx& other) const {
            return !(*this > other);
        }

        bool operator>=(const intx& other) const {
            return !(*this < other);
        }

        // For debugging: print as int64_t or uint64_t
        void print() const {
            if constexpr (Signed) {
                std::cout << sign_extend(data);
            } else {
                std::cout << static_cast<uint64_t>(data);
            }
        }
    };
}