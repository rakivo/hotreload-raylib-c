#include <stdio.h>
#include <stdlib.h>

#include <time.h>
#include <dlfcn.h>

#include <raylib.h>

#include "plug.h"

void* libplug;

#define FN(name) name##_t name

#define FN_SYM(name, lib, do_)                          \
  *(void**) (&name)  = dlsym(lib, #name);                \
  if (name == NULL) {                                    \
    TraceLog(LOG_ERROR, "failed to find %s in %s: %s",  \
             #name, LIB_PLUG_PATH, dlerror());          \
    do_;                                                \
  }

FN(plug_init);
FN(plug_free);
FN(plug_frame);
FN(plug_pre_reload);
FN(plug_post_reload);

bool plug_reload(void)
{
  if (libplug) dlclose(libplug);

  libplug = dlopen(LIB_PLUG_PATH, RTLD_NOW);
  if (!libplug) {
    TraceLog(LOG_ERROR, "HOTRELOAD: could not load plug DLL: %s", dlerror());
    return false;
  }
  dlerror();

  FN_SYM(plug_init,        libplug, return false);
  FN_SYM(plug_free,        libplug, return false);
  FN_SYM(plug_frame,       libplug, return false);
  FN_SYM(plug_pre_reload,  libplug, return false);
  FN_SYM(plug_post_reload, libplug, return false);

  TraceLog(LOG_INFO, "Reloaded libplug successfully");

  return true;
}

int main(void)
{
  SetTargetFPS(60);
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);

  InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "plug");

  SetExitKey(KEY_Q);
  SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));

  if (!plug_reload()) return 1;
  plug_init();

  for (; !WindowShouldClose(); plug_frame()) {
    if (IsKeyDown(KEY_LEFT_ALT) && IsKeyPressed(KEY_R)) {
      void *plug = plug_pre_reload();
      if (!plug_reload()) return 1;
      plug_post_reload(plug);
    }
  }

  plug_free();
  CloseAudioDevice();
  CloseWindow();

  return 0;
}
