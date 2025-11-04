#pragma once

enum class KeyCode {
    // Letters
    a, b, c, d, e, f, g, h, i, j, k, l, m,
    n, o, p, q, r, s, t, u, v, w, x, y, z,

    // Numbers (top row)
    num_0, num_1, num_2, num_3, num_4,
    num_5, num_6, num_7, num_8, num_9,

    // Function keys
    f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12,

    // Modifiers
    left_shift, right_shift,
    left_control, right_control,
    left_alt, right_alt,
    left_super, right_super,
    caps_lock,

    // Navigation and main keys
    escape, tab, enter, space, backspace, insert, delete_key, home, end,
    page_up, page_down, arrow_up, arrow_down, arrow_left, arrow_right,

    // Symbols and punctuation
    grave_accent,   // `
    minus,          // -
    equal,          // =
    left_bracket,   // [
    right_bracket,  // ]
    backslash,      // \
    semicolon,      // ;
    apostrophe,     // '
    comma,          // ,
    period,         // .
    slash,          // /

    // Numpad
    kp_0, kp_1, kp_2, kp_3, kp_4, kp_5, kp_6, kp_7, kp_8, kp_9,
    kp_decimal, kp_divide, kp_multiply, kp_subtract, kp_add, kp_enter, kp_equal,

    // Fallback
    unknown
};
