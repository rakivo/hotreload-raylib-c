#pragma once

typedef void  (*plug_init_t)(void);
typedef void  (*plug_free_t)(void);
typedef void  (*plug_frame_t)(void);
typedef void *(*plug_pre_reload_t)(void);
typedef void  (*plug_post_reload_t)(void*);
