// Copyright (c) 2017 Takehito Gondo
//
// UniRapidJson is released under the MIT License.
// http://opensource.org/licenses/mit-license.php

#ifndef platform_env_h
#define platform_env_h

#if defined(__APPLE__)
    #include <TargetConditionals.h>
    #if TARGET_OS_IPHONE
        #define JSON_IOS
    #endif
#elif defined(ANDROID)
	#define JSON_ANDROID
#elif defined(_WINDLL)
	#define JSON_WINDOWS
#endif

#ifdef JSON_WINDOWS
	#define DLLEXPORT __declspec (dllexport)
#else
	#define DLLEXPORT
#endif

#endif /* platform_env_h */
