#pragma once

#define DD_MAJOR 1     //((DD_VERSION&0xff0000)>>16)
#define DD_MINOR 0     //((DD_VERSION&0xff00)>>8)
#define DD_PATCH 0     //(DD_VERSION&0xff)


#define DD_MAJOR_VERSION(V) ((V & 0xff0000) >> 16)
#define DD_MINOR_VERSION(V) ((V & 0xff00) >> 8)
#define DD_PATCH_VERSION(V) (V & 0xff)

#define DD_VERSION_CHECK(major, minor, patch) \
    (((major&0xff)<<16) | ((minor&0xff)<<8) | (patch&0xff))

#define DD_VERSION DD_VERSION_CHECK(DD_MAJOR, DD_MINOR, DD_PATCH)

/*! Stringify \a x. */
#define _TOSTR(x)   #x
/*! Stringify \a x, perform macro expansion. */
#define TOSTR(x)  _TOSTR(x)


/* the following are compile time version */
/* C++11 requires a space between literal and identifier */
#define DD_VERSION_STR        TOSTR(DD_MAJOR) "." TOSTR(DD_MINOR) "." TOSTR(DD_PATCH)
