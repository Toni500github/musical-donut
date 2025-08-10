# musical-donut
a donut, but you can spin it with some music in the background

## Usage
`donut [-h] [-v] [-s SPEED] [-p PATH]`

press 'q' to exit.
```bash
# Play a path in the background
$ donut -p /path/to/audio.wav

# Spin with some speed
$ donut -s 20

# Enter in interactive mode if no path
$ donut
Looks like you didn't select a path to an MP3, please insert one here: /path/to/audio.wav
Select a speed (def. 10): 15
```

## Building
just run `make`\
if you don't have it installed run `cc -o donut src/donut.c src/parg.c -Iinclude -DVERSION=\"0.0.3\" -lm`
