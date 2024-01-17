#include QMK_KEYBOARD_H

enum layers {
    _QWERTY = 0,
    _NAV,
    _SYM,
    _NUMPAD,
};


#define RGUI_SCN  RGUI_T(KC_SCLN)
#define LGUI_A  LGUI_T(KC_A)
#define LALT_S  LALT_T(KC_S)
#define LCTL_D  LCTL_T(KC_D)
#define LCTL_ESC  LCTL_T(KC_ESC)
#define RCTL_QOT  RCTL_T(KC_QUOT)
#define RCTL_K  RCTL_T(KC_K)
#define RALT_QOT  RALT_T(KC_QUOT)
#define RALT_L	  RALT_T(KC_L)
//NUMPAD NO LONGER USED
//#define LAYER3_D  LT(3,KC_D)
#define LAYER2_F  LT(2,KC_F)
#define LAYER1_G  LT(1,KC_G)
#define LAYER2_J  LT(2,KC_J)
//NUMPAD NO LONGER USED
//#define LAYER3_K  LT(3,KC_K)

#define BUFF_LEN 6

uint16_t	usr_keystrokes = 0;
uint16_t	usr_time = 0;
uint32_t	timer = 0;
uint32_t	timer_key = 0;
char		key_strokes_str[BUFF_LEN] = "Hello";
char		time_str[BUFF_LEN] = "time0";

uint8_t		nbrlen(uint16_t nbr)
{
	uint8_t	i = 0;

	if (nbr == 0)
		return (1);
	while (nbr > 0)
	{
		nbr /= 10;
		++i;
	}
	return (i);
}

void		clear_str(char *str, uint8_t size)
{
	while (size > 0)
		str[--size] = ' ';
}

void		bzero_str(char *str, uint8_t size)
{
	while (size > 0)
		str[--size] = 0;
}

//To be called with strlen(str) - 1
void		itostr_pad(char *str, uint8_t pos, uint16_t nbr)
{
	if (pos < 0)
		return;
	if (str[pos + 1] > '0' && str[pos + 1] <= '9' && nbr == 0)
	{
		str[pos] = ' ';
		return;
	}
	if (nbr <= 9)
	{
		str[pos] = '0' + nbr;
		return;
	}
	itostr_pad(str, pos - 1, nbr / 10);
	itostr_pad(str, pos, nbr % 10);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case KC_BSPC:
      if (record->event.pressed) {
		  --usr_keystrokes;
      }
	  break;
    default:
      if (record->event.pressed) {
		++usr_keystrokes;
	  }
  }
	clear_str(key_strokes_str, BUFF_LEN - 1);
	itostr_pad(key_strokes_str, BUFF_LEN - 2 , usr_keystrokes);
	timer_key = timer_read32();
	/*
	This one is for no padding
	bzero_str(key_strokes_str, BUFF_LEN - 1);
	itostr_pad(key_strokes_str, nbrlen(usr_keystrokes) - 1 , usr_keystrokes);
	*/
	return true; // Process all keycodes normally
}

void matrix_scan_user(void)
{
	if (timer_elapsed32(timer) > 1000)
	{
		timer = timer_read32();
		++usr_time;
		if (timer_elapsed32(timer_key) < OLED_TIMEOUT)
		{
			clear_str(time_str, BUFF_LEN - 1);
			itostr_pad(time_str, BUFF_LEN - 2 , usr_time);
		}
	}
}


