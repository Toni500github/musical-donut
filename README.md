# musical-donut
a donut, but you can spin it with some music in the background

## Usage
`donut [-h] [-v] [-s SPEED] [-p PATH]`

```bash
# Play a path in the background
$ donut -p /path/to/audio.wav

# Spin with some speed
$ donut -s 20
```

## Building
just run `make`\
if you don't have it installed run `cc -o donut src/donut.c src/parg.c -Iinclude -DVERSION=\"0.0.1\" -lm`
