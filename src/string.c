#include "../header/string.h"


void dut(char* buf, int x)
{
    unsigned char digit = 0, index = 0, first_digit_found = 0;
    unsigned int query = 1000000000, value = (unsigned int)x;
    for(int i = 0; i < 10; i++)
    {
        digit = value / query;
        if(!first_digit_found && digit > 0)
            first_digit_found = 1;

        if(first_digit_found)
        {
            buf[index++] = digit + ASCII_ZERO;
            value -= digit * query;
        }
        query /= 10;
    }
    buf[index] = 0;
}

void dst(char* buf, int x)
{
    unsigned char digit = 0, index = 0, first_digit_found = 0;
    unsigned int query = 1000000000;

    if(x < 0)
    {
        buf[index++] = '-';
        x = -x;
    }

    for(int i = 0; i < 10; i++)
    {
        digit = x / query;
        if(!first_digit_found && digit > 0)
            first_digit_found = 1;

        if(first_digit_found)
        {
            buf[index++] = digit + ASCII_ZERO;
            x -= digit * query;
        }
        query /= 10;
    }
    buf[index] = 0;
}

void ht(char* buf, int x, int upper)
{
    unsigned char digit = 0, index = 0, first_digit_found = 0;
    unsigned int mask = 0xF0000000, value = (unsigned int)x;

    buf[index++] = '0';
    buf[index++] = 'x';

    for(int i = 0; i < 8; i++)
    {
        digit = (value & mask) >> ((7 - i)*4);
        if(!first_digit_found && digit > 0)
            first_digit_found = 1;

        if(first_digit_found)
        {
            if(digit < 10)
            {
                buf[index++] = (digit + ASCII_ZERO);
            }
            else
            {
                buf[index++] = (digit - 10 + (upper? ASCII_A : ASCII_a));
            }	
        }
                    
        mask = mask >> 4;
    }
    buf[index] = 0;
}

void hp(char* buf, int x, int upper)
{
    unsigned char digit = 0, index = 0;
    unsigned int mask = 0xF0000000, value = (unsigned int)x;

    buf[index++] = '0';
    buf[index++] = 'x';

    for(int i = 0; i < 8; i++)
    {
        digit = (value & mask) >> ((7 - i)*4);

        if(digit < 10)
        {
            buf[index++] = (digit + ASCII_ZERO);
        }
        else
        {
            buf[index++] = (digit - 10 + (upper? ASCII_A : ASCII_a));
        }	
        
        mask = mask >> 4;
    }
    buf[index] = 0;
}

void bt(char* buf, int x)
{
    unsigned char digit = 0, index = 0;
    unsigned int mask = 0x80000000, value = (unsigned int)x;

    buf[index++] = '0';
    buf[index++] = 'b';

    for(int i = 0; i < 32; i++)
    {
        digit = (value & mask) >> ((31 - i));
        buf[index++] = digit ? '1' : '0';
        mask = mask >> 1;
    }

    buf[index] = 0;
}


void to_string(char* buf, int x, int format)
{
    switch (format)
    {
    case FORMAT_DEC_UNSIGNED_TIGHT:
        dut(buf, x);
        break;

    case FORMAT_DEC_SIGNED_TIGHT:
        dst(buf, x);
        break;

    case FORMAT_HEX_LWRCASE_TIGHT:
        ht(buf, x, 0);
        break;

    case FORMAT_HEX_LWRCASE_PAD:
        hp(buf, x, 0);
        break;

    case FORMAT_HEX_UPRCASE_TIGHT:
        ht(buf, x, 1);
        break;
        
    case FORMAT_HEX_UPRCASE_PAD:
        hp(buf, x, 1);
        break;
    
    case FORMAT_BIN_TIGHT:
        bt(buf, x);
        break;
    
    default:
        break;
    }
}

