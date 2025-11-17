#include "../util/imports.h"
#include "RuntimeValue.h"

namespace std
{
    template<>
    struct hash<bfloat16> {
        std::size_t operator()(const bfloat16& bfloat) const noexcept {
            return std::hash<float>()(static_cast<float>(bfloat));
        }
    };
}

namespace Arc
{
    struct runtime_value_hash {
    private:
        static inline void hash_combine(std::size_t& seed, std::size_t value) {
            seed ^= value + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
    public:
        static inline std::size_t hash(const RuntimeValue& runtimeValue) {
            std::size_t result = runtimeValue.index();

            std::visit([&](auto&& value) {
                using T = std::decay_t<decltype(value)>;

                std::size_t hash = std::hash<T>{}(value);
                
                hash_combine(result, hash);
            }, runtimeValue);

            return result;
        }
    };
}