bool oled_task_user(void) {
    if (is_keyboard_master()) {
        // QMK Logo and version information
        // clang-format off
        static const char PROGMEM qmk_logo[] = {
            0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,0x90,0x91,0x92,0x93,0x94,
            0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xab,0xac,0xad,0xae,0xaf,0xb0,0xb1,0xb2,0xb3,0xb4,
            0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xcb,0xcc,0xcd,0xce,0xcf,0xd0,0xd1,0xd2,0xd3,0xd4,0};
        // clang-format on

        oled_write_P(qmk_logo, false);
        oled_write_P(PSTR("Kyria "), false);
        oled_write_P(PSTR("rev2\n"), false);
        oled_write_P(PSTR("Custom fle-blay\n"), false);
        oled_write_P(PSTR("["), false);
		oled_write(key_strokes_str, false);
        oled_write_P(PSTR("]["), false);
		oled_write(time_str, false);
        oled_write_P(PSTR("]\n"), false);
        // Host Keyboard Layer Status
        oled_write_P(PSTR("Layer: "), false);
        switch (get_highest_layer(layer_state | default_layer_state)) {
            case 0:
                oled_write_P(PSTR("Qwerty\n"), false);
                break;
            case 1:
                oled_write_P(PSTR("Navigation\n"), false);
                break;
            case 2:
                oled_write_P(PSTR("Num/Sym\n"), false);
                break;
            case 3:
                oled_write_P(PSTR("Numpad\n"), false);
                break;
            default:
                oled_write_P(PSTR("Undefined\n"), false);
        }

        // Host Keyboard LED Status
        led_t led_usb_state = host_keyboard_led_state();
        oled_write_P(led_usb_state.num_lock ? PSTR("NUMLCK ") : PSTR("       "), false);
        oled_write_P(led_usb_state.caps_lock ? PSTR("CAPLCK ") : PSTR("       "), false);
        oled_write_P(led_usb_state.scroll_lock ? PSTR("SCRLCK ") : PSTR("       "), false);
    } else {
        // clang-format off
        static const char PROGMEM kyria_logo[] = {
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,192,224,240,112,120, 56, 60, 28, 30, 14, 14, 14,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  7, 14, 14, 14, 30, 28, 60, 56,120,112,240,224,192,128,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,192,224,240,124, 62, 31, 15,  7,  3,  1,128,192,224,240,120, 56, 60, 28, 30, 14, 14,  7,  7,135,231,127, 31,255,255, 31,127,231,135,  7,  7, 14, 14, 30, 28, 60, 56,120,240,224,192,128,  1,  3,  7, 15, 31, 62,124,240,224,192,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,240,252,255, 31,  7,  1,  0,  0,192,240,252,254,255,247,243,177,176, 48, 48, 48, 48, 48, 48, 48,120,254,135,  1,  0,  0,255,255,  0,  0,  1,135,254,120, 48, 48, 48, 48, 48, 48, 48,176,177,243,247,255,254,252,240,192,  0,  0,  1,  7, 31,255,252,240,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,255,255,255,  0,  0,  0,  0,  0,254,255,255,  1,  1,  7, 30,120,225,129,131,131,134,134,140,140,152,152,177,183,254,248,224,255,255,224,248,254,183,177,152,152,140,140,134,134,131,131,129,225,120, 30,  7,  1,  1,255,255,254,  0,  0,  0,  0,  0,255,255,255,  0,  0,  0,  0,255,255,  0,  0,192,192, 48, 48,  0,  0,240,240,  0,  0,  0,  0,  0,  0,240,240,  0,  0,240,240,192,192, 48, 48, 48, 48,192,192,  0,  0, 48, 48,243,243,  0,  0,  0,  0,  0,  0, 48, 48, 48, 48, 48, 48,192,192,  0,  0,  0,  0,  0,
            0,  0,  0,255,255,255,  0,  0,  0,  0,  0,127,255,255,128,128,224,120, 30,135,129,193,193, 97, 97, 49, 49, 25, 25,141,237,127, 31,  7,255,255,  7, 31,127,237,141, 25, 25, 49, 49, 97, 97,193,193,129,135, 30,120,224,128,128,255,255,127,  0,  0,  0,  0,  0,255,255,255,  0,  0,  0,  0, 63, 63,  3,  3, 12, 12, 48, 48,  0,  0,  0,  0, 51, 51, 51, 51, 51, 51, 15, 15,  0,  0, 63, 63,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 48, 48, 63, 63, 48, 48,  0,  0, 12, 12, 51, 51, 51, 51, 51, 51, 63, 63,  0,  0,  0,  0,  0,
            0,  0,  0,  0, 15, 63,255,248,224,128,  0,  0,  3, 15, 63,127,255,239,207,141, 13, 12, 12, 12, 12, 12, 12, 12, 30,127,225,128,  0,  0,255,255,  0,  0,128,225,127, 30, 12, 12, 12, 12, 12, 12, 12, 13,141,207,239,255,127, 63, 15,  3,  0,  0,128,224,248,255, 63, 15,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  3,  7, 15, 62,124,248,240,224,192,128,  1,  3,  7, 15, 30, 28, 60, 56,120,112,112,224,224,225,231,254,248,255,255,248,254,231,225,224,224,112,112,120, 56, 60, 28, 30, 15,  7,  3,  1,128,192,224,240,248,124, 62, 15,  7,  3,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  3,  7, 15, 14, 30, 28, 60, 56,120,112,112,112,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,224,112,112,112,120, 56, 60, 28, 30, 14, 15,  7,  3,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
        };
        // clang-format on
        oled_write_raw_P(kyria_logo, sizeof(kyria_logo));
    }
    return false;
}


