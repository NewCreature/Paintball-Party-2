#ifndef AVC_H
#define AVC_H

bool avc_start_capture(ALLEGRO_DISPLAY * display,
    const char * out_filename,
    bool (*init_proc)(void * data),
    bool (*logic_proc)(void * data),
    void (*render_proc)(void * data),
    int fps,
    int flags,
    void * data);

#endif
