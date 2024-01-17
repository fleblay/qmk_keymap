/* Copyright 2022 Thomas Baart <thomas@splitkb.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#ifdef RGBLIGHT_ENABLE
#    define RGBLIGHT_EFFECT_BREATHING
#    define RGBLIGHT_EFFECT_RAINBOW_MOOD
#    define RGBLIGHT_EFFECT_RAINBOW_SWIRL
#    define RGBLIGHT_EFFECT_SNAKE
#    define RGBLIGHT_EFFECT_KNIGHT
#    define RGBLIGHT_EFFECT_CHRISTMAS
#    define RGBLIGHT_EFFECT_STATIC_GRADIENT
#    define RGBLIGHT_EFFECT_RGB_TEST
#    define RGBLIGHT_EFFECT_ALTERNATING
#    define RGBLIGHT_EFFECT_TWINKLE
#    define RGBLIGHT_HUE_STEP 8
#    define RGBLIGHT_SAT_STEP 8
#    define RGBLIGHT_VAL_STEP 8
#    define RGBLIGHT_LIMIT_VAL 150
#endif

//Make Modfier keys (ie home-row mods) behave like layer tap
//and follow HOLD_ON_OTHER_KEY_PRESS_PER_KEY
#define IGNORE_MOD_TAP_INTERRUPT
//Decide per key if the HOLD action should be selected if a key is pressed
//while a dual role key with an hold action is pressed but before TAPPING_TERM
#define HOLD_ON_OTHER_KEY_PRESS_PER_KEY

//Allow use of layer_tap hold keys shortly after tap instead of repeating key
#define TAPPING_FORCE_HOLD
//#define TAPPING_FORCE_HOLD_PER_KEY

#define TAPPING_TERM 170
#define TAPPING_TERM_PER_KEY

#define COMBO_COUNT 8
//Define if a combo must tap to be triggered
#define COMBO_MUST_TAP_PER_COMBO
