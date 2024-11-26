//   Copyright Naoki Shibata and contributors 2010 - 2020.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef __SLEEF_H__
#define __SLEEF_H__

#define SLEEF_VERSION_MAJOR 3
#define SLEEF_VERSION_MINOR 6
#define SLEEF_VERSION_PATCHLEVEL 0

#include <stddef.h>
#include <stdint.h>

#if (defined(__GNUC__) || defined(__CLANG__)) && !defined(__INTEL_COMPILER)
#define CONST const
#else
#define CONST
#endif

#if defined(__AVX2__) || defined(__aarch64__) || defined(__arm__) || defined(__powerpc64__) || defined(__zarch__)
#ifndef FP_FAST_FMA
#define FP_FAST_FMA
#endif
#ifndef FP_FAST_FMAF
#define FP_FAST_FMAF
#endif
#endif

#if defined(_MSC_VER) && !defined(__STDC__)
#define __STDC__ 1
#endif

#if (defined(__MINGW32__) || defined(__MINGW64__) || defined(__CYGWIN__) || defined(_MSC_VER)) && !defined(SLEEF_STATIC_LIBS)
#ifdef IMPORT_IS_EXPORT
#define IMPORT __declspec(dllexport)
#else // #ifdef IMPORT_IS_EXPORT
#define IMPORT __declspec(dllimport)
#if (defined(_MSC_VER))
#pragma comment(lib,"sleef.lib")
#endif // #if (defined(_MSC_VER))
#endif // #ifdef IMPORT_IS_EXPORT
#else // #if (defined(__MINGW32__) || defined(__MINGW64__) || defined(__CYGWIN__) || defined(_MSC_VER)) && !defined(SLEEF_STATIC_LIBS)
#define IMPORT
#endif // #if (defined(__MINGW32__) || defined(__MINGW64__) || defined(__CYGWIN__) || defined(_MSC_VER)) && !defined(SLEEF_STATIC_LIBS)

#if (defined(__GNUC__) || defined(__CLANG__)) && (defined(__i386__) || defined(__x86_64__))
#include <x86intrin.h>
#endif

#if (defined(_MSC_VER))
#include <intrin.h>
#endif

#if defined(__ARM_NEON__) || defined(__ARM_NEON)
#include <arm_neon.h>
#endif

#if defined(__ARM_FEATURE_SVE)
#include <arm_sve.h>
#endif

#if defined(__VSX__) && defined(__PPC64__) && defined(__LITTLE_ENDIAN__)
#include <altivec.h>
typedef __vector double       SLEEF_VECTOR_DOUBLE;
typedef __vector float        SLEEF_VECTOR_FLOAT;
typedef __vector int          SLEEF_VECTOR_INT;
typedef __vector unsigned int SLEEF_VECTOR_UINT;
typedef __vector long long SLEEF_VECTOR_LONGLONG;
typedef __vector unsigned long long SLEEF_VECTOR_ULONGLONG;
#endif

#if defined(__VX__) && defined(__VEC__)
#ifndef SLEEF_VECINTRIN_H_INCLUDED
#include <vecintrin.h>
#define SLEEF_VECINTRIN_H_INCLUDED
#endif
typedef __vector double       SLEEF_VECTOR_DOUBLE;
typedef __vector float        SLEEF_VECTOR_FLOAT;
typedef __vector int          SLEEF_VECTOR_INT;
typedef __vector unsigned int SLEEF_VECTOR_UINT;
typedef __vector long long SLEEF_VECTOR_LONGLONG;
typedef __vector unsigned long long SLEEF_VECTOR_ULONGLONG;
#endif

//

#ifndef SLEEF_FP_ILOGB0
#define SLEEF_FP_ILOGB0 ((int)-2147483648)
#endif

#ifndef SLEEF_FP_ILOGBNAN
#define SLEEF_FP_ILOGBNAN ((int)2147483647)
#endif

//

IMPORT void *Sleef_malloc(size_t z);
IMPORT void Sleef_free(void *ptr);
IMPORT uint64_t Sleef_currentTimeMicros();

#if defined(__i386__) || defined(__x86_64__) || defined(_MSC_VER)
IMPORT void Sleef_x86CpuID(int32_t out[4], uint32_t eax, uint32_t ecx);
#endif

//

#ifndef Sleef_double2_DEFINED
#define Sleef_double2_DEFINED
typedef struct {
  double x, y;
} Sleef_double2;
#endif

#ifndef Sleef_float2_DEFINED
#define Sleef_float2_DEFINED
typedef struct {
  float x, y;
} Sleef_float2;
#endif

#ifndef Sleef_longdouble2_DEFINED
#define Sleef_longdouble2_DEFINED
typedef struct {
  long double x, y;
} Sleef_longdouble2;
#endif

#if !defined(Sleef_quad_DEFINED)
#define Sleef_quad_DEFINED
#if defined(__SIZEOF_FLOAT128__) || (defined(__linux__) && defined(__GNUC__) && (defined(__i386__) || defined(__x86_64__))) || (defined(__PPC64__) && defined(__GNUC__) && !defined(__clang__) && __GNUC__ >= 8)
typedef __float128 Sleef_quad;
#define SLEEF_QUAD_C(x) (x ## Q)
//#elif defined(__SIZEOF_LONG_DOUBLE__) && defined(__aarch64__)
//typedef long double Sleef_quad;
//#define SLEEF_QUAD_C(x) (x ## L)
#else
typedef struct { uint64_t x, y; } Sleef_quad;
#endif
#endif

#if !defined(Sleef_quad2_DEFINED)
#define Sleef_quad2_DEFINED
typedef union {
  struct {
    Sleef_quad x, y;
  };
  Sleef_quad s[2];
} Sleef_quad2;
#endif

