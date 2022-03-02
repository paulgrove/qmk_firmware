#pragma once

#ifdef ENABLE_FLUTTERSHY

#ifdef ENABLE_FLUTTERSHY_DISPLAY
void render_fluttershy(uint8_t current_wpm);
#endif

#ifdef ENABLE_FLUTTERSHY_MASTER
void housekeeping_task_user_fluttershy(void);
#endif

#ifdef ENABLE_FLUTTERSHY_SLAVE
void user_sync_fluttershy_slave_handler(uint8_t in_buflen, const void* in_data, uint8_t out_buflen, void* out_data);
#endif

void flapwing_fluttershy(void);
void bobhead_fluttershy(void);

#endif
