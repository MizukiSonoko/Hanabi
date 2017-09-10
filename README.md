

Hanabi
====

[![Build Status](https://travis-ci.org/MizukiSonoko/Hanabi.svg?branch=master)](https://travis-ci.org/MizukiSonoko/Hanabi)  
Hanabi is a tool to feel summer in Japan.

![Hanabi](https://s3-ap-northeast-1.amazonaws.com/mizukisonoko/Fireworks)


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

## Preparation

```bash
$ sudo apt-get install ncurses-dev
```

## Install


### macOS
```
$ brew tap mizukisonoko/hanabi
$ brew install hanabi
```

### build
```bash
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
```
$ hanabi -b NUMBER
```

- Set the hours for the fireworks display.
```
$ hanabi -t TIME
```
"TIME > 80" is better.

- Set Frames Per Second.
```
$ hanabi -f FPS
```

## Licence

[MIT](https://github.com/MizukiSonoko/hanabi/blob/master/LICENSE)

## Author

[MizukiSonoko](https://github.com/MizukiSonoko)


