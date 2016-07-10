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

int16_t frameBufferLength 	= 0,
        x_bufferResolution 	= 0,
        y_bufferResolution 	= 0,
        bufferLineLength 	= 0;

int32_t __fbfd, __vtfd;
unsigned char *__fbp = NULL;

void Lcd_init( void )
{
    __fbfd = open( "/dev/fb0", O_RDWR );

    fb_fix_screeninfo i;

    if ( ioctl( __fbfd, FBIOGET_FSCREENINFO, &i ) < 0 ) {
        close( __fbfd );
        return;
    }

    fb_var_screeninfo v;

    if ( ioctl( __fbfd, FBIOGET_VSCREENINFO, &v ) < 0 ) {
        close( __fbfd );
        return;
    }

    frameBufferLength = i.smem_len;
    bufferLineLength = i.line_length;
    x_bufferResolution = v.xres;
    y_bufferResolution = v.yres;

    __fbp = ( unsigned char * )mmap( 0, frameBufferLength, PROT_READ | PROT_WRITE, 
    								 MAP_SHARED, __fbfd, 0 );
    __vtfd = open( "/dev/tty", O_RDONLY );
    ioctl( __vtfd, KDSETMODE, KD_GRAPHICS );
}

void Lcd_release( void )
{
    munmap( __fbp, frameBufferLength );
    close( __fbfd );
    ioctl( __vtfd, KDSETMODE, KD_TEXT );
    close( __vtfd );
}

void Lcd_clear( void )
{
    memset( __fbp, 0, frameBufferLength );
}

#define EV3_PIXEL_SET(x,y) __fbp[((x) >> 3) + (y) * bufferLineLength] |= 1 << ((x) & 7)
#define EV3_PIXEL_UNSET(x,y) __fbp[((x) >> 3) + (y) * bufferLineLength] &= 0xFF ^ (1 << ((x) & 7))

#define EV3_PIXEL(x,y,bit) {if (bit) EV3_PIXEL_SET(x,y); else EV3_PIXEL_UNSET(x,y);}

void ev3_pixel_lcd( int32_t x,int32_t y,int32_t bit )
{
    EV3_PIXEL( x,y,bit );
}

void ev3_text_lcd_large( int32_t x,int32_t y,const char *text )
{
    int32_t i;

    for ( i = 0; text[i]; i++ ) {
        if ( text[i] < ' ' || text[i] > 127 ) {
            continue;
        }

        int32_t c_x = ( ( text[i] - ' ' ) % 16 ) * LARGE_LETTER_WIDTH;
        int32_t c_y = ( ( text[i] - ' ' ) / 16 ) * LARGE_LETTER_HEIGHT;
        int32_t a;

        for ( a = 0; a < LARGE_LETTER_WIDTH; a++ ) {
            int32_t X = x + i * LARGE_LETTER_WIDTH + a;

            if ( X < 0 ) {
                continue;
            }

            if ( X >= ( int32_t )x_bufferResolution ) {
                return;
            }

            int32_t b;

            for ( b = 0; b < LARGE_LETTER_HEIGHT; b++ ) {
                int32_t Y = y + b;

                if ( Y < 0 ) {
                    continue;
                }

                if ( Y >= ( int32_t )y_bufferResolution ) {
                    break;
                }

                if ( LARGE_FONT_PIXEL( c_x + a,c_y + b ) ) {
                    EV3_PIXEL_SET( X,Y );
                }
            }
        }
    }
}

void ev3_text_lcd_normal( int32_t x,int32_t y,const char *text )
{
    int32_t i;

    for ( i = 0; text[i]; i++ ) {
        if ( text[i] < ' ' || text[i] > 127 ) {
            continue;
        }

        int32_t c_x = ( ( text[i] - ' ' ) % 16 ) * NORMAL_LETTER_WIDTH;
        int32_t c_y = ( ( text[i] - ' ' ) / 16 ) * NORMAL_LETTER_HEIGHT;
        int32_t a;

        for ( a = 0; a < NORMAL_LETTER_WIDTH; a++ ) {
            int32_t X = x + i * NORMAL_LETTER_WIDTH + a;

            if ( X < 0 ) {
                continue;
            }

            if ( X >= ( int32_t )x_bufferResolution ) {
                return;
            }

            int32_t b;

            for ( b = 0; b < NORMAL_LETTER_HEIGHT; b++ ) {
                int32_t Y = y + b;

                if ( Y < 0 ) {
                    continue;
                }

                if ( Y >= ( int32_t )y_bufferResolution ) {
                    break;
                }

                if ( NORMAL_FONT_PIXEL( c_x + a,c_y + b ) ) {
                    EV3_PIXEL_SET( X,Y );
                }
            }
        }
    }
}

