/* definitions for keys and commands here */
/* also - TODO add keyboardlayouts
extern const uint8_t KeyboardLayout_de_DE[];
extern const uint8_t KeyboardLayout_en_US[];
extern const uint8_t KeyboardLayout_es_ES[];
extern const uint8_t KeyboardLayout_fr_FR[];
extern const uint8_t KeyboardLayout_it_IT[];
extern const uint8_t KeyboardLayout_pt_PT[];
extern const uint8_t KeyboardLayout_sv_SE[];
extern const uint8_t KeyboardLayout_da_DK[];
extern const uint8_t KeyboardLayout_hu_HU[];
extern const uint8_t KeyboardLayout_pt_BR[];
*/


#ifdef CALCULATE_HASHES
static char *commandsToHash[] = {
    "REM",
    "ID",
    "DELAY",
    "STRING",
    "STRINGLN", 
    "DEFAULT_DELAY",
    "DEFAULTDELAY",
    "STRINGDELAY",
    "STRING_DELAY",
    "DEFAULT_STRING_DELAY",
    "DEFAULTSTRINGDELAY", 
    "REPEAT", 
    "HOLD", 
    "RELEASE",
    "WAIT_FOR_BUTTON_PRESS",
};

//mod keys
static char *modifier_keys[] = {
    "CTRL",
    "CONTROL",
    "SHIFT",
    "ALT",
    "GUI",
    "WINDOWS",
};

static char *single_keys[] = {
    "DOWNARROW",
    "DOWN",
    "LEFTARROW",
    "LEFT", 
    "RIGHTARROW",
    "RIGHT", 
    "UPARROW",
    "UP",

    "ENTER", 
    "BREAK",
    "PAUSE",
    "CAPSLOCK",
    "DELETE", 
    "BACKSPACE", 
    "END",
    "ESC",
    "ESCAPE",
    "HOME",
    "INSERT",
    "NUMLOCK",
    "PAGEUP",
    "PAGEDOWN",
    "PRINTSCREEN",
    "SCROLLLOCK",
    "SPACE",
    "TAB",
    "MENU",
    "APP",

    "F1", 
    "F2", 
    "F3", 
    "F4", 
    "F5", 
    "F6", 
    "F7", 
    "F8", 
    "F9", 
    "F10", 
    "F11", 
    "F12", 
    "F13", 
    "F14", 
    "F15", 
    "F16", 
    "F17", 
    "F18", 
    "F19", 
    "F20", 
    "F21", 
    "F22", 
    "F23", 
    "F24", 
};

//mouse commands
static char *mouse_commands[] = {
    "LEFTCLICK",
    "LEFT_CLICK",
    "RIGHTCLICK",
    "RIGHT_CLICK",
    "MOUSEMOVE",
    "MOUSE_MOVE",
    "MOUSESCROLL",
    "MOUSE_SCROLL"
};

#endif
