extern "C" {

#include <stdint.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include <linux/fb.h>
#include <linux/kd.h>

#include <ev3_lcd.h>

#include "fonts/large_font.xbm"
#define LARGE_FONT_PIXEL(x,y) ((large_font_bits[((x) >> 3) + (y) * 32 ] & (1 << ((x) & 7))) >> ((x) & 7))
#define LARGE_LETTER_WIDTH 16
#define LARGE_LETTER_HEIGHT 16
#include "fonts/normal_font.xbm"
#define NORMAL_FONT_PIXEL(x,y) ((normal_font_bits[((x) >> 3) + (y) * 16 ] & (1 << ((x) & 7))) >> ((x) & 7))
#define NORMAL_LETTER_WIDTH 8
#define NORMAL_LETTER_HEIGHT 9
#include "fonts/small_font.xbm"
#define SMALL_FONT_PIXEL(x,y) ((small_font_bits[((x) >> 3) + (y) * 16 ] & (1 << ((x) & 7))) >> ((x) & 7))
#define SMALL_LETTER_WIDTH 8
#define SMALL_LETTER_HEIGHT 8
#include "fonts/tiny_font.xbm"
#define TINY_FONT_PIXEL(x,y) ((tiny_font_bits[((x) >> 3) + (y) * 16 ] & (1 << ((x) & 7))) >> ((x) & 7))
#define TINY_LETTER_WIDTH 5
#define TINY_FONT_WIDTH 8
#define TINY_LETTER_HEIGHT 7

int initialized = 0;
#define __CHECK_INIT { if ( initialized == 0 && Lcd_init() < 0 ) { return; } }

int16_t frameBufferLength 	= 0,
        x_bufferResolution 	= 0,
        y_bufferResolution 	= 0,
        bufferLineLength 	= 0;

int32_t __fbfd, __vtfd;
unsigned char *__fbp = NULL;

int Lcd_init( void )
{
    __fbfd = open( "/dev/fb0", O_RDWR );

    fb_fix_screeninfo i;

    if ( ioctl( __fbfd, FBIOGET_FSCREENINFO, &i ) < 0 ) {
        close( __fbfd );
        return( -1 );
    }

    fb_var_screeninfo v;

    if ( ioctl( __fbfd, FBIOGET_VSCREENINFO, &v ) < 0 ) {
        close( __fbfd );
        return( -1 );
    }

    frameBufferLength = i.smem_len;
    bufferLineLength = i.line_length;
    x_bufferResolution = v.xres;
    y_bufferResolution = v.yres;

    __fbp = ( unsigned char * )mmap( 0, frameBufferLength, PROT_READ | PROT_WRITE, 
    								 MAP_SHARED, __fbfd, 0 );
    __vtfd = open( "/dev/tty", O_RDONLY );
    ioctl( __vtfd, KDSETMODE, KD_GRAPHICS );
    initialized = 1;
    return( 0 );
}

void Lcd_release( void )
{
    if ( !initialized )
        return;

    munmap( __fbp, frameBufferLength );
    close( __fbfd );
    ioctl( __vtfd, KDSETMODE, KD_TEXT );
    close( __vtfd );
}

void Lcd_clear( void )
{
__CHECK_INIT
    memset( __fbp, 0, frameBufferLength );
}

#define EV3_PIXEL_SET(x,y) __fbp[((x) >> 3) + (y) * bufferLineLength] |= 1 << ((x) & 7)
#define EV3_PIXEL_UNSET(x,y) __fbp[((x) >> 3) + (y) * bufferLineLength] &= 0xFF ^ (1 << ((x) & 7))

#define EV3_PIXEL(x,y,bit) { if (bit) EV3_PIXEL_SET(x,y); else EV3_PIXEL_UNSET(x,y); }

void Lcd_text ( int16_t x, int16_t y, const char *text, Font_size_t size ) 
{
__CHECK_INIT
    int32_t i;

    int32_t letter_width = 0,
            letter_height = 0,
            font_width = 0;

    switch ( size ) {
        case LARGE:
            letter_width = font_width = LARGE_LETTER_WIDTH;
            letter_height = LARGE_LETTER_HEIGHT;
            break;
        
        case NORMAL:
            letter_width = font_width = NORMAL_LETTER_WIDTH;
            letter_height = NORMAL_LETTER_HEIGHT;
            break;

        case SMALL:
            letter_width = font_width = SMALL_LETTER_WIDTH;
            letter_height = SMALL_LETTER_HEIGHT;
            break;

        case TINY:
            letter_width = TINY_LETTER_WIDTH;
            letter_height = TINY_LETTER_HEIGHT;
            font_width = TINY_FONT_WIDTH;
            break;
    }

    for ( i = 0; text[i]; i++ ) {
        if ( text[i] < ' ' || text[i] > 127 ) {
            continue;
        }

        int32_t c_x = ( ( text[i] - ' ' ) % 16 ) * font_width;
        int32_t c_y = ( ( text[i] - ' ' ) / 16 ) * letter_height;
        int32_t a;

        for ( a = 0; a < letter_width; a++ ) {
            int32_t X = x + i * letter_width + a;

            if ( X < 0 ) {
                continue;
            }

            if ( X >= ( int32_t )x_bufferResolution ) {
                return;
            }

            int32_t b;

            for ( b = 0; b < letter_height; b++ ) {
                int32_t Y = y + b;

                if ( Y < 0 ) {
                    continue;
                }

                if ( Y >= ( int32_t )y_bufferResolution ) {
                    break;
                }

                if (    (size == LARGE  && LARGE_FONT_PIXEL( c_x + a,c_y + b ))
                     || (size == NORMAL && NORMAL_FONT_PIXEL( c_x + a,c_y + b ))
                     || (size == SMALL  && SMALL_FONT_PIXEL( c_x + a,c_y + b ))
                     || (size == TINY   && TINY_FONT_PIXEL( c_x + a,c_y + b )) ) {
                    EV3_PIXEL_SET( X,Y );
                }
            }
        }
    }
}

void Lcd_text_dig(int16_t x, int16_t y, const int32_t var, Font_size_t size)
{

    char Buffer[8];

    sprintf(Buffer, "%d", var);

    Lcd_text(x,y,Buffer,size);

}

typedef enum figure_fill_type_ {
    FILL = 0,
    BORDER
}Figure_fill_type_t;

void Lcd_draw_rectangle ( int16_t x, int16_t y, int16_t w, int16_t h, Pixel_color_t color, Figure_fill_type_t type )
{
__CHECK_INIT
    int16_t a,b;
    int16_t minx = x < 0 ? 0 : x;
    int16_t miny = y < 0 ? 0 : y;
    int16_t maxx = (x+w) >= x_bufferResolution ? x_bufferResolution-1 : (x+w);
    int16_t maxy = (y+h) >= y_bufferResolution ? y_bufferResolution-1 : (y+h);

    if (    minx >= x_bufferResolution
         || miny >= y_bufferResolution
         || maxx < 0 
         || maxy < 0 ) {
        return;
    }

    if ( type == FILL ) {

        for ( a = minx; a <= maxx; a++ )
            for ( b = miny; b <= maxy; b++ ) {
                EV3_PIXEL( a, b, color );
            }
    } else if ( type == BORDER ) {
        
        for ( a = minx; a <= maxx; a++ ) {
            EV3_PIXEL( a, miny, color );
            EV3_PIXEL( a, maxy, color );
        }
        for ( a = miny+1; a <= maxy-1; a++ ) {
            EV3_PIXEL( minx, a, color );
            EV3_PIXEL( maxx, a, color );
        }
    } else {
        return;
    }
}

void Lcd_draw_rectangle_filled ( int16_t x, int16_t y, int16_t w, int16_t h, Pixel_color_t color )
{
    Lcd_draw_rectangle( x, y, w, h, color, FILL );
}

void Lcd_draw_rectangle_edged ( int16_t x, int16_t y, int16_t w, int16_t h, Pixel_color_t color )
{
    Lcd_draw_rectangle( x, y, w, h, color, BORDER );
}

#define MIRROR_PIXEL(a,b,color) \
{ \
if (y+b >= 0 && y+b < y_bufferResolution) \
{ \
	if (x+a >= 0 && x+a < x_bufferResolution) \
		EV3_PIXEL(x+a,y+b,color); \
	if (x-a >= 0 && x-a < x_bufferResolution) \
		EV3_PIXEL(x-a,y+b,color); \
} \
if (y-b >= 0 && y-b < y_bufferResolution) \
{ \
	if (x+a >= 0 && x+a < x_bufferResolution) \
		EV3_PIXEL(x+a,y-b,color); \
	if (x-a >= 0 && x-a < x_bufferResolution) \
		EV3_PIXEL(x-a,y-b,color); \
} \
}

void Lcd_draw_ellipse_filled ( int16_t x, int16_t y, int16_t rx, int16_t ry, Pixel_color_t color )
{
__CHECK_INIT
    int32_t a,b;
    int16_t minx = (x-rx) < 0 ? 0 : (x-rx);
    int16_t miny = (y-ry) < 0 ? 0 : (y-ry);
    int16_t maxx = (x+rx) >= x_bufferResolution ? x_bufferResolution-1 : (x+rx);
    int16_t maxy = (y+ry) >= y_bufferResolution ? y_bufferResolution-1 : (y+ry);

    if (    minx >= x_bufferResolution
         || miny >= y_bufferResolution
         || maxx < 0 
         || maxy < 0 ) {
        return;
    }

    for ( a = minx; a <= maxx; a++ )
        for ( b = miny; b <= maxy; b++ ) {
            if ( ( a-x )*( a-x )*ry*ry + ( b-y )*( b-y )*rx*rx >= rx*rx*ry*ry ) {
                continue;
            }
            EV3_PIXEL( a,b,color );
        }
}

void Lcd_draw_ellipse_edged ( int16_t x, int16_t y, int16_t rx, int16_t ry, Pixel_color_t color )
{
__CHECK_INIT
    int32_t a = rx,
            b;

    for ( b = 0; b <= ry; b++ ) {
        int once = 0;

        while ( a*a*ry*ry + b*b*rx*rx >= rx*rx*ry*ry ) {
            once = 1;
            a--;
            MIRROR_PIXEL( a,b,color )

            if ( a == 0 ) {
                break;
            }
        }

        if ( !once )
            MIRROR_PIXEL( a,b,color )
        }
}

void Lcd_draw_circle_filled ( int16_t x, int16_t y, int16_t r, Pixel_color_t color )
{
__CHECK_INIT
    Lcd_draw_ellipse_filled( x, y, r, r, color );
}

void Lcd_draw_circle_edged ( int16_t x, int16_t y, int16_t r, Pixel_color_t color )
{
__CHECK_INIT
    Lcd_draw_ellipse_edged( x, y, r, r, color );
}

void Lcd_draw_line( int16_t x0, int16_t y0, int16_t x1, int16_t y1, Pixel_color_t color )
{
__CHECK_INIT
    int16_t dx =  abs( x1-x0 ), 
            sx = x0 < x1 ? 1 : -1;
    int16_t dy = -abs( y1-y0 ), 
            sy = y0 < y1 ? 1 : -1;
    int16_t err = dx+dy, e2;

    while ( 1 ) {

        if (    x0 >= 0 
             && x0 < x_bufferResolution 
             && y0 >= 0 
             && y0 < y_bufferResolution ) {
            EV3_PIXEL( x0, y0, color );
        }

        if ( x0==x1 && y0==y1 ) {
            break;
        }

        e2 = 2*err;

        if ( e2 > dy ) {
            err += dy;    /* e_xy+e_x > 0 */
            x0 += sx;
        }

        if ( e2 < dx ) {
            err += dx;    /* e_xy+e_y < 0 */
            y0 += sy;
        }
    }
}
}