void ev3_text_lcd_small( int32_t x,int32_t y,const char *text )
{
    int32_t i;

    for ( i = 0; text[i]; i++ ) {
        if ( text[i] < ' ' || text[i] > 127 ) {
            continue;
        }

        int32_t c_x = ( ( text[i] - ' ' ) % 16 ) * SMALL_LETTER_WIDTH;
        int32_t c_y = ( ( text[i] - ' ' ) / 16 ) * SMALL_LETTER_HEIGHT;
        int32_t a;

        for ( a = 0; a < SMALL_LETTER_WIDTH; a++ ) {
            int32_t X = x + i * SMALL_LETTER_WIDTH + a;

            if ( X < 0 ) {
                continue;
            }

            if ( X >= ( int32_t )x_bufferResolution ) {
                return;
            }

            int32_t b;

            for ( b = 0; b < SMALL_LETTER_HEIGHT; b++ ) {
                int32_t Y = y + b;

                if ( Y < 0 ) {
                    continue;
                }

                if ( Y >= ( int32_t )y_bufferResolution ) {
                    break;
                }

                if ( SMALL_FONT_PIXEL( c_x + a,c_y + b ) ) {
                    EV3_PIXEL_SET( X,Y );
                }
            }
        }
    }
}

void ev3_text_lcd_tiny( int32_t x,int32_t y,const char *text )
{
    int32_t i;

    for ( i = 0; text[i]; i++ ) {
        if ( text[i] < ' ' || text[i] > 127 ) {
            continue;
        }

        int32_t c_x = ( ( text[i] - ' ' ) % 16 ) * TINY_FONT_WIDTH;
        int32_t c_y = ( ( text[i] - ' ' ) / 16 ) * TINY_LETTER_HEIGHT;
        int32_t a;

        for ( a = 0; a < TINY_LETTER_WIDTH; a++ ) {
            int32_t X = x + i * TINY_LETTER_WIDTH + a;

            if ( X < 0 ) {
                continue;
            }

            if ( X >= ( int32_t )x_bufferResolution ) {
                return;
            }

            int32_t b;

            for ( b = 0; b < TINY_LETTER_HEIGHT; b++ ) {
                int32_t Y = y + b;

                if ( Y < 0 ) {
                    continue;
                }

                if ( Y >= ( int32_t )y_bufferResolution ) {
                    break;
                }

                if ( TINY_FONT_PIXEL( c_x + a,c_y + b ) ) {
                    EV3_PIXEL_SET( X,Y );
                }
            }
        }
    }
}

void ev3_rectangle_lcd( int32_t x,int32_t y,int32_t w,int32_t h,int32_t bit )
{
    int32_t a,b;
    int32_t minx = x;
    int32_t miny = y;
    int32_t maxx = x+w;
    int32_t maxy = y+h;

    if ( minx >= ( int32_t )x_bufferResolution ) {
        return;
    }

    if ( miny >= ( int32_t )y_bufferResolution ) {
        return;
    }

    if ( maxx < 0 ) {
        return;
    }

    if ( maxy < 0 ) {
        return;
    }

    if ( minx < 0 ) {
        minx = 0;
    }

    if ( miny < 0 ) {
        miny = 0;
    }

    if ( maxx >= x_bufferResolution ) {
        maxx = x_bufferResolution-1;
    }

    if ( maxy >= y_bufferResolution ) {
        maxy = y_bufferResolution-1;
    }

    for ( a = minx; a <= maxx; a++ )
        for ( b = miny; b <= maxy; b++ ) {
            EV3_PIXEL( a,b,bit );
        }
}

