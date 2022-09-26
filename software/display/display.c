/*
 * display.c
 *
 *  Created on: 2022/08/24
 *      Author: mahdi hamzeh <mahdi2001h@gmail.com>
 */

#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <linux/kd.h>
#include <linux/ioctl.h>

static const char font5x8[] = {

    0x00, 0x00, 0x00, 0x00, 0x00, // (spacja)
    0x00, 0x00, 0x5F, 0x00, 0x00, // !
    0x00, 0x07, 0x00, 0x07, 0x00, // "
    0x14, 0x7F, 0x14, 0x7F, 0x14, // #
    0x24, 0x2A, 0x7F, 0x2A, 0x12, // $
    0x23, 0x13, 0x08, 0x64, 0x62, // %
    0x36, 0x49, 0x55, 0x22, 0x50, // &
    0x00, 0x05, 0x03, 0x00, 0x00, // '
    0x00, 0x1C, 0x22, 0x41, 0x00, // (
    0x00, 0x41, 0x22, 0x1C, 0x00, // )
    0x08, 0x2A, 0x1C, 0x2A, 0x08, // *
    0x08, 0x08, 0x3E, 0x08, 0x08, // +
    0x00, 0x50, 0x30, 0x00, 0x00, // ,
    0x08, 0x08, 0x08, 0x08, 0x08, // -
    0x00, 0x30, 0x30, 0x00, 0x00, // .
    0x20, 0x10, 0x08, 0x04, 0x02, // /
    0x3E, 0x51, 0x49, 0x45, 0x3E, // 0
    0x00, 0x42, 0x7F, 0x40, 0x00, // 1
    0x42, 0x61, 0x51, 0x49, 0x46, // 2
    0x21, 0x41, 0x45, 0x4B, 0x31, // 3
    0x18, 0x14, 0x12, 0x7F, 0x10, // 4
    0x27, 0x45, 0x45, 0x45, 0x39, // 5
    0x3C, 0x4A, 0x49, 0x49, 0x30, // 6
    0x01, 0x71, 0x09, 0x05, 0x03, // 7
    0x36, 0x49, 0x49, 0x49, 0x36, // 8
    0x06, 0x49, 0x49, 0x29, 0x1E, // 9
    0x00, 0x36, 0x36, 0x00, 0x00, // :
    0x00, 0x56, 0x36, 0x00, 0x00, // ;
    0x00, 0x08, 0x14, 0x22, 0x41, // <
    0x14, 0x14, 0x14, 0x14, 0x14, // =
    0x41, 0x22, 0x14, 0x08, 0x00, // >
    0x02, 0x01, 0x51, 0x09, 0x06, // ?
    0x32, 0x49, 0x79, 0x41, 0x3E, // @
    0x7E, 0x11, 0x11, 0x11, 0x7E, // A
    0x7F, 0x49, 0x49, 0x49, 0x36, // B
    0x3E, 0x41, 0x41, 0x41, 0x22, // C
    0x7F, 0x41, 0x41, 0x22, 0x1C, // D
    0x7F, 0x49, 0x49, 0x49, 0x41, // E
    0x7F, 0x09, 0x09, 0x01, 0x01, // F
    0x3E, 0x41, 0x41, 0x51, 0x32, // G
    0x7F, 0x08, 0x08, 0x08, 0x7F, // H
    0x00, 0x41, 0x7F, 0x41, 0x00, // I
    0x20, 0x40, 0x41, 0x3F, 0x01, // J
    0x7F, 0x08, 0x14, 0x22, 0x41, // K
    0x7F, 0x40, 0x40, 0x40, 0x40, // L
    0x7F, 0x02, 0x04, 0x02, 0x7F, // M
    0x7F, 0x04, 0x08, 0x10, 0x7F, // N
    0x3E, 0x41, 0x41, 0x41, 0x3E, // O
    0x7F, 0x09, 0x09, 0x09, 0x06, // P
    0x3E, 0x41, 0x51, 0x21, 0x5E, // Q
    0x7F, 0x09, 0x19, 0x29, 0x46, // R
    0x46, 0x49, 0x49, 0x49, 0x31, // S
    0x01, 0x01, 0x7F, 0x01, 0x01, // T
    0x3F, 0x40, 0x40, 0x40, 0x3F, // U
    0x1F, 0x20, 0x40, 0x20, 0x1F, // V
    0x7F, 0x20, 0x18, 0x20, 0x7F, // W
    0x63, 0x14, 0x08, 0x14, 0x63, // X
    0x03, 0x04, 0x78, 0x04, 0x03, // Y
    0x61, 0x51, 0x49, 0x45, 0x43, // Z
    0x00, 0x00, 0x7F, 0x41, 0x41, // [
    0x02, 0x04, 0x08, 0x10, 0x20, // "\"
    0x41, 0x41, 0x7F, 0x00, 0x00, // ]
    0x04, 0x02, 0x01, 0x02, 0x04, // ^
    0x40, 0x40, 0x40, 0x40, 0x40, // _
    0x00, 0x01, 0x02, 0x04, 0x00, // `
    0x20, 0x54, 0x54, 0x54, 0x78, // a
    0x7F, 0x48, 0x44, 0x44, 0x38, // b
    0x38, 0x44, 0x44, 0x44, 0x20, // c
    0x38, 0x44, 0x44, 0x48, 0x7F, // d
    0x38, 0x54, 0x54, 0x54, 0x18, // e
    0x08, 0x7E, 0x09, 0x01, 0x02, // f
    0x08, 0x14, 0x54, 0x54, 0x3C, // g
    0x7F, 0x08, 0x04, 0x04, 0x78, // h
    0x00, 0x44, 0x7D, 0x40, 0x00, // i
    0x20, 0x40, 0x44, 0x3D, 0x00, // j
    0x00, 0x7F, 0x10, 0x28, 0x44, // k
    0x00, 0x41, 0x7F, 0x40, 0x00, // l
    0x7C, 0x04, 0x18, 0x04, 0x78, // m
    0x7C, 0x08, 0x04, 0x04, 0x78, // n
    0x38, 0x44, 0x44, 0x44, 0x38, // o
    0x7C, 0x14, 0x14, 0x14, 0x08, // p
    0x08, 0x14, 0x14, 0x18, 0x7C, // q
    0x7C, 0x08, 0x04, 0x04, 0x08, // r
    0x48, 0x54, 0x54, 0x54, 0x20, // s
    0x04, 0x3F, 0x44, 0x40, 0x20, // t
    0x3C, 0x40, 0x40, 0x20, 0x7C, // u
    0x1C, 0x20, 0x40, 0x20, 0x1C, // v
    0x3C, 0x40, 0x30, 0x40, 0x3C, // w
    0x44, 0x28, 0x10, 0x28, 0x44, // x
    0x0C, 0x50, 0x50, 0x50, 0x3C, // y
    0x44, 0x64, 0x54, 0x4C, 0x44, // z
    0x00, 0x08, 0x36, 0x41, 0x00, // {
    0x00, 0x00, 0x7F, 0x00, 0x00, // |
    0x00, 0x41, 0x36, 0x08, 0x00, // }
    0x08, 0x08, 0x2A, 0x1C, 0x08, // ->
    0x08, 0x1C, 0x2A, 0x08, 0x08  // <-
};

