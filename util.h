#pragma clang diagnostic push
#pragma ide diagnostic ignored "modernize-deprecated-headers"
// SPDX-License-Identifier:
// Copyright (c) 2025 Simon Biewald
//
// Utility header, mostly from Arduino.h

#ifndef BUZZER_UTIL_H
#define BUZZER_UTIL_H

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include <stddef.h>

#ifdef __cplusplus
extern "C"{
#endif

#define PI 3.1415926535897932384626433832795
#define HALF_PI 1.5707963267948966192313216916398
#define TWO_PI 6.283185307179586476925286766559
#define DEG_TO_RAD 0.017453292519943295769236907684886
#define RAD_TO_DEG 57.295779513082320876798154814105
#define EULER 2.718281828459045235360287471352

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitToggle(value, bit) ((value) ^= (1UL << (bit)))
#define bitWrite(value, bit, bitvalue) ((bitvalue) ? bitSet(value, bit) : bitClear(value, bit))

#define bit(b) (1UL << (b))

#ifdef __cplusplus
} // extern "C"
#endif

// Undefine stdlib's abs if encountered
#ifdef abs
#undef abs
#endif

#ifdef __cplusplus
template<class T>
	auto abs(const T& x) -> decltype(x > 0 ? x : -x) {
		return x > 0 ? x : -x;
	}

	template<class T, class L>
	auto min(const T& a, const L& b) -> decltype((b < a) ? b : a) {
		return (b < a) ? b : a;
	}

	template<class T, class L>
	auto max(const T& a, const L& b) -> decltype((b < a) ? b : a) {
		return (a < b) ? b : a;
	}

	template<class T>
	long round(const T& x) {
		return (long)(x >= 0 ? (x + 0.5) : (x - 0.5));
	}

	template<class T>
	auto sq(const T& x) -> decltype(x * x) {
		return x * x;
	}

  template<class T>
  auto radians(const T& deg) -> decltype(deg * DEG_TO_RAD) {
    return deg * DEG_TO_RAD;
  }

  template<class T>
  auto degrees(const T& rad) -> decltype(rad * RAD_TO_DEG) {
    return rad * RAD_TO_DEG;
  }

  template<class T, class L, class H>
	auto constrain(const T& x, const L& l, const H& h) -> decltype((x < l) ? l : (x > h) ? h : x) {
		return (x < l) ? l : (x > h) ? h : x;
	}

#else
#define abs(x)       ({ typeof (x) _x = (x); _x > 0 ? _x : -_x; })
#define min(a,b)     ({ typeof (a) _a = (a); typeof (b) _b = (b); _a < _b ? _a : _b; })
#define max(a,b)     ({ typeof (a) _a = (a); typeof (b) _b = (b); _a > _b ? _a : _b; })
#define sq(x)        ({ typeof (x) _x = (x); _x * _x; })
#define radians(deg) ((deg) * DEG_TO_RAD)
#define degrees(rad) ((rad) * RAD_TO_DEG)
#define constrain(x,low,high)     ({ \
  typeof (x) _x = (x);               \
  typeof (low) _l = (low);           \
  typeof (high) _h = (high);         \
  _x < _l ? _l : _x > _h ? _h : _x; })
#endif

#endif //BUZZER_UTIL_H

#pragma clang diagnostic pop