void ev3_rectangle_lcd_out( int32_t x,int32_t y,int32_t w,int32_t h,int32_t bit )
{
    int32_t a;
    int32_t minx = x;
    int32_t miny = y;
    int32_t maxx = x+w;
    int32_t maxy = y+h;

    if ( minx >= ( int32_t )x_bufferResolution ) {
        return;
    }

    if ( miny >= ( int32_t )y_bufferResolution ) {
        return;
    }

    if ( maxx < 0 ) {
        return;
    }

    if ( maxy < 0 ) {
        return;
    }

    if ( minx < 0 ) {
        minx = 0;
    }

    if ( miny < 0 ) {
        miny = 0;
    }

    if ( maxx >= x_bufferResolution ) {
        maxx = x_bufferResolution-1;
    }

    if ( maxy >= y_bufferResolution ) {
        maxy = y_bufferResolution-1;
    }

    for ( a = minx; a <= maxx; a++ ) {
        EV3_PIXEL( a,miny,bit );
        EV3_PIXEL( a,maxy,bit );
    }

    for ( a = miny+1; a <= maxy-1; a++ ) {
        EV3_PIXEL( minx, a, bit );
        EV3_PIXEL( maxx, a, bit );
    }
}

void ev3_circle_lcd( int32_t x,int32_t y,int32_t r,int32_t bit )
{
    int32_t a,b;
    int32_t minx = x-r;
    int32_t miny = y-r;
    int32_t maxx = x+r;
    int32_t maxy = y+r;

    if ( minx >= ( int32_t )x_bufferResolution ) {
        return;
    }

    if ( miny >= ( int32_t )y_bufferResolution ) {
        return;
    }

    if ( maxx < 0 ) {
        return;
    }

    if ( maxy < 0 ) {
        return;
    }

    if ( minx < 0 ) {
        minx = 0;
    }

    if ( miny < 0 ) {
        miny = 0;
    }

    if ( maxx >= x_bufferResolution ) {
        maxx = x_bufferResolution-1;
    }

    if ( maxy >= y_bufferResolution ) {
        maxy = y_bufferResolution-1;
    }

    if ( bit )
        for ( a = minx; a <= maxx; a++ )
            for ( b = miny; b <= maxy; b++ ) {
                if ( ( a-x )*( a-x ) + ( b-y )*( b-y ) >= r*r ) {
                    continue;
                }

                EV3_PIXEL_SET( a,b );
            }
    else
        for ( a = minx; a <= maxx; a++ )
            for ( b = miny; b <= maxy; b++ ) {
                if ( ( a-x )*( a-x ) + ( b-y )*( b-y ) >= r*r ) {
                    continue;
                }

                EV3_PIXEL_UNSET( a,b );
            }
}

#define MIRROR_PIXEL_SET(a,b) \
{ \
if (y+b >= 0 && y+b < y_bufferResolution) \
{ \
	if (x+a >= 0 && x+a < x_bufferResolution) \
		EV3_PIXEL_SET(x+a,y+b); \
	if (x-a >= 0 && x-a < x_bufferResolution) \
		EV3_PIXEL_SET(x-a,y+b); \
} \
if (y-b >= 0 && y-b < y_bufferResolution) \
{ \
	if (x+a >= 0 && x+a < x_bufferResolution) \
		EV3_PIXEL_SET(x+a,y-b); \
	if (x-a >= 0 && x-a < x_bufferResolution) \
		EV3_PIXEL_SET(x-a,y-b); \
} \
}
#define MIRROR_PIXEL_UNSET(a,b) \
{ \
if (y+b >= 0 && y+b < y_bufferResolution) \
{ \
	if (x+a >= 0 && x+a < x_bufferResolution) \
		EV3_PIXEL_UNSET(x+a,y+b); \
	if (x-a >= 0 && x-a < x_bufferResolution) \
		EV3_PIXEL_UNSET(x-a,y+b); \
} \
if (y-b >= 0 && y-b < y_bufferResolution) \
{ \
	if (x+a >= 0 && x+a < x_bufferResolution) \
		EV3_PIXEL_UNSET(x+a,y-b); \
	if (x-a >= 0 && x-a < x_bufferResolution) \
		EV3_PIXEL_UNSET(x-a,y-b); \
} \
}

