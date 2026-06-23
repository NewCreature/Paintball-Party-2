#ifndef PP2_TEXT_ENTRY_H
#define PP2_TEXT_ENTRY_H

void pp2_enter_text(const char * text, int mode);
bool pp2_enter_text_logic(void);
const char * pp2_get_entered_text(void);
int pp2_get_text_entry_state(void);

#endif