// These are the sizes of the individual character arrays
#define CHAR_ARR__29x24 696
#define CHAR_ARR__10x14 168
unsigned char *ascii_characters_BIG[128];   // Store the ASCII character set, but can have some elements blank
unsigned char *ascii_characters_SMALL[128]; // Store the ASCII character set, but can have some eleunsigned char *c2[128];
unsigned char *numbers_BIG[10];             // For quicker number display routines, these arrays of pointers to the numbers
unsigned char *numbers_small[10];
// 'global' variables to store screen info and take the frame buffer.
int fbfd = 0;
char *fbp = 0;
struct fb_var_screeninfo vinfo;
struct fb_fix_screeninfo finfo;
int page_size = 0;
int cur_page = 0;
// helper function to 'plot' a pixel in given color
// This is the heart of most of the drawing routines except where memory copy or move is used.
void put_pixel(int x, int y, int c)
{
    x = 128 - x;
    y = 64 - y;
    // calculate the pixel's byte offset inside the buffer
    unsigned int pix_offset = x + y * 128;
    // offset by the current buffer start
    int aa = pix_offset / 8;
    char *p = *((char *)(fbp) + aa);
    // now this is about the same as 'fbp[pix_offset] = value'
    char f;
    char as = pix_offset % 8;
    if (!!c)
    {
        f = 0b10000000;
        *((char *)(fbp) + aa) = ((char)p | (f >> as));
    }
    else
    {
        f = 0b10000000;
        *((char *)(fbp) + aa) = ((char)p & ~(f >> as));
    }
}

void Txt_WriteChar(uint8_t x, uint8_t y, uint8_t Inv, char charToWrite)
{
    char i;
    charToWrite -= 32;
    for (i = 0; i < 5; i++)
    {
        uint8_t Data = *((char *)(font5x8 + (5 * charToWrite) + i));
        if (Inv)
            Data = ~Data;
        GBGWrite(x + i, y, 8, Data);
    }
    if (Inv)
        GBGWrite(x + 5, y, 8, 0xFF);
    else
        GBGWrite(x + 5, y, 8, 0);
}

void Txt_WriteString(uint8_t x, uint8_t y, uint8_t Inv, char *stringToWrite)
{

    while (*stringToWrite)
    {
        Txt_WriteChar(x, y, Inv, *stringToWrite++);
        if (x >= 120)
        {
            x = 1;
            y += 8;
        }
        else
        {
            x += 6;
        }
    }
}

void GBGWrite(char x, char y, char _bit, char data)
{
    char j;

    for (j = 0; j < _bit; j++)
        if ((data) & (1 << j))
            put_pixel(x, y + j, 1);
        else
            put_pixel(x, y + j, 0);
}