/*
bool get_tapping_force_hold(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case LSFT_T(KC_SPC):
            return true;
        default:
            return false;
    }
}
*/

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case LAYER2_F:
        case LAYER2_J:
            return TAPPING_TERM - 20;
        default:
            return TAPPING_TERM;
    }
}

bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case LSFT_T(KC_SPC):
        case RSFT_T(KC_ENT):
            return true;
        default:
            return false;
    }
}

bool get_combo_must_tap(uint16_t index, combo_t *combo) {
    return true;
}

const uint16_t PROGMEM combo1[] = {LCTL_D, LAYER2_F, COMBO_END};
const uint16_t PROGMEM combo2[] = {LALT_S, LCTL_D, COMBO_END};
const uint16_t PROGMEM combo3[] = {RCTL_K, RALT_L, COMBO_END};
const uint16_t PROGMEM combo4[] = {LAYER2_J, RALT_L, COMBO_END};
const uint16_t PROGMEM combo5[] = {KC_C, KC_V, COMBO_END};
const uint16_t PROGMEM combo6[] = {KC_M, KC_COMM, COMBO_END};
const uint16_t PROGMEM combo7[] = {LAYER2_J, RCTL_K, COMBO_END};
const uint16_t PROGMEM combo8[] = {LALT_S, LAYER2_F, COMBO_END};
combo_t key_combos[COMBO_COUNT] = {
    COMBO(combo1, KC_ESC),
    COMBO(combo2, KC_GRV),
    COMBO(combo3, KC_QUOT),
    COMBO(combo4, KC_BSLS),
    COMBO(combo5, KC_MINS),
    COMBO(combo6, KC_EQL),
    COMBO(combo7, KC_BSPC),
    COMBO(combo8, KC_TAB)
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	[_QWERTY] = LAYOUT(
	KC_NO	, KC_Q	, KC_W	, KC_E	, KC_R	, KC_T	,														KC_Y, KC_U, KC_I, KC_O, KC_P, KC_NO,
	KC_NO, LGUI_A, LALT_S, LCTL_D, LAYER2_F, LAYER1_G,													KC_H, LAYER2_J, RCTL_K, RALT_L, RGUI_SCN, KC_NO,
	KC_NO, KC_Z, KC_X, KC_C, KC_V, KC_B, LAG_SWP, LAG_NRM,									RAG_NRM, RAG_SWP, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_NO,
							KC_NO, KC_NO, LSFT_T(KC_SPC), KC_TAB, KC_NO,			 KC_NO, KC_BSPC, RSFT_T(KC_ENT), KC_NO, KC_NO
	),

	[_NAV] = LAYOUT(
	KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,KC_TRNS,													 KC_HOME, KC_PGDN, KC_PGUP, KC_END, KC_VOLU, KC_DEL,
	KC_TRNS, KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT,KC_TRNS,													KC_LEFT, KC_DOWN, KC_UP, KC_RGHT, KC_VOLD, KC_INS,
	KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,KC_SCRL,					KC_TRNS, KC_TRNS, KC_PAUS, KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE, KC_PSCR,
								KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,				KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
	),

	[_SYM] = LAYOUT(
	KC_BSLS, KC_1, KC_2, KC_3, KC_4, KC_5,																	KC_6, KC_7, KC_8, KC_9, KC_0, KC_EQL,
	KC_PIPE, KC_EXLM, KC_AT, KC_HASH, KC_DLR, KC_PERC,														KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_PLUS,
	KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_TRNS,								KC_TRNS, KC_NO, KC_NO, KC_LBRC, KC_RBRC, KC_LCBR, KC_RCBR, KC_NO,
						KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,						KC_TRNS, KC_GRV, KC_TRNS, KC_TRNS, KC_TRNS
	),

    //NUMPAD NO LONGER USED
	[_NUMPAD] = LAYOUT(
	KC_TRNS, KC_F9, KC_F10, KC_F11, KC_F12, KC_TRNS,														KC_TRNS, KC_P7, KC_P8, KC_P9, KC_TRNS, KC_TRNS,
	KC_TRNS, KC_F5, KC_F6, KC_F7, KC_F8, KC_TRNS,															KC_TRNS, KC_P4, KC_P5, KC_P6, KC_P0, KC_TRNS,
	KC_TRNS, KC_F1, KC_F2, KC_F3, KC_F4, KC_TRNS, KC_TRNS, KC_TRNS,							KC_TRNS, KC_TRNS, KC_TRNS, KC_P1, KC_P2, KC_P3, KC_TRNS, KC_TRNS,
							KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,					KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
	),
};
