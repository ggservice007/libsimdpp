/*  Copyright (C) 2011-2014  Povilas Kanapickas <povilas@radix.lt>

    Distributed under the Boost Software License, Version 1.0.
        (See accompanying file LICENSE_1_0.txt or copy at
            http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef LIBSIMDPP_SIMDPP_DETAIL_INSN_STORE_H
#define LIBSIMDPP_SIMDPP_DETAIL_INSN_STORE_H

#ifndef LIBSIMDPP_SIMD_H
    #error "This file must be included through simd.h"
#endif

#include <simdpp/types.h>
#include <simdpp/detail/null/memory.h>
#include <simdpp/detail/align.h>

namespace simdpp {
#ifndef SIMDPP_DOXYGEN
namespace SIMDPP_ARCH_NAMESPACE {
#endif
namespace detail {
namespace insn {

inline void i_store(char* p, uint8x16 a)
{
    p = detail::assume_aligned(p, 16);
#if SIMDPP_USE_NULL
    detail::null::store(p, a);
#elif SIMDPP_USE_SSE2
    _mm_store_si128(reinterpret_cast<__m128i*>(p), a);
#elif SIMDPP_USE_NEON
    vst1q_u64(reinterpret_cast<uint64_t*>(p), vreinterpretq_u64_u32(a));
#elif SIMDPP_USE_ALTIVEC
    vec_stl((__vector uint8_t)a, 0, reinterpret_cast<uint8_t*>(p));
#endif
}

#if SIMDPP_USE_AVX2
inline void i_store(char* p, uint8x32 a)
{
    p = detail::assume_aligned(p, 32);
    _mm256_store_si256(reinterpret_cast<__m256i*>(p), a);
}
#endif

// -----------------------------------------------------------------------------

inline void i_store(char* p, uint16<8> a)
{
    i_store(p, uint8<16>(a));
}

#if SIMDPP_USE_AVX2
inline void i_store(char* p, uint16<16> a)
{
    i_store(p, uint8<32>(a));
}
#endif

// -----------------------------------------------------------------------------

inline void i_store(char* p, uint32<4> a)
{
    i_store(p, uint8<16>(a));
}

#if SIMDPP_USE_AVX2
inline void i_store(char* p, uint32<8> a)
{
    i_store(p, uint8<32>(a));
}
#endif

#if SIMDPP_USE_AVX512
inline void i_store(char* p, uint32<16> a)
{
    p = detail::assume_aligned(p, 64);
    _mm512_store_epi32(p, a);
}
#endif

// -----------------------------------------------------------------------------

inline void i_store(char* p, uint64<2> a)
{
    i_store(p, uint8<16>(a));
}

#if SIMDPP_USE_AVX2
inline void i_store(char* p, uint64<4> a)
{
    i_store(p, uint8<32>(a));
}
#endif

#if SIMDPP_USE_AVX512
inline void i_store(char* p, uint64<8> a)
{
    p = detail::assume_aligned(p, 64);
    _mm512_store_epi64(p, a);
}
#endif

// -----------------------------------------------------------------------------

inline void i_store(char* p, float32x4 a)
{
    float* q = reinterpret_cast<float*>(p);
    q = detail::assume_aligned(q, 16);
#if SIMDPP_USE_NULL
    detail::null::store(q, a);
#elif SIMDPP_USE_SSE2
    _mm_store_ps(q, a);
#elif SIMDPP_USE_NEON
    vst1q_f32(q, a);
#elif SIMDPP_USE_ALTIVEC
    vec_stl((__vector float)a, 0, q);
#endif
}

#if SIMDPP_USE_AVX
inline void i_store(char* p, float32x8 a)
{
    float* q = reinterpret_cast<float*>(p);
    q = detail::assume_aligned(q, 32);
    _mm256_store_ps(q, a);
}
#endif

#if SIMDPP_USE_AVX512
inline void i_store(char* p, float32<16> a)
{
    p = detail::assume_aligned(p, 64);
    _mm512_store_ps(p, a);
}
#endif

// -----------------------------------------------------------------------------

inline void i_store(char* p, float64x2 a)
{
    p = detail::assume_aligned(p, 16);
#if SIMDPP_USE_NULL || SIMDPP_USE_NEON || SIMDPP_USE_ALTIVEC
    detail::null::store(p, a);
#elif SIMDPP_USE_SSE2
    _mm_store_pd(reinterpret_cast<double*>(p), a);
#endif
}

#if SIMDPP_USE_AVX
inline void i_store(char* p, float64x4 a)
{
    p = detail::assume_aligned(p, 32);
    _mm256_store_pd(reinterpret_cast<double*>(p), a);
}
#endif

#if SIMDPP_USE_AVX512
inline void i_store(char* p, float64<8> a)
{
    p = detail::assume_aligned(p, 64);
    _mm512_store_pd(p, a);
}
#endif

// -----------------------------------------------------------------------------

template<class V>
void v_store(char* p, V a)
{
    unsigned veclen = sizeof(typename V::base_vector_type);

    p = detail::assume_aligned(p, veclen);
    for (unsigned i = 0; i < V::vec_length; ++i) {
        i_store(p, a.vec(i));
        p += veclen;
    }
}

template<unsigned N>
void i_store(char* p, uint8<N> a) { v_store(p, a); }
template<unsigned N>
void i_store(char* p, uint16<N> a) { v_store(p, a); }
template<unsigned N>
void i_store(char* p, uint32<N> a) { v_store(p, a); }
template<unsigned N>
void i_store(char* p, uint64<N> a) { v_store(p, a); }
template<unsigned N>
void i_store(char* p, float32<N> a){ v_store(p, a); }
template<unsigned N>
void i_store(char* p, float64<N> a){ v_store(p, a); }


} // namespace insn
} // namespace detail
#ifndef SIMDPP_DOXYGEN
} // namespace SIMDPP_ARCH_NAMESPACE
#endif
} // namespace simdpp

#endif