// helper function to draw a rectangle in given color
void fill_rect(int x, int y, int w, int h, int c)
{
    int cx, cy;
    for (cy = 0; cy < h; cy++)
    {
        for (cx = 0; cx < w; cx++)
        {
            put_pixel(x + cx, y + cy, c);
        } // for
    }
}
// This verson creates rectangles with border widths.
void draw_rect(unsigned short x, unsigned short y, unsigned short w, unsigned short h, unsigned short c, unsigned short b)
{
    unsigned short cx, cy;
    for (cy = 0; cy < h; cy++)
    {
        for (cx = 0; cx < w; cx++)
        {
            if (((cx < b) || ((w - cx) <= b)) ||
                ((cy < b) || ((h - cy) <= b)))
            {
                put_pixel(x + cx, y + cy, c);
            } // if
        }
    }
}
// Special thanks to:
// https://www.thecrazyprogrammer.com/2017/01/bresenhams-line-drawing-algorithm-c-c.html
void drawline(int x0, int y0, int x1, int y1, int c)
{
    int dx, dy, p, x, y;
    dx = x1 - x0;
    dy = y1 - y0;
    x = x0;
    y = y0;
    p = 2 * dy - dx;
    while (x < x1)
    {
        if (p >= 0)
        {
            put_pixel(x, y, c);
            y = y + 1;
            p = p + 2 * dy - 2 * dx;
        }
        else
        {
            put_pixel(x, y, c);
            p = p + 2 * dy;
        } // if
        x = x + 1;
    }
}
// This function assigns color directly to the elements of the glyph
// array for faster use with memcpy later instead of 1's and 0s checking if desired.
void setColor(unsigned char *a, unsigned short h, unsigned short w, unsigned short c)
{
    unsigned int cx, cy;
    for (cy = 0; cy < h; cy++)
    {
        for (cx = 0; cx < w; cx++)
        {
            if (a[cy * w + cx] > 0)
            { // if the array has a 1 in this element...
                a[cy * w + cx] = c;
            }
            else
            {
                a[cy * w + cx] = 0;
            } // if
        }     // for
    }         // for
}
// Draw an individual character by the array with a color value
void draw_char(char *glyph, unsigned short x, unsigned short y, unsigned short w, unsigned short h, unsigned short c)
{
    unsigned short cx, cy;
    for (cy = 0; cy < h; cy++)
    {
        for (cx = 0; cx < w; cx++)
        {
            if (glyph[cy * w + cx] > 0)
            { // if the array has a 1 in this element...
                //			printf("%i,", glyph[cy*w + cx]); //deb10021
                put_pixel(x + cx, y + cy, c);
            }
            else
            {
                put_pixel(x + cx, y + cy, 0);
            } // if
        }     // for
    }         // for
}
// draw an individual character by the array with a color value AND a background color value for "highlighted" text
// this is essentially just using a background color value and an "else" branch for the 0s. Good for highlighing.
void draw_charBG(char *glyph, unsigned short x, unsigned short y, unsigned short w, unsigned short h, unsigned short c, unsigned short cb)
{
    unsigned short cx, cy;
    for (cy = 0; cy < h; cy++)
    {
        for (cx = 0; cx < w; cx++)
        {
            if (glyph[cy * w + cx] > 0)
            { // if the array has a 1 in this element...
                put_pixel(x + cx, y + cy, c);
            }
            else
            {
                put_pixel(x + cx, y + cy, cb);
            } // if
        }     // for
    }         // for
}
// This version assumes that the color value is already assigned into the element of the array.
// Thus a color value is not given, because it's already there.
// This routine is NOT calling the put_pixel routine. Whatever is in the "line" goes direct.
// Note: the placement assumes that the X,Y values are not going to be less than 1,1, thus
// the most upper left of the screen is 1,1, and positive Y is "downward". Do not call this
// routine with X.Y = 0,0
// Note: on the test system this method had strange side effects and there was  no noticable
// performance improvement. Much may depend on how the system implements memcpy.
void draw_charAR(char *glyph, unsigned short x, unsigned short y, unsigned short w, unsigned short h)
{
    for (unsigned short cy = 0; cy < h; cy++)
    {
        memcpy(&fbp[((y - 1 + cy) * vinfo.xres) + x], &glyph[cy * w], w * sizeof(char));
    } // for
}
// This call to draw string takes X and Y location and color, and spacing between the characters=
// Assumably a null char* array is never handed to this function
// Due to the nature of these strings a length is needed
// x = location x0
// y = location y
// s is the string
// l is the length of the string
// h is the height of the character
// w is the width
// c is the color
// cb is background color
// sz is the size of the character. Use this variable to add sizes later if needed
// spacing is the distance between the characters
//  Generally everything is known "up front" before the function call so there is no checking in here except for null pointers in the character array
// in case it's forgotten that the particular ASCII representation was not appointed with an array comprising a character.
// Depending on which values are given to the variables, this function may call on several versions of character draw routines.
void draw_string(unsigned short x, unsigned short y, char *s, unsigned short l, unsigned short c, unsigned short cb, unsigned short sp, unsigned char sz)
{
    unsigned short incr = 0;
    unsigned short lh, lw;
    x = x - sp;                             // the loop will add it back for the first character.
    unsigned char **ascii_characters_local; // handle to the array that is going to be used based on the sz variable
    switch (sz)
    {
    case 1:
        ascii_characters_local = ascii_characters_SMALL;
        lh = 14;
        lw = 10;
        break;
    case 2:
        ascii_characters_local = ascii_characters_BIG;
        lh = 29;
        lw = 24;
        break;
    default:
        ascii_characters_local = ascii_characters_SMALL;
        lh = 14;
        lw = 10;
        break; // redundant
    }          // switch
    for (unsigned short incr = 0; incr < l; incr++)
    { // loop length of "string" array of characters.
        x += sp;
        if ((unsigned short)s[incr] == 32)
        {
            x += lw;
            continue;
        }
        if (c > 0)
        {                                                                                                                // Color is given to check if only a fore color was given, or a fore and back
            (cb > 0) ? draw_charBG(ascii_characters_local[(unsigned short)s[incr]], x + (incr * lw), y, lw, lh, c, cb) : // Background color
                draw_char(ascii_characters_local[(unsigned short)s[incr]], x + (incr * lw), y, lw, lh, c);               // No background color
        }
        else
        {                                                                                             // Color is in the glyph array itself
            draw_charAR(ascii_characters_local[(unsigned short)s[incr]], x + (incr * lw), y, lw, lh); // Array "blit" with mem copy
        }                                                                                             // if
    }                                                                                                 // for
}
// this is a quick decimal display function that calls on draw_char. Generally, decimals are characters, but this calls from the
// secondary pointer array that points to characters representing numbers wereby the index is the proper number.
// this function is recursive
void decdisp(unsigned char **aloc, unsigned short x, unsigned short *y, unsigned short *h, unsigned short *w, unsigned short *offs, unsigned short *c, unsigned short *bg, unsigned int divider, unsigned int value)
{
    if (divider == 1)
        return;
    if ((value % divider) == value)
    {
        (bg > 0) ? draw_charBG(aloc[value / (divider / 10)], x, *y, *w, *h, *c, *bg) : // Background color
            draw_char(aloc[value / (divider / 10)], x, *y, *w, *h, *c);                // No background color
        x += *w;
        x += *offs;
        decdisp(aloc, x, y, h, w, offs, c, bg, (divider / 10), value % (divider / 10));
    } // if
}
// this function calls the recursive decdisp function for the display of numbers in a more easier format where
// the numbers are delivered as actual numbers and not characters.
void draw_numbers(unsigned short x, unsigned short y, unsigned short offs, unsigned short c, unsigned short cb, unsigned short sz, unsigned int divider, unsigned int value)
{
    unsigned char **ascii_characters_local; // handle to the array that is going to be used based on the sz variable
    unsigned short lh, lw;
    switch (sz)
    { // height and width are established based on that size variable, almost like a "font size"
    case 1:
        ascii_characters_local = numbers_small;
        lh = 14;
        lw = 10;
        break;
    case 2:
        ascii_characters_local = numbers_BIG;
        lh = 29;
        lw = 24;
        break;
    default:
        ascii_characters_local = numbers_small;
        lh = 14;
        lw = 10;
        break;                                                                    // redundant
    }                                                                             // switch
    unsigned short *yval, *heightval, *widthval, *offsetval, *colorval, *bgcolor; // pointers for the recursive call
    yval = &y;
    heightval = &lh;
    widthval = &lw;
    offsetval = &offs;
    colorval = &c;
    bgcolor = &cb;
    decdisp(ascii_characters_local, x, yval, heightval, widthval, offsetval, colorval, bgcolor, divider, value);
}
// helper to clear (fill with given color) the screen
void clear_screen(int c)
{
    memset(fbp, 0, page_size);
    // memset(fbp , 0xFF,1024);
}
// application entry point
int main(int argc, char *argv[])
{

    struct fb_var_screeninfo orig_vinfo;
    long int screensize = 0;

    // Open the framebuffer file for reading and writing
    fbfd = open("/dev/fb1", O_RDWR);
    if (fbfd == -1)
    {
        printf("Error: cannot open framebuffer device.\n");
        return (1);
    }

    // Get variable screen information
    if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo))
    {
        printf("Error reading variable information.\n");
    }

    // Store for reset (copy vinfo to vinfo_orig)
    memcpy(&orig_vinfo, &vinfo, sizeof(struct fb_var_screeninfo));
    // Change variable info
    vinfo.bits_per_pixel = 8;
    // Can change res here, or leave what was found originally
    // vinfo.xres = 1184;
    // vinfo.yres = 624;
    vinfo.xres_virtual = vinfo.xres;
    vinfo.yres_virtual = vinfo.yres * 2;
    if (ioctl(fbfd, FBIOPUT_VSCREENINFO, &vinfo))
    {
        printf("Error setting variable information.\n");
    }

    // Get fixed screen information
    if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo))
    {
        printf("Error reading fixed information.\n");
    }

    page_size = 8192;

    // map fb to user mem
    screensize = finfo.smem_len;
    fbp = (char *)mmap(0,
                       screensize,
                       PROT_READ | PROT_WRITE,
                       MAP_SHARED,
                       fbfd,
                       0);
    if ((int)fbp == -1)
    {
        printf("Failed to mmap\n");
    }
    else
    {

        if (strcmp(argv[1], "init") == 0)
        {
            init_big_font();
            clear_screen(0);
            draw_string(37, 2, (char *)"MC60", 4, 1, 0, 2, 1);
            draw_string(5, 20, (char *)"PROGRAMMER", 10, 1, 0, 2, 1);
            Txt_WriteString(20, 45, 0, "click to start");
        }
        else if (strcmp(argv[1], "waiting") == 0)
        {
            init_big_font();
            clear_screen(0);
            draw_string(20, 25, (char *)"WAITING", 7, 1, 0, 2, 1);
        }
        else if (strcmp(argv[1], "wait") == 0)
        {
            drawline(0, 50, 110, 0, 0);
            drawline(10, 50, 10 + 10, 0, 2);
            drawline(30, 50, 30 + 10, 0, 2);
            drawline(50, 50, 50 + 10, 0, 2);
            drawline(70, 50, 70 + 10, 0, 2);
            drawline(90, 50, 90 + 10, 0, 2);
            drawline(110, 50, 110 + 10, 0, 2);
            usleep(200000);
            drawline(0, 50, 120, 0, 0);
            drawline(20, 50, 20 + 10, 0, 2);
            drawline(40, 50, 40 + 10, 0, 2);
            drawline(60, 50, 60 + 10, 0, 2);
            drawline(80, 50, 80 + 10, 0, 2);
            drawline(100, 50, 100 + 10, 0, 2);
        }
        else if (strcmp(argv[1], "timeout") == 0)
        {
            init_big_font();
            clear_screen(0);
            draw_string(20, 25, (char *)"TIMEOUT", 7, 1, 0, 2, 1);
        }
        else if (strcmp(argv[1], "downloading") == 0)
        {
            init_big_font();
            clear_screen(0);
            draw_string(5, 5, (char *)"DOWNLOADING", 10, 1, 0, 2, 1);
        }

        else if (strcmp(argv[1], "finish") == 0)
        {
            init_big_font();
            clear_screen(0);
            draw_string(15, 25, (char *)"FINISHED", 8, 1, 0, 2, 1);
        }
        else if (strcmp(argv[1], "clear") == 0)
        {
            clear_screen(0);
        }

        else if (strcmp(argv[1], "error") == 0)
        {
            init_big_font();
            clear_screen(0);
            draw_string(32, 25, (char *)"ERROR", 5, 1, 0, 2, 1);

            char num[50];
            FILE *fptr;

            if ((fptr = fopen("error.txt", "r")) == NULL)
            {
                printf("Error! opening file");
                exit(1);
            }
            char line[80] = {0};
            fgets(line, 80, fptr);
            fclose(fptr);
            Txt_WriteString(1, 45, 0, line);
        }

        else if (strcmp(argv[1], "prog") == 0)
        {

            int line = atoi(argv[2]);
            int prog = atoi(argv[3]);

            int y;

            switch (line)
            {
            case 1:
                draw_rect(12, 23, 104, 9, 9, 1);
                y = 25;
                break;
            case 2:
                draw_rect(12, 34, 104, 9, 9, 1);
                y = 36;
                break;
            case 3:
                draw_rect(12, 45, 104, 9, 9, 1);
                y = 47;
                break;
            default:
                break;
            }

            fill_rect(14, y, prog, 5, 1);
        }
    }

    // unmap fb file from memory
    munmap(fbp, screensize);

    // reset the display mode
    if (ioctl(fbfd, FBIOPUT_VSCREENINFO, &orig_vinfo))
    {
        printf("Error re-setting variable information.\n");
    }
    // close fb file
    close(fbfd);

    return 0;
}

