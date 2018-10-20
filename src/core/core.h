#pragma once

#include "../dd_dll_global.h"

#ifdef __cplusplus
extern "C" {
#endif

int DD_INNER_SHARED_EXPORT controllerMain(int argc, char *argv[]);
int DD_INNER_SHARED_EXPORT playerMain(int argc, char *argv[]);
int DD_INNER_SHARED_EXPORT serviceMain(int argc, char **);
int DD_INNER_SHARED_EXPORT updaterMain(int argc, char *argv[]);

#ifdef __cplusplus
}
#endif
