/*
   Spinning 3D donut with option of playback music
    Original video: https://www.youtube.com/watch?v=DEqXNfs_HhY
*/

#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "parg.h"
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

#if (defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) || defined(__CYGWIN__) || \
     defined(__MINGW32__) || defined(__MINGW64__))
# define PLATFORM_WINDOWS 1
#else
# define PLATFORM_WINDOWS 0
#endif

#if PLATFORM_WINDOWS
# include <windows.h>
# include <conio.h>

void platform_sleep(int ms)
{
    Sleep(ms);
}

bool is_exiting(void)
{
    if (_kbhit())
    {
        char c = _getch();
        return c == 'q' || c == 'Q';
    }
    return false;
}
#else
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/select.h>

void platform_sleep(int ms)
{
    usleep(ms * 1000);
}

struct termios orig_termios;

void disable_raw_mode(void)
{
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enable_raw_mode(void)
{
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disable_raw_mode);

    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON);  // Disable echo and canonical mode
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int kbhit(void)
{
    struct timeval tv = {0L, 0L};
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    return select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv) > 0;
}

bool is_exiting(void)
{
    struct timeval tv = {0L, 0L};
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    if (select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv) > 0)
    {
        char c = '\0';
        read(STDIN_FILENO, &c, 1);
        return (c == 'q' || c == 'Q');
    }
    return false;
}
#endif

void print_donut(float speed)
{
    float A = 0, B = 0;
    float i, j;
    int k;
    float z[1760];
    char b[1760];
    float Xspeed = speed / 100;
    float Yspeed = Xspeed / 2;
    printf("\x1b[2J");
    while (1)
    {
        memset(b, 32, 1760);
        memset(z, 0, 7040);
        for(j=0; j < 6.28; j += 0.07)
        {
            for(i=0; i < 6.28; i += 0.02)
            {
                float c = sin(i);
                float d = cos(j);
                float e = sin(A);
                float f = sin(j);
                float g = cos(A);
                float h = d + 2;
                float D = 1 / (c * h * e + f * g + 5);
                float l = cos(i);
                float m = cos(B);
                float n = sin(B);
                float t = c * h * g - f * e;
                int x = 40 + 30 * D * (l * h * m - t * n);
                int y = 12 + 15 * D * (l * h * n + t * m);
                int o = x + 80 * y;
                int N = 8 * ((f * e - c * d * g) * m - c * d * e - f * g - l * d * n);
                if(22 > y && y > 0 && x > 0 && 80 > x && D > z[o])
                {
                    z[o] = D;
                    b[o] = ".,-~:;=!*#$@"[N > 0 ? N : 0];
                }
            }
        }
        printf("\x1b[H");
        for(k = 0; k < 1761; k++)
            putchar(k % 80 ? b[k] : 10);
        if (is_exiting())
            break;
        A += Xspeed;
        B += Yspeed;
        platform_sleep(30);
    }
}

bool parseargs(int argc, char* argv[], char *path, float *speed)
{
    struct parg_state ps;
    int c;

    parg_init(&ps);

    while ((c = parg_getopt(&ps, argc, argv, "hvs:p:")) != -1)
    {
        switch (c)
        {
            case 'h':
                    printf("Usage: %s [-h] [-v] [-s SPEED] [-p PATH]\n", argv[0]);
                    return false;
            case 'p':
                    strcpy(path, ps.optarg);
                    break;
            case 's':
                    *speed = atof(ps.optarg);
                    break;
            case 'v':
                    printf("musical-donut " VERSION "\n");
                    return false;
            case '?':
                    if (ps.optopt == 's' || ps.optopt == 'p')
                        fprintf(stderr, "option -p or -s requires an argument\n");
                    else
                        fprintf(stderr, "unknown option -%c\n", ps.optopt);
                    return false;
            default:
                    fprintf(stderr, "error: unhandled option -%c\n", c);
                    return false;
        }
    }

    return true;
}

int main(int argc, char *argv[])
{
#if !PLATFORM_WINDOWS
    enable_raw_mode();
#endif

    char path[4096];
    float speed = 10;
    if (!parseargs(argc, argv, path, &speed))
        return -1;

    ma_engine engine;
    if (ma_engine_init(NULL, &engine) != MA_SUCCESS) {
        fprintf(stderr, "Failed to init the audio engine");
        return -1;
    }

    ma_engine_play_sound(&engine, path, NULL);
    print_donut(speed);

    ma_engine_uninit(&engine);

    return 0;
}
