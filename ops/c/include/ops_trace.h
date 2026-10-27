#ifndef __OPS_TRACE_H
#define __OPS_TRACE_H

// Link-time dispatch trace for the ops_dat_global_fetch_data path. Each tag prints
// its own __FILE__, so the run names the backend definition that actually linked.
//
//   OPS_GFD_TRACE=1   one line per call site, the first time it is reached
//   OPS_GFD_TRACE=2   one line per call, plus the per-dat gather layout detail
//
// Unset means zero output and a single predictable branch per call.

#include <cstdio>
#include <cstdlib>
#include <cstring>

int ops_get_proc();

// __FILE__ arrives absolute from the Makefile; keep only the src-relative tail
static inline const char *ops_trace_file(const char *f) {
  const char *p = strstr(f, "/src/");
  return p ? p + 5 : f;
}

static inline int ops_trace_level() {
  static int level = -1;
  if (level < 0) {
    const char *e = getenv("OPS_GFD_TRACE");
    level = e ? atoi(e) : 0;
  }
  return level;
}

#define OPS_TRACE(fmt, ...)                                                    \
  do {                                                                         \
    int _lvl = ops_trace_level();                                              \
    if (_lvl >= 1) {                                                           \
      static bool _seen = false;                                               \
      if (_lvl >= 2 || !_seen) {                                               \
        _seen = true;                                                          \
        char _loc[128];                                                        \
        snprintf(_loc, sizeof(_loc), "%s:%d", ops_trace_file(__FILE__),         \
                 __LINE__);                                                    \
        printf("[ops-trace r%d] %-42s %-28s " fmt "\n", ops_get_proc(), _loc,  \
               __func__, ##__VA_ARGS__);                                       \
        fflush(stdout);                                                        \
      }                                                                        \
    }                                                                          \
  } while (0)

#endif /* __OPS_TRACE_H */