#ifdef __cplusplus
extern "C"
{
#endif

IMPORT CONST double Sleef_sin_u35(double);
IMPORT CONST double Sleef_cos_u35(double);
IMPORT CONST Sleef_double2 Sleef_sincos_u35(double);
IMPORT CONST double Sleef_tan_u35(double);
IMPORT CONST double Sleef_asin_u35(double);
IMPORT CONST double Sleef_acos_u35(double);
IMPORT CONST double Sleef_atan_u35(double);
IMPORT CONST double Sleef_atan2_u35(double, double);
IMPORT CONST double Sleef_log_u35(double);
IMPORT CONST double Sleef_cbrt_u35(double);
IMPORT CONST double Sleef_sin_u10(double);
IMPORT CONST double Sleef_cos_u10(double);
IMPORT CONST Sleef_double2 Sleef_sincos_u10(double);
IMPORT CONST double Sleef_tan_u10(double);
IMPORT CONST double Sleef_asin_u10(double);
IMPORT CONST double Sleef_acos_u10(double);
IMPORT CONST double Sleef_atan_u10(double);
IMPORT CONST double Sleef_atan2_u10(double, double);
IMPORT CONST double Sleef_log_u10(double);
IMPORT CONST double Sleef_cbrt_u10(double);
IMPORT CONST double Sleef_exp_u10(double);
IMPORT CONST double Sleef_pow_u10(double, double);
IMPORT CONST double Sleef_sinh_u10(double);
IMPORT CONST double Sleef_cosh_u10(double);
IMPORT CONST double Sleef_tanh_u10(double);
IMPORT CONST double Sleef_sinh_u35(double);
IMPORT CONST double Sleef_cosh_u35(double);
IMPORT CONST double Sleef_tanh_u35(double);
IMPORT CONST double Sleef_asinh_u10(double);
IMPORT CONST double Sleef_acosh_u10(double);
IMPORT CONST double Sleef_atanh_u10(double);
IMPORT CONST double Sleef_exp2_u10(double);
IMPORT CONST double Sleef_exp10_u10(double);
IMPORT CONST double Sleef_exp2_u35(double);
IMPORT CONST double Sleef_exp10_u35(double);
IMPORT CONST double Sleef_expm1_u10(double);
IMPORT CONST double Sleef_log10_u10(double);
IMPORT CONST double Sleef_log2_u10(double);
IMPORT CONST double Sleef_log2_u35(double);
IMPORT CONST double Sleef_log1p_u10(double);
IMPORT CONST Sleef_double2 Sleef_sincospi_u05(double);
IMPORT CONST Sleef_double2 Sleef_sincospi_u35(double);
IMPORT CONST double Sleef_sinpi_u05(double);
IMPORT CONST double Sleef_cospi_u05(double);
IMPORT CONST double Sleef_ldexp(double, int);
IMPORT CONST int Sleef_ilogb(double);
IMPORT CONST double Sleef_fma(double, double, double);
IMPORT CONST double Sleef_sqrt(double);
IMPORT CONST double Sleef_sqrt_u05(double);
IMPORT CONST double Sleef_sqrt_u35(double);

IMPORT CONST double Sleef_hypot_u05(double, double);
IMPORT CONST double Sleef_hypot_u35(double, double);

IMPORT CONST double Sleef_fabs(double);
IMPORT CONST double Sleef_copysign(double, double);
IMPORT CONST double Sleef_fmax(double, double);
IMPORT CONST double Sleef_fmin(double, double);
IMPORT CONST double Sleef_fdim(double, double);
IMPORT CONST double Sleef_trunc(double);
IMPORT CONST double Sleef_floor(double);
IMPORT CONST double Sleef_ceil(double);
IMPORT CONST double Sleef_round(double);
IMPORT CONST double Sleef_rint(double);
IMPORT CONST double Sleef_nextafter(double, double);
IMPORT CONST double Sleef_frfrexp(double);
IMPORT CONST int Sleef_expfrexp(double);
IMPORT CONST double Sleef_fmod(double, double);
IMPORT CONST double Sleef_remainder(double, double);
IMPORT CONST Sleef_double2 Sleef_modf(double);

IMPORT CONST double Sleef_lgamma_u10(double);
IMPORT CONST double Sleef_tgamma_u10(double);
IMPORT CONST double Sleef_erf_u10(double);
IMPORT CONST double Sleef_erfc_u15(double);

IMPORT CONST float Sleef_sinf_u35(float);
IMPORT CONST float Sleef_cosf_u35(float);
IMPORT CONST Sleef_float2 Sleef_sincosf_u35(float);
IMPORT CONST float Sleef_tanf_u35(float);
IMPORT CONST float Sleef_asinf_u35(float);
IMPORT CONST float Sleef_acosf_u35(float);
IMPORT CONST float Sleef_atanf_u35(float);
IMPORT CONST float Sleef_atan2f_u35(float, float);
IMPORT CONST float Sleef_logf_u35(float);
IMPORT CONST float Sleef_cbrtf_u35(float);
IMPORT CONST float Sleef_sinf_u10(float);
IMPORT CONST float Sleef_cosf_u10(float);
IMPORT CONST Sleef_float2 Sleef_sincosf_u10(float);
IMPORT CONST float Sleef_fastsinf_u3500(float);
IMPORT CONST float Sleef_fastcosf_u3500(float);
IMPORT CONST float Sleef_tanf_u10(float);
IMPORT CONST float Sleef_asinf_u10(float);
IMPORT CONST float Sleef_acosf_u10(float);
IMPORT CONST float Sleef_atanf_u10(float);
IMPORT CONST float Sleef_atan2f_u10(float, float);
IMPORT CONST float Sleef_logf_u10(float);
IMPORT CONST float Sleef_cbrtf_u10(float);
IMPORT CONST float Sleef_expf_u10(float);
IMPORT CONST float Sleef_powf_u10(float, float);
IMPORT CONST float Sleef_fastpowf_u3500(float, float);
IMPORT CONST float Sleef_sinhf_u10(float);
IMPORT CONST float Sleef_coshf_u10(float);
IMPORT CONST float Sleef_tanhf_u10(float);
IMPORT CONST float Sleef_sinhf_u35(float);
IMPORT CONST float Sleef_coshf_u35(float);
IMPORT CONST float Sleef_tanhf_u35(float);
IMPORT CONST float Sleef_asinhf_u10(float);
IMPORT CONST float Sleef_acoshf_u10(float);
IMPORT CONST float Sleef_atanhf_u10(float);
IMPORT CONST float Sleef_exp2f_u10(float);
IMPORT CONST float Sleef_exp10f_u10(float);
IMPORT CONST float Sleef_exp2f_u35(float);
IMPORT CONST float Sleef_exp10f_u35(float);
IMPORT CONST float Sleef_expm1f_u10(float);
IMPORT CONST float Sleef_log10f_u10(float);
IMPORT CONST float Sleef_log2f_u10(float);
IMPORT CONST float Sleef_log2f_u35(float);
IMPORT CONST float Sleef_log1pf_u10(float);
IMPORT CONST Sleef_float2 Sleef_sincospif_u05(float);
IMPORT CONST Sleef_float2 Sleef_sincospif_u35(float);
IMPORT CONST float Sleef_sinpif_u05(float d);
IMPORT CONST float Sleef_cospif_u05(float d);
IMPORT CONST float Sleef_ldexpf(float, int);
IMPORT CONST int Sleef_ilogbf(float);
IMPORT CONST float Sleef_fmaf(float, float, float);
IMPORT CONST float Sleef_sqrtf(float);
IMPORT CONST float Sleef_sqrtf_u05(float);
IMPORT CONST float Sleef_sqrtf_u35(float);

IMPORT CONST float Sleef_hypotf_u05(float, float);
IMPORT CONST float Sleef_hypotf_u35(float, float);

IMPORT CONST float Sleef_fabsf(float);
IMPORT CONST float Sleef_copysignf(float, float);
IMPORT CONST float Sleef_fmaxf(float, float);
IMPORT CONST float Sleef_fminf(float, float);
IMPORT CONST float Sleef_fdimf(float, float);
IMPORT CONST float Sleef_truncf(float);
IMPORT CONST float Sleef_floorf(float);
IMPORT CONST float Sleef_ceilf(float);
IMPORT CONST float Sleef_roundf(float);
IMPORT CONST float Sleef_rintf(float);
IMPORT CONST float Sleef_nextafterf(float, float);
IMPORT CONST float Sleef_frfrexpf(float);
IMPORT CONST int Sleef_expfrexpf(float);
IMPORT CONST float Sleef_fmodf(float, float);
IMPORT CONST float Sleef_remainderf(float, float);
IMPORT CONST Sleef_float2 Sleef_modff(float);

IMPORT CONST float Sleef_lgammaf_u10(float);
IMPORT CONST float Sleef_tgammaf_u10(float);
IMPORT CONST float Sleef_erff_u10(float);
IMPORT CONST float Sleef_erfcf_u15(float);

IMPORT CONST Sleef_longdouble2 Sleef_sincospil_u05(long double);
IMPORT CONST Sleef_longdouble2 Sleef_sincospil_u35(long double);

#if defined(Sleef_quad2_DEFINED)
IMPORT CONST Sleef_quad2 Sleef_sincospiq_u05(Sleef_quad);
IMPORT CONST Sleef_quad2 Sleef_sincospiq_u35(Sleef_quad);
#endif
#ifdef __ARM_NEON__

#ifndef Sleef_float32x4_t_2_DEFINED
typedef struct {
  float32x4_t x, y;
} Sleef_float32x4_t_2;
#define Sleef_float32x4_t_2_DEFINED
#endif

IMPORT CONST float32x4_t Sleef_sinf4_u35(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_sinf4_u35(float32x4_t);
IMPORT CONST float32x4_t Sleef_cosf4_u35(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_cosf4_u35(float32x4_t);
IMPORT CONST Sleef_float32x4_t_2 Sleef_sincosf4_u35(float32x4_t);
IMPORT CONST Sleef_float32x4_t_2 Sleef_cinz_sincosf4_u35(float32x4_t);
IMPORT CONST float32x4_t Sleef_tanf4_u35(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_tanf4_u35(float32x4_t);
IMPORT CONST float32x4_t Sleef_asinf4_u35(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_asinf4_u35(float32x4_t);
IMPORT CONST float32x4_t Sleef_acosf4_u35(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_acosf4_u35(float32x4_t);
IMPORT CONST float32x4_t Sleef_atanf4_u35(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_atanf4_u35(float32x4_t);
IMPORT CONST float32x4_t Sleef_atan2f4_u35(float32x4_t, float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_atan2f4_u35(float32x4_t, float32x4_t);
IMPORT CONST float32x4_t Sleef_logf4_u35(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_logf4_u35(float32x4_t);
IMPORT CONST float32x4_t Sleef_cbrtf4_u35(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_cbrtf4_u35(float32x4_t);
IMPORT CONST float32x4_t Sleef_sinf4_u10(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_sinf4_u10(float32x4_t);
IMPORT CONST float32x4_t Sleef_cosf4_u10(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_cosf4_u10(float32x4_t);
IMPORT CONST Sleef_float32x4_t_2 Sleef_sincosf4_u10(float32x4_t);
IMPORT CONST Sleef_float32x4_t_2 Sleef_cinz_sincosf4_u10(float32x4_t);
IMPORT CONST float32x4_t Sleef_tanf4_u10(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_tanf4_u10(float32x4_t);
IMPORT CONST float32x4_t Sleef_asinf4_u10(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_asinf4_u10(float32x4_t);
IMPORT CONST float32x4_t Sleef_acosf4_u10(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_acosf4_u10(float32x4_t);
IMPORT CONST float32x4_t Sleef_atanf4_u10(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_atanf4_u10(float32x4_t);
IMPORT CONST float32x4_t Sleef_atan2f4_u10(float32x4_t, float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_atan2f4_u10(float32x4_t, float32x4_t);
IMPORT CONST float32x4_t Sleef_logf4_u10(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_logf4_u10(float32x4_t);
IMPORT CONST float32x4_t Sleef_cbrtf4_u10(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_cbrtf4_u10(float32x4_t);
IMPORT CONST float32x4_t Sleef_expf4_u10(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_expf4_u10(float32x4_t);
IMPORT CONST float32x4_t Sleef_powf4_u10(float32x4_t, float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_powf4_u10(float32x4_t, float32x4_t);
IMPORT CONST float32x4_t Sleef_sinhf4_u10(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_sinhf4_u10(float32x4_t);
IMPORT CONST float32x4_t Sleef_coshf4_u10(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_coshf4_u10(float32x4_t);
IMPORT CONST float32x4_t Sleef_tanhf4_u10(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_tanhf4_u10(float32x4_t);
IMPORT CONST float32x4_t Sleef_sinhf4_u35(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_sinhf4_u35(float32x4_t);
IMPORT CONST float32x4_t Sleef_coshf4_u35(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_coshf4_u35(float32x4_t);
IMPORT CONST float32x4_t Sleef_tanhf4_u35(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_tanhf4_u35(float32x4_t);
IMPORT CONST float32x4_t Sleef_fastsinf4_u3500(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_fastsinf4_u3500(float32x4_t);
IMPORT CONST float32x4_t Sleef_fastcosf4_u3500(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_fastcosf4_u3500(float32x4_t);
IMPORT CONST float32x4_t Sleef_fastpowf4_u3500(float32x4_t, float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_fastpowf4_u3500(float32x4_t, float32x4_t);
IMPORT CONST float32x4_t Sleef_asinhf4_u10(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_asinhf4_u10(float32x4_t);
IMPORT CONST float32x4_t Sleef_acoshf4_u10(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_acoshf4_u10(float32x4_t);
IMPORT CONST float32x4_t Sleef_atanhf4_u10(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_atanhf4_u10(float32x4_t);
IMPORT CONST float32x4_t Sleef_exp2f4_u10(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_exp2f4_u10(float32x4_t);
IMPORT CONST float32x4_t Sleef_exp2f4_u35(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_exp2f4_u35(float32x4_t);
IMPORT CONST float32x4_t Sleef_exp10f4_u10(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_exp10f4_u10(float32x4_t);
IMPORT CONST float32x4_t Sleef_exp10f4_u35(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_exp10f4_u35(float32x4_t);
IMPORT CONST float32x4_t Sleef_expm1f4_u10(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_expm1f4_u10(float32x4_t);
IMPORT CONST float32x4_t Sleef_log10f4_u10(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_log10f4_u10(float32x4_t);
IMPORT CONST float32x4_t Sleef_log2f4_u10(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_log2f4_u10(float32x4_t);
IMPORT CONST float32x4_t Sleef_log2f4_u35(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_log2f4_u35(float32x4_t);
IMPORT CONST float32x4_t Sleef_log1pf4_u10(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_log1pf4_u10(float32x4_t);
IMPORT CONST Sleef_float32x4_t_2 Sleef_sincospif4_u05(float32x4_t);
IMPORT CONST Sleef_float32x4_t_2 Sleef_cinz_sincospif4_u05(float32x4_t);
IMPORT CONST Sleef_float32x4_t_2 Sleef_sincospif4_u35(float32x4_t);
IMPORT CONST Sleef_float32x4_t_2 Sleef_cinz_sincospif4_u35(float32x4_t);
IMPORT CONST float32x4_t Sleef_sinpif4_u05(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_sinpif4_u05(float32x4_t);
IMPORT CONST float32x4_t Sleef_cospif4_u05(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_cospif4_u05(float32x4_t);
IMPORT CONST float32x4_t Sleef_fmaf4(float32x4_t, float32x4_t, float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_fmaf4(float32x4_t, float32x4_t, float32x4_t);
IMPORT CONST float32x4_t Sleef_sqrtf4(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_sqrtf4(float32x4_t);
IMPORT CONST float32x4_t Sleef_sqrtf4_u05(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_sqrtf4_u05(float32x4_t);
IMPORT CONST float32x4_t Sleef_sqrtf4_u35(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_sqrtf4_u35(float32x4_t);
IMPORT CONST float32x4_t Sleef_hypotf4_u05(float32x4_t, float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_hypotf4_u05(float32x4_t, float32x4_t);
IMPORT CONST float32x4_t Sleef_hypotf4_u35(float32x4_t, float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_hypotf4_u35(float32x4_t, float32x4_t);
IMPORT CONST float32x4_t Sleef_fabsf4(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_fabsf4(float32x4_t);
IMPORT CONST float32x4_t Sleef_copysignf4(float32x4_t, float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_copysignf4(float32x4_t, float32x4_t);
IMPORT CONST float32x4_t Sleef_fmaxf4(float32x4_t, float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_fmaxf4(float32x4_t, float32x4_t);
IMPORT CONST float32x4_t Sleef_fminf4(float32x4_t, float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_fminf4(float32x4_t, float32x4_t);
IMPORT CONST float32x4_t Sleef_fdimf4(float32x4_t, float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_fdimf4(float32x4_t, float32x4_t);
IMPORT CONST float32x4_t Sleef_truncf4(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_truncf4(float32x4_t);
IMPORT CONST float32x4_t Sleef_floorf4(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_floorf4(float32x4_t);
IMPORT CONST float32x4_t Sleef_ceilf4(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_ceilf4(float32x4_t);
IMPORT CONST float32x4_t Sleef_roundf4(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_roundf4(float32x4_t);
IMPORT CONST float32x4_t Sleef_rintf4(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_rintf4(float32x4_t);
IMPORT CONST float32x4_t Sleef_nextafterf4(float32x4_t, float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_nextafterf4(float32x4_t, float32x4_t);
IMPORT CONST float32x4_t Sleef_frfrexpf4(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_frfrexpf4(float32x4_t);
IMPORT CONST float32x4_t Sleef_fmodf4(float32x4_t, float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_fmodf4(float32x4_t, float32x4_t);
IMPORT CONST float32x4_t Sleef_remainderf4(float32x4_t, float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_remainderf4(float32x4_t, float32x4_t);
IMPORT CONST Sleef_float32x4_t_2 Sleef_modff4(float32x4_t);
IMPORT CONST Sleef_float32x4_t_2 Sleef_cinz_modff4(float32x4_t);
IMPORT CONST float32x4_t Sleef_lgammaf4_u10(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_lgammaf4_u10(float32x4_t);
IMPORT CONST float32x4_t Sleef_tgammaf4_u10(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_tgammaf4_u10(float32x4_t);
IMPORT CONST float32x4_t Sleef_erff4_u10(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_erff4_u10(float32x4_t);
IMPORT CONST float32x4_t Sleef_erfcf4_u15(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_erfcf4_u15(float32x4_t);
IMPORT CONST int Sleef_getIntf4(int);
IMPORT CONST int Sleef_cinz_getIntf4(int);
IMPORT CONST void *Sleef_getPtrf4(int);
IMPORT CONST void *Sleef_cinz_getPtrf4(int);
#endif
#ifdef __ARM_NEON__

#ifndef Sleef_float32x4_t_2_DEFINED
typedef struct {
  float32x4_t x, y;
} Sleef_float32x4_t_2;
#define Sleef_float32x4_t_2_DEFINED
#endif

IMPORT CONST float32x4_t Sleef_sinf4_u35neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_sinf4_u35neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_cosf4_u35neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_cosf4_u35neon(float32x4_t);
IMPORT CONST Sleef_float32x4_t_2 Sleef_sincosf4_u35neon(float32x4_t);
IMPORT CONST Sleef_float32x4_t_2 Sleef_cinz_sincosf4_u35neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_tanf4_u35neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_tanf4_u35neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_asinf4_u35neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_asinf4_u35neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_acosf4_u35neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_acosf4_u35neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_atanf4_u35neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_atanf4_u35neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_atan2f4_u35neon(float32x4_t, float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_atan2f4_u35neon(float32x4_t, float32x4_t);
IMPORT CONST float32x4_t Sleef_logf4_u35neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_logf4_u35neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_cbrtf4_u35neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_cbrtf4_u35neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_sinf4_u10neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_sinf4_u10neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_cosf4_u10neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_cosf4_u10neon(float32x4_t);
IMPORT CONST Sleef_float32x4_t_2 Sleef_sincosf4_u10neon(float32x4_t);
IMPORT CONST Sleef_float32x4_t_2 Sleef_cinz_sincosf4_u10neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_tanf4_u10neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_tanf4_u10neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_asinf4_u10neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_asinf4_u10neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_acosf4_u10neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_acosf4_u10neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_atanf4_u10neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_atanf4_u10neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_atan2f4_u10neon(float32x4_t, float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_atan2f4_u10neon(float32x4_t, float32x4_t);
IMPORT CONST float32x4_t Sleef_logf4_u10neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_logf4_u10neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_cbrtf4_u10neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_cbrtf4_u10neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_expf4_u10neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_expf4_u10neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_powf4_u10neon(float32x4_t, float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_powf4_u10neon(float32x4_t, float32x4_t);
IMPORT CONST float32x4_t Sleef_sinhf4_u10neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_sinhf4_u10neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_coshf4_u10neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_coshf4_u10neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_tanhf4_u10neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_tanhf4_u10neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_sinhf4_u35neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_sinhf4_u35neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_coshf4_u35neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_coshf4_u35neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_tanhf4_u35neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_tanhf4_u35neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_fastsinf4_u3500neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_fastsinf4_u3500neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_fastcosf4_u3500neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_fastcosf4_u3500neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_fastpowf4_u3500neon(float32x4_t, float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_fastpowf4_u3500neon(float32x4_t, float32x4_t);
IMPORT CONST float32x4_t Sleef_asinhf4_u10neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_asinhf4_u10neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_acoshf4_u10neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_acoshf4_u10neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_atanhf4_u10neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_atanhf4_u10neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_exp2f4_u10neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_exp2f4_u10neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_exp2f4_u35neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_exp2f4_u35neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_exp10f4_u10neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_exp10f4_u10neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_exp10f4_u35neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_exp10f4_u35neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_expm1f4_u10neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_expm1f4_u10neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_log10f4_u10neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_log10f4_u10neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_log2f4_u10neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_log2f4_u10neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_log2f4_u35neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_log2f4_u35neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_log1pf4_u10neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_log1pf4_u10neon(float32x4_t);
IMPORT CONST Sleef_float32x4_t_2 Sleef_sincospif4_u05neon(float32x4_t);
IMPORT CONST Sleef_float32x4_t_2 Sleef_cinz_sincospif4_u05neon(float32x4_t);
IMPORT CONST Sleef_float32x4_t_2 Sleef_sincospif4_u35neon(float32x4_t);
IMPORT CONST Sleef_float32x4_t_2 Sleef_cinz_sincospif4_u35neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_sinpif4_u05neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_sinpif4_u05neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_cospif4_u05neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_cospif4_u05neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_fmaf4_neon(float32x4_t, float32x4_t, float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_fmaf4_neon(float32x4_t, float32x4_t, float32x4_t);
IMPORT CONST float32x4_t Sleef_sqrtf4_neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_sqrtf4_neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_sqrtf4_u05neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_sqrtf4_u05neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_sqrtf4_u35neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_sqrtf4_u35neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_hypotf4_u05neon(float32x4_t, float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_hypotf4_u05neon(float32x4_t, float32x4_t);
IMPORT CONST float32x4_t Sleef_hypotf4_u35neon(float32x4_t, float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_hypotf4_u35neon(float32x4_t, float32x4_t);
IMPORT CONST float32x4_t Sleef_fabsf4_neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_fabsf4_neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_copysignf4_neon(float32x4_t, float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_copysignf4_neon(float32x4_t, float32x4_t);
IMPORT CONST float32x4_t Sleef_fmaxf4_neon(float32x4_t, float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_fmaxf4_neon(float32x4_t, float32x4_t);
IMPORT CONST float32x4_t Sleef_fminf4_neon(float32x4_t, float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_fminf4_neon(float32x4_t, float32x4_t);
IMPORT CONST float32x4_t Sleef_fdimf4_neon(float32x4_t, float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_fdimf4_neon(float32x4_t, float32x4_t);
IMPORT CONST float32x4_t Sleef_truncf4_neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_truncf4_neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_floorf4_neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_floorf4_neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_ceilf4_neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_ceilf4_neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_roundf4_neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_roundf4_neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_rintf4_neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_rintf4_neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_nextafterf4_neon(float32x4_t, float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_nextafterf4_neon(float32x4_t, float32x4_t);
IMPORT CONST float32x4_t Sleef_frfrexpf4_neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_frfrexpf4_neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_fmodf4_neon(float32x4_t, float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_fmodf4_neon(float32x4_t, float32x4_t);
IMPORT CONST float32x4_t Sleef_remainderf4_neon(float32x4_t, float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_remainderf4_neon(float32x4_t, float32x4_t);
IMPORT CONST Sleef_float32x4_t_2 Sleef_modff4_neon(float32x4_t);
IMPORT CONST Sleef_float32x4_t_2 Sleef_cinz_modff4_neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_lgammaf4_u10neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_lgammaf4_u10neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_tgammaf4_u10neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_tgammaf4_u10neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_erff4_u10neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_erff4_u10neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_erfcf4_u15neon(float32x4_t);
IMPORT CONST float32x4_t Sleef_cinz_erfcf4_u15neon(float32x4_t);
IMPORT CONST int Sleef_getIntf4_neon(int);
IMPORT CONST int Sleef_cinz_getIntf4_neon(int);
IMPORT CONST void *Sleef_getPtrf4_neon(int);
IMPORT CONST void *Sleef_cinz_getPtrf4_neon(int);
#endif
#ifdef __ARM_NEON__

#ifndef Sleef_float32x4_t_2_DEFINED
typedef struct {
  float32x4_t x, y;
} Sleef_float32x4_t_2;
#define Sleef_float32x4_t_2_DEFINED
#endif

IMPORT CONST float32x4_t Sleef_sinf4_u35neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_finz_sinf4_u35neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_cosf4_u35neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_finz_cosf4_u35neonvfpv4(float32x4_t);
IMPORT CONST Sleef_float32x4_t_2 Sleef_sincosf4_u35neonvfpv4(float32x4_t);
IMPORT CONST Sleef_float32x4_t_2 Sleef_finz_sincosf4_u35neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_tanf4_u35neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_finz_tanf4_u35neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_asinf4_u35neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_finz_asinf4_u35neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_acosf4_u35neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_finz_acosf4_u35neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_atanf4_u35neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_finz_atanf4_u35neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_atan2f4_u35neonvfpv4(float32x4_t, float32x4_t);
IMPORT CONST float32x4_t Sleef_finz_atan2f4_u35neonvfpv4(float32x4_t, float32x4_t);
IMPORT CONST float32x4_t Sleef_logf4_u35neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_finz_logf4_u35neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_cbrtf4_u35neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_finz_cbrtf4_u35neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_sinf4_u10neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_finz_sinf4_u10neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_cosf4_u10neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_finz_cosf4_u10neonvfpv4(float32x4_t);
IMPORT CONST Sleef_float32x4_t_2 Sleef_sincosf4_u10neonvfpv4(float32x4_t);
IMPORT CONST Sleef_float32x4_t_2 Sleef_finz_sincosf4_u10neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_tanf4_u10neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_finz_tanf4_u10neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_asinf4_u10neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_finz_asinf4_u10neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_acosf4_u10neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_finz_acosf4_u10neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_atanf4_u10neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_finz_atanf4_u10neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_atan2f4_u10neonvfpv4(float32x4_t, float32x4_t);
IMPORT CONST float32x4_t Sleef_finz_atan2f4_u10neonvfpv4(float32x4_t, float32x4_t);
IMPORT CONST float32x4_t Sleef_logf4_u10neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_finz_logf4_u10neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_cbrtf4_u10neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_finz_cbrtf4_u10neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_expf4_u10neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_finz_expf4_u10neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_powf4_u10neonvfpv4(float32x4_t, float32x4_t);
IMPORT CONST float32x4_t Sleef_finz_powf4_u10neonvfpv4(float32x4_t, float32x4_t);
IMPORT CONST float32x4_t Sleef_sinhf4_u10neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_finz_sinhf4_u10neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_coshf4_u10neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_finz_coshf4_u10neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_tanhf4_u10neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_finz_tanhf4_u10neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_sinhf4_u35neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_finz_sinhf4_u35neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_coshf4_u35neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_finz_coshf4_u35neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_tanhf4_u35neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_finz_tanhf4_u35neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_fastsinf4_u3500neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_finz_fastsinf4_u3500neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_fastcosf4_u3500neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_finz_fastcosf4_u3500neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_fastpowf4_u3500neonvfpv4(float32x4_t, float32x4_t);
IMPORT CONST float32x4_t Sleef_finz_fastpowf4_u3500neonvfpv4(float32x4_t, float32x4_t);
IMPORT CONST float32x4_t Sleef_asinhf4_u10neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_finz_asinhf4_u10neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_acoshf4_u10neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_finz_acoshf4_u10neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_atanhf4_u10neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_finz_atanhf4_u10neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_exp2f4_u10neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_finz_exp2f4_u10neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_exp2f4_u35neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_finz_exp2f4_u35neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_exp10f4_u10neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_finz_exp10f4_u10neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_exp10f4_u35neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_finz_exp10f4_u35neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_expm1f4_u10neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_finz_expm1f4_u10neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_log10f4_u10neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_finz_log10f4_u10neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_log2f4_u10neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_finz_log2f4_u10neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_log2f4_u35neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_finz_log2f4_u35neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_log1pf4_u10neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_finz_log1pf4_u10neonvfpv4(float32x4_t);
IMPORT CONST Sleef_float32x4_t_2 Sleef_sincospif4_u05neonvfpv4(float32x4_t);
IMPORT CONST Sleef_float32x4_t_2 Sleef_finz_sincospif4_u05neonvfpv4(float32x4_t);
IMPORT CONST Sleef_float32x4_t_2 Sleef_sincospif4_u35neonvfpv4(float32x4_t);
IMPORT CONST Sleef_float32x4_t_2 Sleef_finz_sincospif4_u35neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_sinpif4_u05neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_finz_sinpif4_u05neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_cospif4_u05neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_finz_cospif4_u05neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_fmaf4_neonvfpv4(float32x4_t, float32x4_t, float32x4_t);
IMPORT CONST float32x4_t Sleef_finz_fmaf4_neonvfpv4(float32x4_t, float32x4_t, float32x4_t);
IMPORT CONST float32x4_t Sleef_sqrtf4_neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_finz_sqrtf4_neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_sqrtf4_u05neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_finz_sqrtf4_u05neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_sqrtf4_u35neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_finz_sqrtf4_u35neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_hypotf4_u05neonvfpv4(float32x4_t, float32x4_t);
IMPORT CONST float32x4_t Sleef_finz_hypotf4_u05neonvfpv4(float32x4_t, float32x4_t);
IMPORT CONST float32x4_t Sleef_hypotf4_u35neonvfpv4(float32x4_t, float32x4_t);
IMPORT CONST float32x4_t Sleef_finz_hypotf4_u35neonvfpv4(float32x4_t, float32x4_t);
IMPORT CONST float32x4_t Sleef_fabsf4_neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_finz_fabsf4_neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_copysignf4_neonvfpv4(float32x4_t, float32x4_t);
IMPORT CONST float32x4_t Sleef_finz_copysignf4_neonvfpv4(float32x4_t, float32x4_t);
IMPORT CONST float32x4_t Sleef_fmaxf4_neonvfpv4(float32x4_t, float32x4_t);
IMPORT CONST float32x4_t Sleef_finz_fmaxf4_neonvfpv4(float32x4_t, float32x4_t);
IMPORT CONST float32x4_t Sleef_fminf4_neonvfpv4(float32x4_t, float32x4_t);
IMPORT CONST float32x4_t Sleef_finz_fminf4_neonvfpv4(float32x4_t, float32x4_t);
IMPORT CONST float32x4_t Sleef_fdimf4_neonvfpv4(float32x4_t, float32x4_t);
IMPORT CONST float32x4_t Sleef_finz_fdimf4_neonvfpv4(float32x4_t, float32x4_t);
IMPORT CONST float32x4_t Sleef_truncf4_neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_finz_truncf4_neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_floorf4_neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_finz_floorf4_neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_ceilf4_neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_finz_ceilf4_neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_roundf4_neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_finz_roundf4_neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_rintf4_neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_finz_rintf4_neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_nextafterf4_neonvfpv4(float32x4_t, float32x4_t);
IMPORT CONST float32x4_t Sleef_finz_nextafterf4_neonvfpv4(float32x4_t, float32x4_t);
IMPORT CONST float32x4_t Sleef_frfrexpf4_neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_finz_frfrexpf4_neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_fmodf4_neonvfpv4(float32x4_t, float32x4_t);
IMPORT CONST float32x4_t Sleef_finz_fmodf4_neonvfpv4(float32x4_t, float32x4_t);
IMPORT CONST float32x4_t Sleef_remainderf4_neonvfpv4(float32x4_t, float32x4_t);
IMPORT CONST float32x4_t Sleef_finz_remainderf4_neonvfpv4(float32x4_t, float32x4_t);
IMPORT CONST Sleef_float32x4_t_2 Sleef_modff4_neonvfpv4(float32x4_t);
IMPORT CONST Sleef_float32x4_t_2 Sleef_finz_modff4_neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_lgammaf4_u10neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_finz_lgammaf4_u10neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_tgammaf4_u10neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_finz_tgammaf4_u10neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_erff4_u10neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_finz_erff4_u10neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_erfcf4_u15neonvfpv4(float32x4_t);
IMPORT CONST float32x4_t Sleef_finz_erfcf4_u15neonvfpv4(float32x4_t);
IMPORT CONST int Sleef_getIntf4_neonvfpv4(int);
IMPORT CONST int Sleef_finz_getIntf4_neonvfpv4(int);
IMPORT CONST void *Sleef_getPtrf4_neonvfpv4(int);
IMPORT CONST void *Sleef_finz_getPtrf4_neonvfpv4(int);
#endif
#ifdef __STDC__

#ifndef Sleef_double_2_DEFINED
typedef struct {
  double x, y;
} Sleef_double_2;
#define Sleef_double_2_DEFINED
#endif

IMPORT CONST double Sleef_sind1_u35purec(double);
IMPORT CONST double Sleef_cinz_sind1_u35purec(double);
IMPORT CONST double Sleef_cosd1_u35purec(double);
IMPORT CONST double Sleef_cinz_cosd1_u35purec(double);
IMPORT CONST Sleef_double_2 Sleef_sincosd1_u35purec(double);
IMPORT CONST Sleef_double_2 Sleef_cinz_sincosd1_u35purec(double);
IMPORT CONST double Sleef_tand1_u35purec(double);
IMPORT CONST double Sleef_cinz_tand1_u35purec(double);
IMPORT CONST double Sleef_asind1_u35purec(double);
IMPORT CONST double Sleef_cinz_asind1_u35purec(double);
IMPORT CONST double Sleef_acosd1_u35purec(double);
IMPORT CONST double Sleef_cinz_acosd1_u35purec(double);
IMPORT CONST double Sleef_atand1_u35purec(double);
IMPORT CONST double Sleef_cinz_atand1_u35purec(double);
IMPORT CONST double Sleef_atan2d1_u35purec(double, double);
IMPORT CONST double Sleef_cinz_atan2d1_u35purec(double, double);
IMPORT CONST double Sleef_logd1_u35purec(double);
IMPORT CONST double Sleef_cinz_logd1_u35purec(double);
IMPORT CONST double Sleef_cbrtd1_u35purec(double);
IMPORT CONST double Sleef_cinz_cbrtd1_u35purec(double);
IMPORT CONST double Sleef_sind1_u10purec(double);
IMPORT CONST double Sleef_cinz_sind1_u10purec(double);
IMPORT CONST double Sleef_cosd1_u10purec(double);
IMPORT CONST double Sleef_cinz_cosd1_u10purec(double);
IMPORT CONST Sleef_double_2 Sleef_sincosd1_u10purec(double);
IMPORT CONST Sleef_double_2 Sleef_cinz_sincosd1_u10purec(double);
IMPORT CONST double Sleef_tand1_u10purec(double);
IMPORT CONST double Sleef_cinz_tand1_u10purec(double);
IMPORT CONST double Sleef_asind1_u10purec(double);
IMPORT CONST double Sleef_cinz_asind1_u10purec(double);
IMPORT CONST double Sleef_acosd1_u10purec(double);
IMPORT CONST double Sleef_cinz_acosd1_u10purec(double);
IMPORT CONST double Sleef_atand1_u10purec(double);
IMPORT CONST double Sleef_cinz_atand1_u10purec(double);
IMPORT CONST double Sleef_atan2d1_u10purec(double, double);
IMPORT CONST double Sleef_cinz_atan2d1_u10purec(double, double);
IMPORT CONST double Sleef_logd1_u10purec(double);
IMPORT CONST double Sleef_cinz_logd1_u10purec(double);
IMPORT CONST double Sleef_cbrtd1_u10purec(double);
IMPORT CONST double Sleef_cinz_cbrtd1_u10purec(double);
IMPORT CONST double Sleef_expd1_u10purec(double);
IMPORT CONST double Sleef_cinz_expd1_u10purec(double);
IMPORT CONST double Sleef_powd1_u10purec(double, double);
IMPORT CONST double Sleef_cinz_powd1_u10purec(double, double);
IMPORT CONST double Sleef_sinhd1_u10purec(double);
IMPORT CONST double Sleef_cinz_sinhd1_u10purec(double);
IMPORT CONST double Sleef_coshd1_u10purec(double);
IMPORT CONST double Sleef_cinz_coshd1_u10purec(double);
IMPORT CONST double Sleef_tanhd1_u10purec(double);
IMPORT CONST double Sleef_cinz_tanhd1_u10purec(double);
IMPORT CONST double Sleef_sinhd1_u35purec(double);
IMPORT CONST double Sleef_cinz_sinhd1_u35purec(double);
IMPORT CONST double Sleef_coshd1_u35purec(double);
IMPORT CONST double Sleef_cinz_coshd1_u35purec(double);
IMPORT CONST double Sleef_tanhd1_u35purec(double);
IMPORT CONST double Sleef_cinz_tanhd1_u35purec(double);
IMPORT CONST double Sleef_fastsind1_u3500purec(double);
IMPORT CONST double Sleef_cinz_fastsind1_u3500purec(double);
IMPORT CONST double Sleef_fastcosd1_u3500purec(double);
IMPORT CONST double Sleef_cinz_fastcosd1_u3500purec(double);
IMPORT CONST double Sleef_fastpowd1_u3500purec(double, double);
IMPORT CONST double Sleef_cinz_fastpowd1_u3500purec(double, double);
IMPORT CONST double Sleef_asinhd1_u10purec(double);
IMPORT CONST double Sleef_cinz_asinhd1_u10purec(double);
IMPORT CONST double Sleef_acoshd1_u10purec(double);
IMPORT CONST double Sleef_cinz_acoshd1_u10purec(double);
IMPORT CONST double Sleef_atanhd1_u10purec(double);
IMPORT CONST double Sleef_cinz_atanhd1_u10purec(double);
IMPORT CONST double Sleef_exp2d1_u10purec(double);
IMPORT CONST double Sleef_cinz_exp2d1_u10purec(double);
IMPORT CONST double Sleef_exp2d1_u35purec(double);
IMPORT CONST double Sleef_cinz_exp2d1_u35purec(double);
IMPORT CONST double Sleef_exp10d1_u10purec(double);
IMPORT CONST double Sleef_cinz_exp10d1_u10purec(double);
IMPORT CONST double Sleef_exp10d1_u35purec(double);
IMPORT CONST double Sleef_cinz_exp10d1_u35purec(double);
IMPORT CONST double Sleef_expm1d1_u10purec(double);
IMPORT CONST double Sleef_cinz_expm1d1_u10purec(double);
IMPORT CONST double Sleef_log10d1_u10purec(double);
IMPORT CONST double Sleef_cinz_log10d1_u10purec(double);
IMPORT CONST double Sleef_log2d1_u10purec(double);
IMPORT CONST double Sleef_cinz_log2d1_u10purec(double);
IMPORT CONST double Sleef_log2d1_u35purec(double);
IMPORT CONST double Sleef_cinz_log2d1_u35purec(double);
IMPORT CONST double Sleef_log1pd1_u10purec(double);
IMPORT CONST double Sleef_cinz_log1pd1_u10purec(double);
IMPORT CONST Sleef_double_2 Sleef_sincospid1_u05purec(double);
IMPORT CONST Sleef_double_2 Sleef_cinz_sincospid1_u05purec(double);
IMPORT CONST Sleef_double_2 Sleef_sincospid1_u35purec(double);
IMPORT CONST Sleef_double_2 Sleef_cinz_sincospid1_u35purec(double);
IMPORT CONST double Sleef_sinpid1_u05purec(double);
IMPORT CONST double Sleef_cinz_sinpid1_u05purec(double);
IMPORT CONST double Sleef_cospid1_u05purec(double);
IMPORT CONST double Sleef_cinz_cospid1_u05purec(double);
IMPORT CONST double Sleef_ldexpd1_purec(double, int32_t);
IMPORT CONST double Sleef_cinz_ldexpd1_purec(double, int32_t);
IMPORT CONST int32_t Sleef_ilogbd1_purec(double);
IMPORT CONST int32_t Sleef_cinz_ilogbd1_purec(double);
IMPORT CONST double Sleef_fmad1_purec(double, double, double);
IMPORT CONST double Sleef_cinz_fmad1_purec(double, double, double);
IMPORT CONST double Sleef_sqrtd1_purec(double);
IMPORT CONST double Sleef_cinz_sqrtd1_purec(double);
IMPORT CONST double Sleef_sqrtd1_u05purec(double);
IMPORT CONST double Sleef_cinz_sqrtd1_u05purec(double);
IMPORT CONST double Sleef_sqrtd1_u35purec(double);
IMPORT CONST double Sleef_cinz_sqrtd1_u35purec(double);
IMPORT CONST double Sleef_hypotd1_u05purec(double, double);
IMPORT CONST double Sleef_cinz_hypotd1_u05purec(double, double);
IMPORT CONST double Sleef_hypotd1_u35purec(double, double);
IMPORT CONST double Sleef_cinz_hypotd1_u35purec(double, double);
IMPORT CONST double Sleef_fabsd1_purec(double);
IMPORT CONST double Sleef_cinz_fabsd1_purec(double);
IMPORT CONST double Sleef_copysignd1_purec(double, double);
IMPORT CONST double Sleef_cinz_copysignd1_purec(double, double);
IMPORT CONST double Sleef_fmaxd1_purec(double, double);
IMPORT CONST double Sleef_cinz_fmaxd1_purec(double, double);
IMPORT CONST double Sleef_fmind1_purec(double, double);
IMPORT CONST double Sleef_cinz_fmind1_purec(double, double);
IMPORT CONST double Sleef_fdimd1_purec(double, double);
IMPORT CONST double Sleef_cinz_fdimd1_purec(double, double);
IMPORT CONST double Sleef_truncd1_purec(double);
IMPORT CONST double Sleef_cinz_truncd1_purec(double);
IMPORT CONST double Sleef_floord1_purec(double);
IMPORT CONST double Sleef_cinz_floord1_purec(double);
IMPORT CONST double Sleef_ceild1_purec(double);
IMPORT CONST double Sleef_cinz_ceild1_purec(double);
IMPORT CONST double Sleef_roundd1_purec(double);
IMPORT CONST double Sleef_cinz_roundd1_purec(double);
IMPORT CONST double Sleef_rintd1_purec(double);
IMPORT CONST double Sleef_cinz_rintd1_purec(double);
IMPORT CONST double Sleef_nextafterd1_purec(double, double);
IMPORT CONST double Sleef_cinz_nextafterd1_purec(double, double);
IMPORT CONST double Sleef_frfrexpd1_purec(double);
IMPORT CONST double Sleef_cinz_frfrexpd1_purec(double);
IMPORT CONST int32_t Sleef_expfrexpd1_purec(double);
IMPORT CONST int32_t Sleef_cinz_expfrexpd1_purec(double);
IMPORT CONST double Sleef_fmodd1_purec(double, double);
IMPORT CONST double Sleef_cinz_fmodd1_purec(double, double);
IMPORT CONST double Sleef_remainderd1_purec(double, double);
IMPORT CONST double Sleef_cinz_remainderd1_purec(double, double);
IMPORT CONST Sleef_double_2 Sleef_modfd1_purec(double);
IMPORT CONST Sleef_double_2 Sleef_cinz_modfd1_purec(double);
IMPORT CONST double Sleef_lgammad1_u10purec(double);
IMPORT CONST double Sleef_cinz_lgammad1_u10purec(double);
IMPORT CONST double Sleef_tgammad1_u10purec(double);
IMPORT CONST double Sleef_cinz_tgammad1_u10purec(double);
IMPORT CONST double Sleef_erfd1_u10purec(double);
IMPORT CONST double Sleef_cinz_erfd1_u10purec(double);
IMPORT CONST double Sleef_erfcd1_u15purec(double);
IMPORT CONST double Sleef_cinz_erfcd1_u15purec(double);
IMPORT CONST int Sleef_getIntd1_purec(int);
IMPORT CONST void *Sleef_getPtrd1_purec(int);

#ifndef Sleef_float_2_DEFINED
typedef struct {
  float x, y;
} Sleef_float_2;
#define Sleef_float_2_DEFINED
#endif

IMPORT CONST float Sleef_sinf1_u35purec(float);
IMPORT CONST float Sleef_cinz_sinf1_u35purec(float);
IMPORT CONST float Sleef_cosf1_u35purec(float);
IMPORT CONST float Sleef_cinz_cosf1_u35purec(float);
IMPORT CONST Sleef_float_2 Sleef_sincosf1_u35purec(float);
IMPORT CONST Sleef_float_2 Sleef_cinz_sincosf1_u35purec(float);
IMPORT CONST float Sleef_tanf1_u35purec(float);
IMPORT CONST float Sleef_cinz_tanf1_u35purec(float);
IMPORT CONST float Sleef_asinf1_u35purec(float);
IMPORT CONST float Sleef_cinz_asinf1_u35purec(float);
IMPORT CONST float Sleef_acosf1_u35purec(float);
IMPORT CONST float Sleef_cinz_acosf1_u35purec(float);
IMPORT CONST float Sleef_atanf1_u35purec(float);
IMPORT CONST float Sleef_cinz_atanf1_u35purec(float);
IMPORT CONST float Sleef_atan2f1_u35purec(float, float);
IMPORT CONST float Sleef_cinz_atan2f1_u35purec(float, float);
IMPORT CONST float Sleef_logf1_u35purec(float);
IMPORT CONST float Sleef_cinz_logf1_u35purec(float);
IMPORT CONST float Sleef_cbrtf1_u35purec(float);
IMPORT CONST float Sleef_cinz_cbrtf1_u35purec(float);
IMPORT CONST float Sleef_sinf1_u10purec(float);
IMPORT CONST float Sleef_cinz_sinf1_u10purec(float);
IMPORT CONST float Sleef_cosf1_u10purec(float);
IMPORT CONST float Sleef_cinz_cosf1_u10purec(float);
IMPORT CONST Sleef_float_2 Sleef_sincosf1_u10purec(float);
IMPORT CONST Sleef_float_2 Sleef_cinz_sincosf1_u10purec(float);
IMPORT CONST float Sleef_tanf1_u10purec(float);
IMPORT CONST float Sleef_cinz_tanf1_u10purec(float);
IMPORT CONST float Sleef_asinf1_u10purec(float);
IMPORT CONST float Sleef_cinz_asinf1_u10purec(float);
IMPORT CONST float Sleef_acosf1_u10purec(float);
IMPORT CONST float Sleef_cinz_acosf1_u10purec(float);
IMPORT CONST float Sleef_atanf1_u10purec(float);
IMPORT CONST float Sleef_cinz_atanf1_u10purec(float);
IMPORT CONST float Sleef_atan2f1_u10purec(float, float);
IMPORT CONST float Sleef_cinz_atan2f1_u10purec(float, float);
IMPORT CONST float Sleef_logf1_u10purec(float);
IMPORT CONST float Sleef_cinz_logf1_u10purec(float);
IMPORT CONST float Sleef_cbrtf1_u10purec(float);
IMPORT CONST float Sleef_cinz_cbrtf1_u10purec(float);
IMPORT CONST float Sleef_expf1_u10purec(float);
IMPORT CONST float Sleef_cinz_expf1_u10purec(float);
IMPORT CONST float Sleef_powf1_u10purec(float, float);
IMPORT CONST float Sleef_cinz_powf1_u10purec(float, float);
IMPORT CONST float Sleef_sinhf1_u10purec(float);
IMPORT CONST float Sleef_cinz_sinhf1_u10purec(float);
IMPORT CONST float Sleef_coshf1_u10purec(float);
IMPORT CONST float Sleef_cinz_coshf1_u10purec(float);
IMPORT CONST float Sleef_tanhf1_u10purec(float);
IMPORT CONST float Sleef_cinz_tanhf1_u10purec(float);
IMPORT CONST float Sleef_sinhf1_u35purec(float);
IMPORT CONST float Sleef_cinz_sinhf1_u35purec(float);
IMPORT CONST float Sleef_coshf1_u35purec(float);
IMPORT CONST float Sleef_cinz_coshf1_u35purec(float);
IMPORT CONST float Sleef_tanhf1_u35purec(float);
IMPORT CONST float Sleef_cinz_tanhf1_u35purec(float);
IMPORT CONST float Sleef_fastsinf1_u3500purec(float);
IMPORT CONST float Sleef_cinz_fastsinf1_u3500purec(float);
IMPORT CONST float Sleef_fastcosf1_u3500purec(float);
IMPORT CONST float Sleef_cinz_fastcosf1_u3500purec(float);
IMPORT CONST float Sleef_fastpowf1_u3500purec(float, float);
IMPORT CONST float Sleef_cinz_fastpowf1_u3500purec(float, float);
IMPORT CONST float Sleef_asinhf1_u10purec(float);
IMPORT CONST float Sleef_cinz_asinhf1_u10purec(float);
IMPORT CONST float Sleef_acoshf1_u10purec(float);
IMPORT CONST float Sleef_cinz_acoshf1_u10purec(float);
IMPORT CONST float Sleef_atanhf1_u10purec(float);
IMPORT CONST float Sleef_cinz_atanhf1_u10purec(float);
IMPORT CONST float Sleef_exp2f1_u10purec(float);
IMPORT CONST float Sleef_cinz_exp2f1_u10purec(float);
IMPORT CONST float Sleef_exp2f1_u35purec(float);
IMPORT CONST float Sleef_cinz_exp2f1_u35purec(float);
IMPORT CONST float Sleef_exp10f1_u10purec(float);
IMPORT CONST float Sleef_cinz_exp10f1_u10purec(float);
IMPORT CONST float Sleef_exp10f1_u35purec(float);
IMPORT CONST float Sleef_cinz_exp10f1_u35purec(float);
IMPORT CONST float Sleef_expm1f1_u10purec(float);
IMPORT CONST float Sleef_cinz_expm1f1_u10purec(float);
IMPORT CONST float Sleef_log10f1_u10purec(float);
IMPORT CONST float Sleef_cinz_log10f1_u10purec(float);
IMPORT CONST float Sleef_log2f1_u10purec(float);
IMPORT CONST float Sleef_cinz_log2f1_u10purec(float);
IMPORT CONST float Sleef_log2f1_u35purec(float);
IMPORT CONST float Sleef_cinz_log2f1_u35purec(float);
IMPORT CONST float Sleef_log1pf1_u10purec(float);
IMPORT CONST float Sleef_cinz_log1pf1_u10purec(float);
IMPORT CONST Sleef_float_2 Sleef_sincospif1_u05purec(float);
IMPORT CONST Sleef_float_2 Sleef_cinz_sincospif1_u05purec(float);
IMPORT CONST Sleef_float_2 Sleef_sincospif1_u35purec(float);
IMPORT CONST Sleef_float_2 Sleef_cinz_sincospif1_u35purec(float);
IMPORT CONST float Sleef_sinpif1_u05purec(float);
IMPORT CONST float Sleef_cinz_sinpif1_u05purec(float);
IMPORT CONST float Sleef_cospif1_u05purec(float);
IMPORT CONST float Sleef_cinz_cospif1_u05purec(float);
IMPORT CONST float Sleef_fmaf1_purec(float, float, float);
IMPORT CONST float Sleef_cinz_fmaf1_purec(float, float, float);
IMPORT CONST float Sleef_sqrtf1_purec(float);
IMPORT CONST float Sleef_cinz_sqrtf1_purec(float);
IMPORT CONST float Sleef_sqrtf1_u05purec(float);
IMPORT CONST float Sleef_cinz_sqrtf1_u05purec(float);
IMPORT CONST float Sleef_sqrtf1_u35purec(float);
IMPORT CONST float Sleef_cinz_sqrtf1_u35purec(float);
IMPORT CONST float Sleef_hypotf1_u05purec(float, float);
IMPORT CONST float Sleef_cinz_hypotf1_u05purec(float, float);
IMPORT CONST float Sleef_hypotf1_u35purec(float, float);
IMPORT CONST float Sleef_cinz_hypotf1_u35purec(float, float);
IMPORT CONST float Sleef_fabsf1_purec(float);
IMPORT CONST float Sleef_cinz_fabsf1_purec(float);
IMPORT CONST float Sleef_copysignf1_purec(float, float);
IMPORT CONST float Sleef_cinz_copysignf1_purec(float, float);
IMPORT CONST float Sleef_fmaxf1_purec(float, float);
IMPORT CONST float Sleef_cinz_fmaxf1_purec(float, float);
IMPORT CONST float Sleef_fminf1_purec(float, float);
IMPORT CONST float Sleef_cinz_fminf1_purec(float, float);
IMPORT CONST float Sleef_fdimf1_purec(float, float);
IMPORT CONST float Sleef_cinz_fdimf1_purec(float, float);
IMPORT CONST float Sleef_truncf1_purec(float);
IMPORT CONST float Sleef_cinz_truncf1_purec(float);
IMPORT CONST float Sleef_floorf1_purec(float);
IMPORT CONST float Sleef_cinz_floorf1_purec(float);
IMPORT CONST float Sleef_ceilf1_purec(float);
IMPORT CONST float Sleef_cinz_ceilf1_purec(float);
IMPORT CONST float Sleef_roundf1_purec(float);
IMPORT CONST float Sleef_cinz_roundf1_purec(float);
IMPORT CONST float Sleef_rintf1_purec(float);
IMPORT CONST float Sleef_cinz_rintf1_purec(float);
IMPORT CONST float Sleef_nextafterf1_purec(float, float);
IMPORT CONST float Sleef_cinz_nextafterf1_purec(float, float);
IMPORT CONST float Sleef_frfrexpf1_purec(float);
IMPORT CONST float Sleef_cinz_frfrexpf1_purec(float);
IMPORT CONST float Sleef_fmodf1_purec(float, float);
IMPORT CONST float Sleef_cinz_fmodf1_purec(float, float);
IMPORT CONST float Sleef_remainderf1_purec(float, float);
IMPORT CONST float Sleef_cinz_remainderf1_purec(float, float);
IMPORT CONST Sleef_float_2 Sleef_modff1_purec(float);
IMPORT CONST Sleef_float_2 Sleef_cinz_modff1_purec(float);
IMPORT CONST float Sleef_lgammaf1_u10purec(float);
IMPORT CONST float Sleef_cinz_lgammaf1_u10purec(float);
IMPORT CONST float Sleef_tgammaf1_u10purec(float);
IMPORT CONST float Sleef_cinz_tgammaf1_u10purec(float);
IMPORT CONST float Sleef_erff1_u10purec(float);
IMPORT CONST float Sleef_cinz_erff1_u10purec(float);
IMPORT CONST float Sleef_erfcf1_u15purec(float);
IMPORT CONST float Sleef_cinz_erfcf1_u15purec(float);
IMPORT CONST int Sleef_getIntf1_purec(int);
IMPORT CONST int Sleef_cinz_getIntf1_purec(int);
IMPORT CONST void *Sleef_getPtrf1_purec(int);
IMPORT CONST void *Sleef_cinz_getPtrf1_purec(int);
#endif
#ifdef FP_FAST_FMA

#ifndef Sleef_double_2_DEFINED
typedef struct {
  double x, y;
} Sleef_double_2;
#define Sleef_double_2_DEFINED
#endif

IMPORT CONST double Sleef_sind1_u35purecfma(double);
IMPORT CONST double Sleef_finz_sind1_u35purecfma(double);
IMPORT CONST double Sleef_cosd1_u35purecfma(double);
IMPORT CONST double Sleef_finz_cosd1_u35purecfma(double);
IMPORT CONST Sleef_double_2 Sleef_sincosd1_u35purecfma(double);
IMPORT CONST Sleef_double_2 Sleef_finz_sincosd1_u35purecfma(double);
IMPORT CONST double Sleef_tand1_u35purecfma(double);
IMPORT CONST double Sleef_finz_tand1_u35purecfma(double);
IMPORT CONST double Sleef_asind1_u35purecfma(double);
IMPORT CONST double Sleef_finz_asind1_u35purecfma(double);
IMPORT CONST double Sleef_acosd1_u35purecfma(double);
IMPORT CONST double Sleef_finz_acosd1_u35purecfma(double);
IMPORT CONST double Sleef_atand1_u35purecfma(double);
IMPORT CONST double Sleef_finz_atand1_u35purecfma(double);
IMPORT CONST double Sleef_atan2d1_u35purecfma(double, double);
IMPORT CONST double Sleef_finz_atan2d1_u35purecfma(double, double);
IMPORT CONST double Sleef_logd1_u35purecfma(double);
IMPORT CONST double Sleef_finz_logd1_u35purecfma(double);
IMPORT CONST double Sleef_cbrtd1_u35purecfma(double);
IMPORT CONST double Sleef_finz_cbrtd1_u35purecfma(double);
IMPORT CONST double Sleef_sind1_u10purecfma(double);
IMPORT CONST double Sleef_finz_sind1_u10purecfma(double);
IMPORT CONST double Sleef_cosd1_u10purecfma(double);
IMPORT CONST double Sleef_finz_cosd1_u10purecfma(double);
IMPORT CONST Sleef_double_2 Sleef_sincosd1_u10purecfma(double);
IMPORT CONST Sleef_double_2 Sleef_finz_sincosd1_u10purecfma(double);
IMPORT CONST double Sleef_tand1_u10purecfma(double);
IMPORT CONST double Sleef_finz_tand1_u10purecfma(double);
IMPORT CONST double Sleef_asind1_u10purecfma(double);
IMPORT CONST double Sleef_finz_asind1_u10purecfma(double);
IMPORT CONST double Sleef_acosd1_u10purecfma(double);
IMPORT CONST double Sleef_finz_acosd1_u10purecfma(double);
IMPORT CONST double Sleef_atand1_u10purecfma(double);
IMPORT CONST double Sleef_finz_atand1_u10purecfma(double);
IMPORT CONST double Sleef_atan2d1_u10purecfma(double, double);
IMPORT CONST double Sleef_finz_atan2d1_u10purecfma(double, double);
IMPORT CONST double Sleef_logd1_u10purecfma(double);
IMPORT CONST double Sleef_finz_logd1_u10purecfma(double);
IMPORT CONST double Sleef_cbrtd1_u10purecfma(double);
IMPORT CONST double Sleef_finz_cbrtd1_u10purecfma(double);
IMPORT CONST double Sleef_expd1_u10purecfma(double);
IMPORT CONST double Sleef_finz_expd1_u10purecfma(double);
IMPORT CONST double Sleef_powd1_u10purecfma(double, double);
IMPORT CONST double Sleef_finz_powd1_u10purecfma(double, double);
IMPORT CONST double Sleef_sinhd1_u10purecfma(double);
IMPORT CONST double Sleef_finz_sinhd1_u10purecfma(double);
IMPORT CONST double Sleef_coshd1_u10purecfma(double);
IMPORT CONST double Sleef_finz_coshd1_u10purecfma(double);
IMPORT CONST double Sleef_tanhd1_u10purecfma(double);
IMPORT CONST double Sleef_finz_tanhd1_u10purecfma(double);
IMPORT CONST double Sleef_sinhd1_u35purecfma(double);
IMPORT CONST double Sleef_finz_sinhd1_u35purecfma(double);
IMPORT CONST double Sleef_coshd1_u35purecfma(double);
IMPORT CONST double Sleef_finz_coshd1_u35purecfma(double);
IMPORT CONST double Sleef_tanhd1_u35purecfma(double);
IMPORT CONST double Sleef_finz_tanhd1_u35purecfma(double);
IMPORT CONST double Sleef_fastsind1_u3500purecfma(double);
IMPORT CONST double Sleef_finz_fastsind1_u3500purecfma(double);
IMPORT CONST double Sleef_fastcosd1_u3500purecfma(double);
IMPORT CONST double Sleef_finz_fastcosd1_u3500purecfma(double);
IMPORT CONST double Sleef_fastpowd1_u3500purecfma(double, double);
IMPORT CONST double Sleef_finz_fastpowd1_u3500purecfma(double, double);
IMPORT CONST double Sleef_asinhd1_u10purecfma(double);
IMPORT CONST double Sleef_finz_asinhd1_u10purecfma(double);
IMPORT CONST double Sleef_acoshd1_u10purecfma(double);
IMPORT CONST double Sleef_finz_acoshd1_u10purecfma(double);
IMPORT CONST double Sleef_atanhd1_u10purecfma(double);
IMPORT CONST double Sleef_finz_atanhd1_u10purecfma(double);
IMPORT CONST double Sleef_exp2d1_u10purecfma(double);
IMPORT CONST double Sleef_finz_exp2d1_u10purecfma(double);
IMPORT CONST double Sleef_exp2d1_u35purecfma(double);
IMPORT CONST double Sleef_finz_exp2d1_u35purecfma(double);
IMPORT CONST double Sleef_exp10d1_u10purecfma(double);
IMPORT CONST double Sleef_finz_exp10d1_u10purecfma(double);
IMPORT CONST double Sleef_exp10d1_u35purecfma(double);
IMPORT CONST double Sleef_finz_exp10d1_u35purecfma(double);
IMPORT CONST double Sleef_expm1d1_u10purecfma(double);
IMPORT CONST double Sleef_finz_expm1d1_u10purecfma(double);
IMPORT CONST double Sleef_log10d1_u10purecfma(double);
IMPORT CONST double Sleef_finz_log10d1_u10purecfma(double);
IMPORT CONST double Sleef_log2d1_u10purecfma(double);
IMPORT CONST double Sleef_finz_log2d1_u10purecfma(double);
IMPORT CONST double Sleef_log2d1_u35purecfma(double);
IMPORT CONST double Sleef_finz_log2d1_u35purecfma(double);
IMPORT CONST double Sleef_log1pd1_u10purecfma(double);
IMPORT CONST double Sleef_finz_log1pd1_u10purecfma(double);
IMPORT CONST Sleef_double_2 Sleef_sincospid1_u05purecfma(double);
IMPORT CONST Sleef_double_2 Sleef_finz_sincospid1_u05purecfma(double);
IMPORT CONST Sleef_double_2 Sleef_sincospid1_u35purecfma(double);
IMPORT CONST Sleef_double_2 Sleef_finz_sincospid1_u35purecfma(double);
IMPORT CONST double Sleef_sinpid1_u05purecfma(double);
IMPORT CONST double Sleef_finz_sinpid1_u05purecfma(double);
IMPORT CONST double Sleef_cospid1_u05purecfma(double);
IMPORT CONST double Sleef_finz_cospid1_u05purecfma(double);
IMPORT CONST double Sleef_ldexpd1_purecfma(double, int32_t);
IMPORT CONST double Sleef_finz_ldexpd1_purecfma(double, int32_t);
IMPORT CONST int32_t Sleef_ilogbd1_purecfma(double);
IMPORT CONST int32_t Sleef_finz_ilogbd1_purecfma(double);
IMPORT CONST double Sleef_fmad1_purecfma(double, double, double);
IMPORT CONST double Sleef_finz_fmad1_purecfma(double, double, double);
IMPORT CONST double Sleef_sqrtd1_purecfma(double);
IMPORT CONST double Sleef_finz_sqrtd1_purecfma(double);
IMPORT CONST double Sleef_sqrtd1_u05purecfma(double);
IMPORT CONST double Sleef_finz_sqrtd1_u05purecfma(double);
IMPORT CONST double Sleef_sqrtd1_u35purecfma(double);
IMPORT CONST double Sleef_finz_sqrtd1_u35purecfma(double);
IMPORT CONST double Sleef_hypotd1_u05purecfma(double, double);
IMPORT CONST double Sleef_finz_hypotd1_u05purecfma(double, double);
IMPORT CONST double Sleef_hypotd1_u35purecfma(double, double);
IMPORT CONST double Sleef_finz_hypotd1_u35purecfma(double, double);
IMPORT CONST double Sleef_fabsd1_purecfma(double);
IMPORT CONST double Sleef_finz_fabsd1_purecfma(double);
IMPORT CONST double Sleef_copysignd1_purecfma(double, double);
IMPORT CONST double Sleef_finz_copysignd1_purecfma(double, double);
IMPORT CONST double Sleef_fmaxd1_purecfma(double, double);
IMPORT CONST double Sleef_finz_fmaxd1_purecfma(double, double);
IMPORT CONST double Sleef_fmind1_purecfma(double, double);
IMPORT CONST double Sleef_finz_fmind1_purecfma(double, double);
IMPORT CONST double Sleef_fdimd1_purecfma(double, double);
IMPORT CONST double Sleef_finz_fdimd1_purecfma(double, double);
IMPORT CONST double Sleef_truncd1_purecfma(double);
IMPORT CONST double Sleef_finz_truncd1_purecfma(double);
IMPORT CONST double Sleef_floord1_purecfma(double);
IMPORT CONST double Sleef_finz_floord1_purecfma(double);
IMPORT CONST double Sleef_ceild1_purecfma(double);
IMPORT CONST double Sleef_finz_ceild1_purecfma(double);
IMPORT CONST double Sleef_roundd1_purecfma(double);
IMPORT CONST double Sleef_finz_roundd1_purecfma(double);
IMPORT CONST double Sleef_rintd1_purecfma(double);
IMPORT CONST double Sleef_finz_rintd1_purecfma(double);
IMPORT CONST double Sleef_nextafterd1_purecfma(double, double);
IMPORT CONST double Sleef_finz_nextafterd1_purecfma(double, double);
IMPORT CONST double Sleef_frfrexpd1_purecfma(double);
IMPORT CONST double Sleef_finz_frfrexpd1_purecfma(double);
IMPORT CONST int32_t Sleef_expfrexpd1_purecfma(double);
IMPORT CONST int32_t Sleef_finz_expfrexpd1_purecfma(double);
IMPORT CONST double Sleef_fmodd1_purecfma(double, double);
IMPORT CONST double Sleef_finz_fmodd1_purecfma(double, double);
IMPORT CONST double Sleef_remainderd1_purecfma(double, double);
IMPORT CONST double Sleef_finz_remainderd1_purecfma(double, double);
IMPORT CONST Sleef_double_2 Sleef_modfd1_purecfma(double);
IMPORT CONST Sleef_double_2 Sleef_finz_modfd1_purecfma(double);
IMPORT CONST double Sleef_lgammad1_u10purecfma(double);
IMPORT CONST double Sleef_finz_lgammad1_u10purecfma(double);
IMPORT CONST double Sleef_tgammad1_u10purecfma(double);
IMPORT CONST double Sleef_finz_tgammad1_u10purecfma(double);
IMPORT CONST double Sleef_erfd1_u10purecfma(double);
IMPORT CONST double Sleef_finz_erfd1_u10purecfma(double);
IMPORT CONST double Sleef_erfcd1_u15purecfma(double);
IMPORT CONST double Sleef_finz_erfcd1_u15purecfma(double);
IMPORT CONST int Sleef_getIntd1_purecfma(int);
IMPORT CONST void *Sleef_getPtrd1_purecfma(int);

#ifndef Sleef_float_2_DEFINED
typedef struct {
  float x, y;
} Sleef_float_2;
#define Sleef_float_2_DEFINED
#endif

IMPORT CONST float Sleef_sinf1_u35purecfma(float);
IMPORT CONST float Sleef_finz_sinf1_u35purecfma(float);
IMPORT CONST float Sleef_cosf1_u35purecfma(float);
IMPORT CONST float Sleef_finz_cosf1_u35purecfma(float);
IMPORT CONST Sleef_float_2 Sleef_sincosf1_u35purecfma(float);
IMPORT CONST Sleef_float_2 Sleef_finz_sincosf1_u35purecfma(float);
IMPORT CONST float Sleef_tanf1_u35purecfma(float);
IMPORT CONST float Sleef_finz_tanf1_u35purecfma(float);
IMPORT CONST float Sleef_asinf1_u35purecfma(float);
IMPORT CONST float Sleef_finz_asinf1_u35purecfma(float);
IMPORT CONST float Sleef_acosf1_u35purecfma(float);
IMPORT CONST float Sleef_finz_acosf1_u35purecfma(float);
IMPORT CONST float Sleef_atanf1_u35purecfma(float);
IMPORT CONST float Sleef_finz_atanf1_u35purecfma(float);
IMPORT CONST float Sleef_atan2f1_u35purecfma(float, float);
IMPORT CONST float Sleef_finz_atan2f1_u35purecfma(float, float);
IMPORT CONST float Sleef_logf1_u35purecfma(float);
IMPORT CONST float Sleef_finz_logf1_u35purecfma(float);
IMPORT CONST float Sleef_cbrtf1_u35purecfma(float);
IMPORT CONST float Sleef_finz_cbrtf1_u35purecfma(float);
IMPORT CONST float Sleef_sinf1_u10purecfma(float);
IMPORT CONST float Sleef_finz_sinf1_u10purecfma(float);
IMPORT CONST float Sleef_cosf1_u10purecfma(float);
IMPORT CONST float Sleef_finz_cosf1_u10purecfma(float);
IMPORT CONST Sleef_float_2 Sleef_sincosf1_u10purecfma(float);
IMPORT CONST Sleef_float_2 Sleef_finz_sincosf1_u10purecfma(float);
IMPORT CONST float Sleef_tanf1_u10purecfma(float);
IMPORT CONST float Sleef_finz_tanf1_u10purecfma(float);
IMPORT CONST float Sleef_asinf1_u10purecfma(float);
IMPORT CONST float Sleef_finz_asinf1_u10purecfma(float);
IMPORT CONST float Sleef_acosf1_u10purecfma(float);
IMPORT CONST float Sleef_finz_acosf1_u10purecfma(float);
IMPORT CONST float Sleef_atanf1_u10purecfma(float);
IMPORT CONST float Sleef_finz_atanf1_u10purecfma(float);
IMPORT CONST float Sleef_atan2f1_u10purecfma(float, float);
IMPORT CONST float Sleef_finz_atan2f1_u10purecfma(float, float);
IMPORT CONST float Sleef_logf1_u10purecfma(float);
IMPORT CONST float Sleef_finz_logf1_u10purecfma(float);
IMPORT CONST float Sleef_cbrtf1_u10purecfma(float);
IMPORT CONST float Sleef_finz_cbrtf1_u10purecfma(float);
IMPORT CONST float Sleef_expf1_u10purecfma(float);
IMPORT CONST float Sleef_finz_expf1_u10purecfma(float);
IMPORT CONST float Sleef_powf1_u10purecfma(float, float);
IMPORT CONST float Sleef_finz_powf1_u10purecfma(float, float);
IMPORT CONST float Sleef_sinhf1_u10purecfma(float);
IMPORT CONST float Sleef_finz_sinhf1_u10purecfma(float);
IMPORT CONST float Sleef_coshf1_u10purecfma(float);
IMPORT CONST float Sleef_finz_coshf1_u10purecfma(float);
IMPORT CONST float Sleef_tanhf1_u10purecfma(float);
IMPORT CONST float Sleef_finz_tanhf1_u10purecfma(float);
IMPORT CONST float Sleef_sinhf1_u35purecfma(float);
IMPORT CONST float Sleef_finz_sinhf1_u35purecfma(float);
IMPORT CONST float Sleef_coshf1_u35purecfma(float);
IMPORT CONST float Sleef_finz_coshf1_u35purecfma(float);
IMPORT CONST float Sleef_tanhf1_u35purecfma(float);
IMPORT CONST float Sleef_finz_tanhf1_u35purecfma(float);
IMPORT CONST float Sleef_fastsinf1_u3500purecfma(float);
IMPORT CONST float Sleef_finz_fastsinf1_u3500purecfma(float);
IMPORT CONST float Sleef_fastcosf1_u3500purecfma(float);
IMPORT CONST float Sleef_finz_fastcosf1_u3500purecfma(float);
IMPORT CONST float Sleef_fastpowf1_u3500purecfma(float, float);
IMPORT CONST float Sleef_finz_fastpowf1_u3500purecfma(float, float);
IMPORT CONST float Sleef_asinhf1_u10purecfma(float);
IMPORT CONST float Sleef_finz_asinhf1_u10purecfma(float);
IMPORT CONST float Sleef_acoshf1_u10purecfma(float);
IMPORT CONST float Sleef_finz_acoshf1_u10purecfma(float);
IMPORT CONST float Sleef_atanhf1_u10purecfma(float);
IMPORT CONST float Sleef_finz_atanhf1_u10purecfma(float);
IMPORT CONST float Sleef_exp2f1_u10purecfma(float);
IMPORT CONST float Sleef_finz_exp2f1_u10purecfma(float);
IMPORT CONST float Sleef_exp2f1_u35purecfma(float);
IMPORT CONST float Sleef_finz_exp2f1_u35purecfma(float);
IMPORT CONST float Sleef_exp10f1_u10purecfma(float);
IMPORT CONST float Sleef_finz_exp10f1_u10purecfma(float);
IMPORT CONST float Sleef_exp10f1_u35purecfma(float);
IMPORT CONST float Sleef_finz_exp10f1_u35purecfma(float);
IMPORT CONST float Sleef_expm1f1_u10purecfma(float);
IMPORT CONST float Sleef_finz_expm1f1_u10purecfma(float);
IMPORT CONST float Sleef_log10f1_u10purecfma(float);
IMPORT CONST float Sleef_finz_log10f1_u10purecfma(float);
IMPORT CONST float Sleef_log2f1_u10purecfma(float);
IMPORT CONST float Sleef_finz_log2f1_u10purecfma(float);
IMPORT CONST float Sleef_log2f1_u35purecfma(float);
IMPORT CONST float Sleef_finz_log2f1_u35purecfma(float);
IMPORT CONST float Sleef_log1pf1_u10purecfma(float);
IMPORT CONST float Sleef_finz_log1pf1_u10purecfma(float);
IMPORT CONST Sleef_float_2 Sleef_sincospif1_u05purecfma(float);
IMPORT CONST Sleef_float_2 Sleef_finz_sincospif1_u05purecfma(float);
IMPORT CONST Sleef_float_2 Sleef_sincospif1_u35purecfma(float);
IMPORT CONST Sleef_float_2 Sleef_finz_sincospif1_u35purecfma(float);
IMPORT CONST float Sleef_sinpif1_u05purecfma(float);
IMPORT CONST float Sleef_finz_sinpif1_u05purecfma(float);
IMPORT CONST float Sleef_cospif1_u05purecfma(float);
IMPORT CONST float Sleef_finz_cospif1_u05purecfma(float);
IMPORT CONST float Sleef_fmaf1_purecfma(float, float, float);
IMPORT CONST float Sleef_finz_fmaf1_purecfma(float, float, float);
IMPORT CONST float Sleef_sqrtf1_purecfma(float);
IMPORT CONST float Sleef_finz_sqrtf1_purecfma(float);
IMPORT CONST float Sleef_sqrtf1_u05purecfma(float);
IMPORT CONST float Sleef_finz_sqrtf1_u05purecfma(float);
IMPORT CONST float Sleef_sqrtf1_u35purecfma(float);
IMPORT CONST float Sleef_finz_sqrtf1_u35purecfma(float);
IMPORT CONST float Sleef_hypotf1_u05purecfma(float, float);
IMPORT CONST float Sleef_finz_hypotf1_u05purecfma(float, float);
IMPORT CONST float Sleef_hypotf1_u35purecfma(float, float);
IMPORT CONST float Sleef_finz_hypotf1_u35purecfma(float, float);
IMPORT CONST float Sleef_fabsf1_purecfma(float);
IMPORT CONST float Sleef_finz_fabsf1_purecfma(float);
IMPORT CONST float Sleef_copysignf1_purecfma(float, float);
IMPORT CONST float Sleef_finz_copysignf1_purecfma(float, float);
IMPORT CONST float Sleef_fmaxf1_purecfma(float, float);
IMPORT CONST float Sleef_finz_fmaxf1_purecfma(float, float);
IMPORT CONST float Sleef_fminf1_purecfma(float, float);
IMPORT CONST float Sleef_finz_fminf1_purecfma(float, float);
IMPORT CONST float Sleef_fdimf1_purecfma(float, float);
IMPORT CONST float Sleef_finz_fdimf1_purecfma(float, float);
IMPORT CONST float Sleef_truncf1_purecfma(float);
IMPORT CONST float Sleef_finz_truncf1_purecfma(float);
IMPORT CONST float Sleef_floorf1_purecfma(float);
IMPORT CONST float Sleef_finz_floorf1_purecfma(float);
IMPORT CONST float Sleef_ceilf1_purecfma(float);
IMPORT CONST float Sleef_finz_ceilf1_purecfma(float);
IMPORT CONST float Sleef_roundf1_purecfma(float);
IMPORT CONST float Sleef_finz_roundf1_purecfma(float);
IMPORT CONST float Sleef_rintf1_purecfma(float);
IMPORT CONST float Sleef_finz_rintf1_purecfma(float);
IMPORT CONST float Sleef_nextafterf1_purecfma(float, float);
IMPORT CONST float Sleef_finz_nextafterf1_purecfma(float, float);
IMPORT CONST float Sleef_frfrexpf1_purecfma(float);
IMPORT CONST float Sleef_finz_frfrexpf1_purecfma(float);
IMPORT CONST float Sleef_fmodf1_purecfma(float, float);
IMPORT CONST float Sleef_finz_fmodf1_purecfma(float, float);
IMPORT CONST float Sleef_remainderf1_purecfma(float, float);
IMPORT CONST float Sleef_finz_remainderf1_purecfma(float, float);
IMPORT CONST Sleef_float_2 Sleef_modff1_purecfma(float);
IMPORT CONST Sleef_float_2 Sleef_finz_modff1_purecfma(float);
IMPORT CONST float Sleef_lgammaf1_u10purecfma(float);
IMPORT CONST float Sleef_finz_lgammaf1_u10purecfma(float);
IMPORT CONST float Sleef_tgammaf1_u10purecfma(float);
IMPORT CONST float Sleef_finz_tgammaf1_u10purecfma(float);
IMPORT CONST float Sleef_erff1_u10purecfma(float);
IMPORT CONST float Sleef_finz_erff1_u10purecfma(float);
IMPORT CONST float Sleef_erfcf1_u15purecfma(float);
IMPORT CONST float Sleef_finz_erfcf1_u15purecfma(float);
IMPORT CONST int Sleef_getIntf1_purecfma(int);
IMPORT CONST int Sleef_finz_getIntf1_purecfma(int);
IMPORT CONST void *Sleef_getPtrf1_purecfma(int);
IMPORT CONST void *Sleef_finz_getPtrf1_purecfma(int);
#endif
#ifdef __cplusplus
}
#endif

#undef IMPORT
#endif // #ifndef __SLEEF_H__
