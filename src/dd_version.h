#pragma once

#ifdef DD_HAVE_VERSION_H
#include "../version_ci.h"
#endif

#ifndef DD_MAJOR_VERSION
#define DD_MAJOR_VERSION 1     //((DD_VERSION&0xff0000)>>16)
#endif
#ifndef DD_MINOR_VERSION
#define DD_MINOR_VERSION 0     //((DD_VERSION&0xff00)>>8)
#endif
#ifndef DD_PATCH_VERSION
#define DD_PATCH_VERSION 0     //(DD_VERSION&0xff)
#endif
#ifndef DD_BUILD_VERSION
#define DD_BUILD_VERSION 0
#endif

#ifndef DD_VERSION_CHECK
#define DD_VERSION_CHECK(major, minor, patch) (((major&0xff)<<16) | ((minor&0xff)<<8) | (patch&0xff))
#endif

#ifndef DD_VERSION
#define DD_VERSION DD_VERSION_CHECK(DD_MAJOR_VERSION, DD_MINOR_VERSION, DD_PATCH_VERSION)
#endif

/*! Stringify \a x. */
#define _TOSTR(x) #x
/*! Stringify \a x, perform macro expansion. */
#define TOSTR(x) _TOSTR(x)

/* the following are compile time version */
/* C++11 requires a space between literal and identifier */
#ifndef DD_VERSION_STR
#define DD_VERSION_STR TOSTR(DD_MAJOR_VERSION) "." TOSTR(DD_MINOR_VERSION) "." TOSTR(DD_PATCH_VERSION) "." TOSTR(DD_BUILD_VERSION)
#endif