void ev3_circle_lcd_out( int32_t x,int32_t y,int32_t r,int32_t bit )
{
    int32_t a,b;

    if ( bit ) {
        a = r;

        for ( b = 0; b <= r; b++ ) {
            int once = 0;

            while ( a*a + b*b >= r*r ) {
                once = 1;
                a--;
                MIRROR_PIXEL_SET( a,b )

                if ( a == 0 ) {
                    break;
                }
            }

            if ( !once )
                MIRROR_PIXEL_SET( a,b )
            }
    } else {
        a = r;

        for ( b = 0; b <= r; b++ ) {
            int once = 0;

            while ( a*a + b*b >= r*r ) {
                once = 1;
                a--;
                MIRROR_PIXEL_UNSET( a,b )

                if ( a == 0 ) {
                    break;
                }
            }

            if ( !once )
                MIRROR_PIXEL_UNSET( a,b )
            }
    }

}

void ev3_ellipse_lcd( int32_t x,int32_t y,int32_t rx,int32_t ry,int32_t bit )
{
    int32_t a,b;
    int32_t minx = x-rx;
    int32_t miny = y-ry;
    int32_t maxx = x+rx;
    int32_t maxy = y+ry;

    if ( minx >= ( int32_t )x_bufferResolution ) {
        return;
    }

    if ( miny >= ( int32_t )y_bufferResolution ) {
        return;
    }

    if ( maxx < 0 ) {
        return;
    }

    if ( maxy < 0 ) {
        return;
    }

    if ( minx < 0 ) {
        minx = 0;
    }

    if ( miny < 0 ) {
        miny = 0;
    }

    if ( maxx >= x_bufferResolution ) {
        maxx = x_bufferResolution-1;
    }

    if ( maxy >= y_bufferResolution ) {
        maxy = y_bufferResolution-1;
    }

    if ( bit )
        for ( a = minx; a <= maxx; a++ )
            for ( b = miny; b <= maxy; b++ ) {
                if ( ( a-x )*( a-x )*ry*ry + ( b-y )*( b-y )*rx*rx >= rx*rx*ry*ry ) {
                    continue;
                }

                EV3_PIXEL_SET( a,b );
            }
    else
        for ( a = minx; a <= maxx; a++ )
            for ( b = miny; b <= maxy; b++ ) {
                if ( ( a-x )*( a-x )*ry*ry + ( b-y )*( b-y )*rx*rx >= rx*rx*ry*ry ) {
                    continue;
                }

                EV3_PIXEL_UNSET( a,b );
            }
}

void ev3_ellipse_lcd_out( int32_t x,int32_t y,int32_t rx,int32_t ry,int32_t bit )
{
    int32_t a,b;

    if ( bit ) {
        a = rx;

        for ( b = 0; b <= ry; b++ ) {
            int once = 0;

            while ( a*a*ry*ry + b*b*rx*rx >= rx*rx*ry*ry ) {
                once = 1;
                a--;
                MIRROR_PIXEL_SET( a,b )

                if ( a == 0 ) {
                    break;
                }
            }

            if ( !once )
                MIRROR_PIXEL_SET( a,b )
            }
    } else {
        a = rx;

        for ( b = 0; b <= ry; b++ ) {
            int once = 0;

            while ( a*a*ry*ry + b*b*rx*rx >= rx*rx*ry*ry ) {
                once = 1;
                a--;
                MIRROR_PIXEL_UNSET( a,b )

                if ( a == 0 ) {
                    break;
                }
            }

            if ( !once )
                MIRROR_PIXEL_UNSET( a,b )
            }
    }

}

void ev3_line_lcd( int32_t x0, int32_t y0, int32_t x1, int32_t y1,int32_t bit )
{
    int32_t dx =  abs( x1-x0 ), sx = x0<x1 ? 1 : -1;
    int32_t dy = -abs( y1-y0 ), sy = y0<y1 ? 1 : -1;
    int32_t err = dx+dy, e2;

    if ( bit )
        for ( ;; ) {
            if ( x0 >= 0 && x0 < ( int32_t )x_bufferResolution && y0 >= 0 && y0 < ( int32_t )y_bufferResolution ) {
                EV3_PIXEL( x0,y0,1 );
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
    else
        for ( ;; ) {
            if ( x0 >= 0 && x0 < ( int32_t )x_bufferResolution && y0 >= 0 && y0 < ( int32_t )y_bufferResolution ) {
                EV3_PIXEL( x0,y0,0 );
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