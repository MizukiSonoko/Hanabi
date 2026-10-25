

Hanabi
====

[![Build Status](https://travis-ci.org/MizukiSonoko/Hanabi.svg?branch=master)](https://travis-ci.org/MizukiSonoko/Hanabi)  
Hanabi is a tool to feel summer in Japan.

![Hanabi](https://github.com/MizukiSonoko/Hanabi/blob/master/images/demo-movie.gif)


## Description
 This is summer in Japan.
```
  *|*
 *\*/*   _   _                   _     _   
 -*O*-  | | | | __ _ _ __   __ _| |__ (_)  
 */*\*  | |_| |/ _` | '_ \ / _` | '_ \| |  
  *|*   |  _  | (_| | | | | (_| | |_) | |  
   |    |_| |_|\__,_|_| |_|\__,_|_.__/|_|  
   |                                       
 /^^\/^^\/^\/\/^^^^\   /^^\/^\/\    /\^    
/    \ . \   .  .   \^/    \ ,  \^^/   \^/\
-------------------------------------------
###########################################
```

## Run

```bash
$ docker run --rm -e COLUMNS=$COLUMNS -e LINES=$LINES docker.pkg.github.com/mizukisonoko/hanabi/hanabi:1.2
```

### With option
```bash
$  docker run --rm -e COLUMNS=$COLUMNS -e LINES=$LINES docker.pkg.github.com/mizukisonoko/hanabi/hanabi:1.2 hanabi -i
```

## Install 

### macOS
```
$ brew tap mizukisonoko/hanabi
$ brew install hanabi
```

### build
```bash
$ sudo apt-get install ncurses-dev
$ git clone https://github.com/MizukiSonoko/Hanabi
$ cd Hanabi
$ ./configure
$ make
$ sudo make install
```

## Usage

```bash
$ hanabi
```

```
Usage: hanabi [-b bomb] [-t time] [-f fps] [-i] [-m] [-n] [-s text] [-v]
```

## Options

- Set the number of fireworks ball.
```bash
$ hanabi -b NUMBER
```

- Set the hours for the fireworks display.
```bash
$ hanabi -t TIME
```
"TIME > 80" is better.

- Set Frames Per Second. default is 10
```bash
$ hanabi -f FPS
```

- Infinite Hanabi.
```bash
$ hanabi -i
```

- Show mountains at the bottom of the screen.
```bash
$ hanabi -m
```

- Show Niagara fireworks shaped like Mt. Fuji.
```bash
$ hanabi -n
```

- Launch a text-shaped firework. Letters, numbers and `! ? . , : - +` are supported. Other characters are shown as `?`.
```bash
$ hanabi -s "Happy Birthday!"
```

- Show the version.
```bash
$ hanabi -v
```

## Special fireworks

These appear at random while the show is running. No option is needed.

- **Star mine**: a rapid volley of small shells launched one after another.
- **Shaped fireworks (katamono)**: about 1 in 8 shells blooms into a heart, a smile, a star or a cat.
- **Big shell (nishakudama)**: one huge shell that rises from the ground and bursts with a two-tone core.

## Licence

[MIT](https://github.com/MizukiSonoko/hanabi/blob/master/LICENSE)

## Author

[MizukiSonoko](https://github.com/MizukiSonoko)


