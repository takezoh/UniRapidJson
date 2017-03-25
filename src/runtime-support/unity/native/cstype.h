// Copyright (c) 2017 Takehito Gondo
//
// UniRapidJson is released under the MIT License.
// http://opensource.org/licenses/mit-license.php

#ifndef cstype_h
#define cstype_h

namespace cstype {
    typedef uint8_t  BYTE;
    typedef int8_t   SBYTE;
    typedef int16_t  SHORT;
    typedef uint16_t USHORT;
    typedef int32_t  INT;
    typedef uint32_t UINT;
    typedef int64_t  LONG;
    typedef uint64_t ULONG;
    typedef float    FLOAT;
    typedef double   DOUBLE;
    typedef int8_t   BOOL;
    typedef const char* STRING;
    typedef uint32_t SIZE;

    template <typename T>
    struct INTPTR {
        typedef const T* CP;
        typedef T* P;
    };
}

#endif /* cstype_h */
