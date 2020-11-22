#ifndef HEADER_string
#define HEADER_string

#define FORMAT_DEC_UNSIGNED_TIGHT   0
#define FORMAT_DEC_SIGNED_TIGHT     2
#define FORMAT_HEX_LWRCASE_TIGHT    4
#define FORMAT_HEX_LWRCASE_PAD      5
#define FORMAT_HEX_UPRCASE_TIGHT    6
#define FORMAT_HEX_UPRCASE_PAD      7
#define FORMAT_BIN_TIGHT            8

#define ASCII_ZERO  48
#define ASCII_A     65
#define ASCII_a     97

void to_string(char* buf, int x, int format);

#endif