void init_big_font()
{
    // The actual glyphs here. Discard that which is not used to save memory
    {
        unsigned char A__10x14[CHAR_ARR__10x14] = {
            0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
            0, 0, 0, 1, 1, 1, 1, 0, 0, 0,
            0, 0, 1, 1, 1, 1, 1, 1, 0, 0,
            0, 1, 1, 1, 0, 0, 1, 1, 1, 0,
            1, 1, 1, 0, 0, 0, 0, 1, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1};

        unsigned char B__10x14[CHAR_ARR__10x14] = {
            1, 1, 1, 1, 1, 1, 1, 0, 0, 0,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
            1, 1, 0, 0, 0, 0, 0, 1, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
            1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
            1, 1, 0, 0, 0, 0, 0, 1, 1, 0,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 1, 1, 0,
            1, 1, 1, 1, 1, 1, 1, 0, 0, 0};

        unsigned char C__10x14[CHAR_ARR__10x14] = {
            0, 0, 0, 1, 1, 1, 1, 1, 0, 0,
            0, 0, 1, 1, 1, 1, 1, 1, 0, 0,
            1, 1, 1, 0, 0, 0, 0, 1, 1, 0,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 1, 0, 0, 0, 0, 1, 1, 0,
            0, 0, 1, 1, 1, 1, 1, 1, 0, 0,
            0, 0, 0, 1, 1, 1, 1, 1, 0, 0};

        unsigned char D__10x14[CHAR_ARR__10x14] = {
            1, 1, 1, 1, 1, 1, 1, 0, 0, 0,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
            1, 1, 0, 0, 0, 0, 0, 1, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 0, 0, 0};

        unsigned char E__10x14[CHAR_ARR__10x14] = {
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
            1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
            1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

        unsigned char F__10x14[CHAR_ARR__10x14] = {
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
            1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
            1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 0, 0, 0, 0, 0, 0, 0, 0};

        unsigned char G__10x14[CHAR_ARR__10x14] = {
            0, 0, 0, 1, 1, 1, 1, 1, 0, 0,
            0, 0, 1, 1, 1, 1, 1, 1, 0, 0,
            1, 1, 1, 0, 0, 0, 0, 1, 1, 0,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 0, 0, 0, 1, 1, 1, 1, 1,
            1, 1, 0, 0, 0, 1, 1, 1, 1, 1,
            1, 1, 1, 0, 0, 0, 0, 1, 1, 1,
            0, 0, 1, 1, 1, 1, 1, 1, 0, 0,
            0, 0, 0, 1, 1, 1, 1, 1, 0, 0};

        unsigned char H__10x14[CHAR_ARR__10x14] = {
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1};

        unsigned char I__10x14[CHAR_ARR__10x14] = {
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
            0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
            0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
            0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
            0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
            0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
            0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
            0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
            0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
            0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

        unsigned char J__10x14[CHAR_ARR__10x14] = {
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
            0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
            0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
            0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
            0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
            0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
            0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
            0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
            1, 1, 0, 0, 1, 1, 0, 0, 0, 0,
            1, 1, 0, 0, 1, 1, 0, 0, 0, 0,
            0, 1, 1, 1, 1, 1, 0, 0, 0, 0,
            0, 0, 1, 1, 1, 0, 0, 0, 0, 0};

        unsigned char K__10x14[CHAR_ARR__10x14] = {
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 1, 1, 0,
            1, 1, 0, 0, 0, 0, 1, 1, 0, 0,
            1, 1, 0, 0, 0, 1, 1, 0, 0, 0,
            1, 1, 0, 0, 1, 1, 0, 0, 0, 0,
            1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
            1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
            1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
            1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
            1, 1, 0, 1, 1, 0, 0, 0, 0, 0,
            1, 1, 0, 0, 1, 1, 0, 0, 0, 0,
            1, 1, 0, 0, 0, 0, 1, 1, 0, 0,
            1, 1, 0, 0, 0, 0, 0, 1, 1, 0,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1};

        unsigned char L__10x14[CHAR_ARR__10x14] = {
            1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

        unsigned char M__10x14[CHAR_ARR__10x14] = {
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 1, 0, 0, 0, 0, 1, 1, 1,
            1, 1, 1, 1, 0, 0, 1, 1, 1, 1,
            1, 1, 0, 1, 1, 1, 1, 0, 1, 1,
            1, 1, 0, 0, 1, 1, 0, 0, 1, 1,
            1, 1, 0, 0, 1, 1, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            0, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1};

        unsigned char N__10x14[CHAR_ARR__10x14] = {
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 1, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 1, 1, 0, 0, 0, 0, 1, 1,
            1, 1, 1, 1, 1, 0, 0, 0, 1, 1,
            1, 1, 0, 1, 1, 1, 0, 0, 1, 1,
            1, 1, 0, 0, 1, 1, 1, 0, 1, 1,
            1, 1, 0, 0, 0, 1, 1, 1, 1, 1,
            1, 1, 0, 0, 0, 0, 1, 1, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 1, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1};

        unsigned char O__10x14[CHAR_ARR__10x14] = {
            0, 0, 1, 1, 1, 1, 1, 1, 0, 0,
            0, 1, 1, 1, 1, 1, 1, 1, 1, 0,
            1, 1, 1, 0, 0, 0, 0, 1, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 1, 0, 0, 0, 0, 1, 1, 1,
            0, 1, 1, 1, 1, 1, 1, 1, 1, 0,
            0, 0, 1, 1, 1, 1, 1, 1, 0, 0};

        unsigned char P__10x14[CHAR_ARR__10x14] = {
            1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
            1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
            1, 1, 0, 0, 0, 0, 0, 1, 1, 0,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 1, 1, 0,
            1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
            1, 1, 1, 1, 1, 1, 1, 0, 0, 0,
            1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 0, 0, 0, 0, 0, 0, 0, 0};

        unsigned char Q__10x14[CHAR_ARR__10x14] = {
            0, 0, 1, 1, 1, 1, 1, 1, 0, 0,
            0, 1, 1, 1, 1, 1, 1, 1, 1, 0,
            1, 1, 1, 0, 0, 0, 0, 1, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 1, 1, 1, 0, 1, 1,
            1, 1, 0, 0, 0, 1, 1, 1, 1, 1,
            1, 1, 1, 0, 0, 0, 1, 1, 1, 1,
            0, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            0, 0, 1, 1, 1, 1, 1, 1, 1, 1};

        unsigned char R__10x14[CHAR_ARR__10x14] = {
            1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
            1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
            1, 1, 0, 0, 0, 0, 0, 1, 1, 0,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 1, 1, 0,
            1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
            1, 1, 1, 1, 1, 1, 1, 0, 0, 0,
            1, 1, 0, 1, 1, 1, 0, 0, 0, 0,
            1, 1, 0, 0, 0, 1, 1, 0, 0, 0,
            1, 1, 0, 0, 0, 0, 1, 1, 0, 0,
            1, 1, 0, 0, 0, 0, 0, 1, 1, 0,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1};

        unsigned char S__10x14[CHAR_ARR__10x14] = {
            0, 0, 0, 1, 1, 1, 1, 0, 0, 0,
            0, 0, 1, 1, 1, 1, 1, 1, 0, 0,
            0, 1, 1, 0, 0, 0, 1, 1, 1, 0,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
            0, 1, 1, 1, 1, 1, 1, 0, 0, 0,
            0, 0, 1, 1, 1, 1, 1, 1, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 1, 1, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 1, 0, 0, 0, 0, 1, 1, 0,
            0, 0, 1, 1, 1, 1, 1, 1, 0, 0,
            0, 0, 0, 1, 1, 1, 1, 0, 0, 0};

        unsigned char T__10x14[CHAR_ARR__10x14] = {
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
            0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
            0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
            0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
            0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
            0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
            0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
            0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
            0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
            0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
            0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
            0, 0, 0, 0, 1, 1, 0, 0, 0, 0};

        unsigned char U__10x14[CHAR_ARR__10x14] = {
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            0, 1, 1, 0, 0, 0, 0, 1, 1, 0,
            0, 0, 1, 1, 1, 1, 1, 1, 0, 0,
            0, 0, 0, 1, 1, 1, 1, 0, 0, 0};

        unsigned char V__10x14[CHAR_ARR__10x14] = {
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            0, 1, 1, 0, 0, 0, 0, 1, 1, 0,
            0, 1, 1, 0, 0, 0, 0, 1, 1, 0,
            0, 0, 1, 1, 0, 0, 1, 1, 0, 0,
            0, 0, 0, 1, 1, 1, 1, 0, 0, 0,
            0, 0, 0, 0, 1, 1, 0, 0, 0, 0};

        unsigned char W__10x14[CHAR_ARR__10x14] = {
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 1, 1, 0, 0, 1, 1,
            1, 1, 0, 1, 1, 1, 1, 0, 1, 1,
            1, 1, 0, 1, 1, 1, 1, 0, 1, 1,
            1, 1, 1, 1, 0, 0, 1, 1, 1, 1,
            1, 1, 1, 0, 0, 0, 1, 1, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1};

        unsigned char X__10x14[CHAR_ARR__10x14] = {
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 1, 0, 0, 0, 0, 1, 1, 1,
            0, 1, 1, 1, 0, 0, 1, 1, 1, 0,
            0, 0, 1, 1, 0, 0, 1, 1, 0, 0,
            0, 0, 0, 1, 1, 1, 1, 0, 0, 0,
            0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
            0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
            0, 0, 0, 1, 1, 1, 1, 0, 0, 0,
            0, 0, 1, 1, 0, 0, 1, 1, 0, 0,
            0, 1, 1, 1, 0, 0, 1, 1, 1, 1,
            1, 1, 1, 0, 0, 0, 0, 1, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1};

        unsigned char Y__10x14[CHAR_ARR__10x14] = {
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 1, 0, 0, 0, 0, 1, 1, 1,
            0, 1, 1, 1, 0, 0, 1, 1, 1, 0,
            0, 0, 1, 1, 0, 0, 1, 1, 0, 0,
            0, 0, 0, 1, 1, 1, 1, 0, 0, 0,
            0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
            0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
            0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
            0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
            0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
            0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
            0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
            0, 0, 0, 0, 1, 1, 0, 0, 0, 0};

        unsigned char Z__10x14[CHAR_ARR__10x14] = {
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 1, 1, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 1, 1, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 1, 1, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 1, 1, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 1, 1, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 1, 1, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

        unsigned char AR1__10x14[CHAR_ARR__10x14] = {
            0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
            0, 0, 0, 1, 1, 1, 0, 0, 0, 0,
            0, 0, 1, 1, 1, 1, 0, 0, 0, 0,
            0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
            0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
            0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
            0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
            0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
            0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
            0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
            0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
            0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
            0, 0, 1, 1, 1, 1, 1, 1, 0, 0,
            0, 0, 1, 1, 1, 1, 1, 1, 0, 0};

        unsigned char AR2__10x14[CHAR_ARR__10x14] = {
            0, 0, 0, 1, 1, 1, 1, 0, 0, 0,
            0, 1, 1, 1, 1, 1, 1, 1, 0, 0,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
            0, 0, 0, 0, 0, 0, 0, 1, 1, 0,
            0, 0, 0, 0, 0, 0, 1, 1, 0, 0,
            0, 0, 0, 0, 0, 1, 1, 0, 0, 0,
            0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
            0, 0, 0, 1, 1, 0, 0, 0, 0, 0,
            0, 0, 1, 1, 0, 0, 0, 0, 0, 0,
            0, 1, 1, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

        unsigned char AR3__10x14[CHAR_ARR__10x14] = {
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
            0, 0, 0, 0, 0, 0, 0, 1, 1, 0,
            0, 0, 0, 0, 0, 0, 1, 1, 0, 0,
            0, 0, 0, 0, 0, 1, 1, 0, 0, 0,
            0, 0, 0, 0, 1, 1, 1, 1, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
            0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
            0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
            0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 1, 1, 0,
            1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
            0, 1, 1, 1, 1, 1, 1, 0, 0, 0};

        unsigned char AR4__10x14[CHAR_ARR__10x14] = {
            0, 0, 0, 0, 0, 0, 1, 1, 0, 0,
            0, 0, 0, 0, 0, 1, 1, 1, 0, 0,
            0, 0, 0, 0, 1, 1, 1, 1, 0, 0,
            0, 0, 0, 1, 1, 0, 1, 1, 0, 0,
            0, 0, 1, 1, 0, 0, 1, 1, 0, 0,
            0, 1, 1, 0, 0, 0, 1, 1, 0, 0,
            1, 1, 0, 0, 0, 0, 1, 1, 0, 0,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            0, 0, 0, 0, 0, 0, 1, 1, 0, 0,
            0, 0, 0, 0, 0, 0, 1, 1, 0, 0,
            0, 0, 0, 0, 0, 0, 1, 1, 0, 0,
            0, 0, 0, 0, 0, 0, 1, 1, 0, 0,
            0, 0, 0, 0, 0, 0, 1, 1, 0, 0};

        unsigned char AR5__10x14[CHAR_ARR__10x14] = {
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 1, 1, 1, 1, 1, 0, 0, 0,
            1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 1, 1, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
            0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 1, 1, 0,
            0, 1, 1, 1, 1, 1, 1, 1, 0, 0,
            0, 0, 1, 1, 1, 1, 1, 0, 0, 0};

        unsigned char AR6__10x14[CHAR_ARR__10x14] = {
            0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
            0, 0, 0, 0, 1, 1, 1, 1, 1, 1,
            0, 0, 0, 1, 1, 0, 0, 0, 0, 0,
            0, 0, 1, 1, 0, 0, 0, 0, 0, 0,
            0, 1, 1, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            0, 1, 1, 1, 1, 1, 1, 1, 1, 0,
            0, 0, 1, 1, 1, 1, 1, 1, 0, 0};

        unsigned char AR7__10x14[CHAR_ARR__10x14] = {
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
            0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
            0, 0, 0, 0, 0, 0, 0, 1, 1, 0,
            0, 0, 0, 0, 0, 0, 1, 1, 0, 0,
            0, 0, 0, 0, 0, 1, 1, 0, 0, 0,
            0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
            0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
            0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
            0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
            0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
            0, 0, 0, 0, 1, 1, 0, 0, 0, 0,
            0, 0, 0, 0, 1, 1, 0, 0, 0, 0};

        unsigned char AR8__10x14[CHAR_ARR__10x14] = {
            0, 0, 1, 1, 1, 1, 1, 1, 0, 0,
            0, 1, 1, 1, 1, 1, 1, 1, 1, 0,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            0, 1, 1, 1, 1, 1, 1, 1, 1, 0,
            0, 0, 1, 1, 1, 1, 1, 1, 0, 0,
            0, 1, 1, 1, 1, 1, 1, 1, 1, 0,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            0, 1, 1, 1, 1, 1, 1, 1, 1, 0,
            0, 0, 1, 1, 1, 1, 1, 1, 0, 0};

        unsigned char AR9__10x14[CHAR_ARR__10x14] = {
            0, 0, 1, 1, 1, 1, 1, 1, 0, 0,
            0, 1, 1, 1, 1, 1, 1, 1, 1, 0,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 0, 1, 1,
            0, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            0, 0, 1, 1, 1, 1, 1, 1, 1, 1,
            0, 0, 0, 0, 0, 0, 0, 1, 1, 1,
            0, 0, 0, 0, 0, 0, 0, 1, 1, 1,
            0, 0, 0, 0, 0, 0, 1, 1, 1, 0,
            0, 0, 0, 0, 0, 1, 1, 1, 0, 0,
            0, 0, 0, 0, 1, 1, 1, 0, 0, 0,
            1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
            1, 1, 1, 1, 1, 0, 0, 0, 0, 0};

        unsigned char AR0__10x14[CHAR_ARR__10x14] = {
            0, 0, 1, 1, 1, 1, 1, 1, 0, 0,
            0, 1, 1, 1, 1, 1, 1, 1, 1, 0,
            1, 1, 0, 0, 0, 0, 1, 1, 1, 1,
            1, 1, 0, 0, 0, 0, 1, 1, 1, 1,
            1, 1, 0, 0, 0, 1, 1, 1, 1, 1,
            1, 1, 0, 0, 0, 1, 1, 0, 1, 1,
            1, 1, 0, 0, 1, 1, 0, 0, 1, 1,
            1, 1, 0, 0, 1, 1, 0, 0, 1, 1,
            1, 1, 0, 1, 1, 0, 0, 0, 1, 1,
            1, 1, 1, 1, 1, 0, 0, 0, 1, 1,
            1, 1, 1, 1, 0, 0, 0, 0, 1, 1,
            1, 1, 1, 1, 0, 0, 0, 0, 1, 1,
            0, 1, 1, 1, 1, 1, 1, 1, 1, 0,
            0, 0, 1, 1, 1, 1, 1, 1, 0, 0};

        // specials
        unsigned char COLON__10x14[CHAR_ARR__10x14] = {
            0, 0, 0, 1, 1, 1, 1, 0, 0, 0,
            0, 0, 0, 1, 1, 1, 1, 0, 0, 0,
            0, 0, 0, 1, 1, 1, 1, 0, 0, 0,
            0, 0, 0, 1, 1, 1, 1, 0, 0, 0,
            0, 0, 0, 1, 1, 1, 1, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 1, 1, 1, 1, 0, 0, 0,
            0, 0, 0, 1, 1, 1, 1, 0, 0, 0,
            0, 0, 0, 1, 1, 1, 1, 0, 0, 0,
            0, 0, 0, 1, 1, 1, 1, 0, 0, 0,
            0, 0, 0, 1, 1, 1, 1, 0, 0, 0};

        unsigned char SPACE__10x14[CHAR_ARR__10x14] = {
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

        ascii_characters_SMALL[32] = SPACE__10x14;
        ascii_characters_SMALL[48] = AR0__10x14;
        ascii_characters_SMALL[49] = AR1__10x14;
        ascii_characters_SMALL[50] = AR2__10x14;
        ascii_characters_SMALL[51] = AR3__10x14;
        ascii_characters_SMALL[52] = AR4__10x14;
        ascii_characters_SMALL[53] = AR5__10x14;
        ascii_characters_SMALL[54] = AR6__10x14;
        ascii_characters_SMALL[55] = AR7__10x14;
        ascii_characters_SMALL[56] = AR8__10x14;
        ascii_characters_SMALL[57] = AR9__10x14;
        ascii_characters_SMALL[58] = COLON__10x14;
        ascii_characters_SMALL[65] = A__10x14;
        ascii_characters_SMALL[66] = B__10x14;
        ascii_characters_SMALL[67] = C__10x14;
        ascii_characters_SMALL[68] = D__10x14;
        ascii_characters_SMALL[69] = E__10x14;
        ascii_characters_SMALL[70] = F__10x14;
        ascii_characters_SMALL[71] = G__10x14;
        ascii_characters_SMALL[72] = H__10x14;
        ascii_characters_SMALL[73] = I__10x14;
        ascii_characters_SMALL[74] = J__10x14;
        ascii_characters_SMALL[75] = K__10x14;
        ascii_characters_SMALL[76] = L__10x14;
        ascii_characters_SMALL[77] = M__10x14;
        ascii_characters_SMALL[78] = N__10x14;
        ascii_characters_SMALL[79] = O__10x14;
        ascii_characters_SMALL[80] = P__10x14;
        ascii_characters_SMALL[81] = Q__10x14;
        ascii_characters_SMALL[82] = R__10x14;
        ascii_characters_SMALL[83] = S__10x14;
        ascii_characters_SMALL[84] = T__10x14;
        ascii_characters_SMALL[85] = U__10x14;
        ascii_characters_SMALL[86] = V__10x14;
        ascii_characters_SMALL[87] = W__10x14;
        ascii_characters_SMALL[88] = X__10x14;
        ascii_characters_SMALL[89] = Y__10x14;
        ascii_characters_SMALL[90] = Z__10x14;

        numbers_small[0] = AR0__10x14; // For number displays
        numbers_small[1] = AR1__10x14;
        numbers_small[2] = AR2__10x14;
        numbers_small[3] = AR3__10x14;
        numbers_small[4] = AR4__10x14;
        numbers_small[5] = AR5__10x14;
        numbers_small[6] = AR6__10x14;
        numbers_small[7] = AR7__10x14;
        numbers_small[8] = AR8__10x14;
        numbers_small[9] = AR9__10x14;
    }
}
