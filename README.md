

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

## Licence

[MIT](https://github.com/MizukiSonoko/hanabi/blob/master/LICENSE)

## Author

[MizukiSonoko](https://github.com/MizukiSonoko)


