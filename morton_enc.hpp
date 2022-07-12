#pragma once
#include <bitset>

namespace morton {

template <size_t N>
struct encoder {
    template <class Arg, class...Args>
    std::bitset<N> encode(Arg&& arg, Args&&...args) {
        return encode_<1 + sizeof...(Args), 0>(std::forward<Arg>(arg), std::forward<Args>(args)...);
    }

private:
    
    template <size_t S, size_t L>
    struct mask {
        static std::bitset<N> make_mask() {
            std::bitset<N> result;
            bool bit = true;
            size_t j = 0;
            for (size_t i = 0; i < result.size(); ++i) {
                if (i % L == 0) {
                    bit = j % S == 0;
                    ++j;
                }
                result.set(i, bit);
            }
            return result;
        } 

        static const std::bitset<N> mask_;

        static void apply(std::bitset<N>& value);
    };

    template <size_t S>
    struct mask<S, 1> {
        static std::bitset<N> make_mask() {
            std::bitset<N> result;
            bool bit = true;
            
            for (size_t i = 0; i < result.size(); ++i) {
                bit = i % S == 0;
                result.set(i, bit);
            }
            return result;
        } 

        static const std::bitset<N> mask_;

        static void apply(std::bitset<N>& value);
    };

    template <size_t Stride, size_t Shift, class Arg, class... Args>
    std::bitset<N> encode_(Arg&& arg, Args&&...args) {
        return encode_<Stride, Shift>(std::forward<Arg>(arg)) | encode_<Stride, Shift + 1>(std::forward<Args>(args)...);
    }

    template <size_t Stride, size_t Shift, class Arg>
    std::bitset<N> encode_(Arg&& arg) {
        std::bitset<N> result(std::forward<Arg>(arg));
        mask<Stride, N / 2 + N % 2>::apply(result);
        return result << Shift;
    }
};

template <size_t N>
template <size_t S, size_t L>
const std::bitset<N> encoder<N>::mask<S, L>::mask_ = encoder<N>::mask<S, L>::make_mask();

template <size_t N>
template <size_t S, size_t L>
inline void encoder<N>::mask<S, L>::apply(std::bitset<N>& value) {
    value |= (value << (S * L - L));
    value &= mask_;
    mask<S, L/2>::apply(value);
}

template <size_t N>
template <size_t S>
const std::bitset<N> encoder<N>::mask<S, 1>::mask_ = encoder<N>::mask<S, 1>::make_mask();

template <size_t N>
template <size_t S>
inline void encoder<N>::mask<S, 1>::apply(std::bitset<N>& value) {
    value |= value << (S - 1);
    value &= mask_;
}

}
