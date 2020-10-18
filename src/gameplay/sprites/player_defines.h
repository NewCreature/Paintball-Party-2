#ifndef PP2_PLAYER_DEFINES_H
#define PP2_PLAYER_DEFINES_H

#define PP2_PLAYER_MAX_WEAPONS  8
#define PP2_PLAYER_MAX_TIMERS  16

#define PP2_PLAYER_FLAG_ACTIVE         1
#define PP2_PLAYER_FLAG_GROUND         2
#define PP2_PLAYER_FLAG_POWER_CLOAK    4
#define PP2_PLAYER_FLAG_POWER_JUMP     8
#define PP2_PLAYER_FLAG_POWER_RUN     16
#define PP2_PLAYER_FLAG_POWER_DEFLECT 32
#define PP2_PLAYER_FLAG_POWER_FLY     64
#define PP2_PLAYER_FLAG_POWER_TURBO  128
#define PP2_PLAYER_FLAG_TYPING       256

#define PP2_PLAYER_TIMER_CLOAK         0
#define PP2_PLAYER_TIMER_JUMP          1
#define PP2_PLAYER_TIMER_RUN           2
#define PP2_PLAYER_TIMER_DEFLECT       3
#define PP2_PLAYER_TIMER_FLY           4
#define PP2_PLAYER_TIMER_TURBO         5
#define PP2_PLAYER_TIMER_WEAPON_SELECT 6
#define PP2_PLAYER_TIMER_STOMPED       7

#define PP2_PLAYER_STEP_SELECT_PROFILE     0 // selecting profile
#define PP2_PLAYER_STEP_SELECTED_PROFILE   1 // wait for network to go to step 2
#define PP2_PLAYER_STEP_SELECT_CHARACTER   2 // player is selecting
#define PP2_PLAYER_STEP_CHARACTER_WAIT     3 // if waiting, render preview semi-transparent
#define PP2_PLAYER_STEP_CHARACTER_FOUND    4 // character was found so render only darkly
#define PP2_PLAYER_STEP_SELECTED_CHARACTER 5 // character was selected
#define PP2_PLAYER_STEP_DONE               6 // player is done

#endif
