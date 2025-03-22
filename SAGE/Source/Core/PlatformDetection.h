#pragma once

#if defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#define PLATFORM_WINDOWS
#if defined(_WIN64)
#define PLATFORM_WINDOWS_64
#else
#define PLATFORM_WINDOWS_32
#endif
#elif defined(__APPLE__)
#define PLATFORM_APPLE
#include <TargetConditionals.h>
#if TARGET_IPHONE_SIMULATOR
#define PLATFORM_IOS_SIMULATOR
#elif TARGET_OS_MACCATALYST
#define PLATFORM_MAC_CATALYST
#elif TARGET_OS_IPHONE
#define PLATFORM_IOS
#elif TARGET_OS_MAC
#define PLATFORM_MACOS
#else
#error "Unknown Apple platform"
#endif
#elif defined(__ANDROID__)
#define PLATFORM_ANDROID
#elif defined(__linux__)
#define PLATFORM_LINUX
#elif defined(__FreeBSD__)
#define PLATFORM_FREEBSD
#elif defined(__OpenBSD__)
#define PLATFORM_OPENBSD
#elif defined(__NetBSD__)
#define PLATFORM_NETBSD
#elif defined(__DragonFly__)
#define PLATFORM_DRAGONFLY
#elif defined(__unix__)
#define PLATFORM_UNIX
#elif defined(_POSIX_VERSION)
#define PLATFORM_POSIX
#elif defined(__HAIKU__)
#define PLATFORM_HAIKU
#elif defined(__EMSCRIPTEN__)
#define PLATFORM_EMSCRIPTEN
#elif defined(__riscos__)
#define PLATFORM_RISCOS
#elif defined(__asmjs__)
#define PLATFORM_ASMJS
#else
#error "Unknown platform"
#endif