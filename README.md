# fx-9860G File Metadata

This program loads the header data of a file for the CASIO fx-9860G series of calculators and displays it in a human-readable format.

## Usage

```
Usage: fx-meta [filename]
Prints out metadata of a CASIO fx-9860G file.
```

Example output with [my PONG port](https://github.com/thegreatrazz/casio-pong):
```
= File Header =
Identifier:     USBPower�
Type:           0xf3
File Size:      16696 bytes
Obj. count:     65535

= Addin Header =
Internal name:  @PONG
eStrips:        0
Version:        01.00.0000
Date:           2018.0902.1816
Title:          Pong
Size:           0x4138 bytes
Icon:
         ███   ██  █  █  ███         █  
         █  █ █  █ ██ █ █            █  
         ███  █  █ █ ██ █ ██         █  
         █    █  █ █  █ █  █         █  
         █     ██  █  █  ██          █  
                                     █  
                                     █  
             ██                      █  
             ██                      █  
             ██   ██                 █  
             ██  █████               █  
             ██  ████ ██       ███████  
             ██   ██  █ █      ███████  
             ██     ██  █ █    ███████  
             ██       ██   █   ███████  
             ██          █  █  ███████  
                               ███████
```

## Build

All you need is `make`, `gcc` and `pkg-config`. Download this repo (either through `git` or as a `zip`/`tgz`) and run `make`.

The binary should be in `bin/fx-meta`.

## Why

I've made it because I'm currently learning C (for uni) and I think dipping my feet into the water is a great way to get started.

Just for now, I can see a few issues with it. Everything is in the `main` method, it's targeted towards the GCC compiler (because that's what I have on my Linux machine, and endianness is a thing), strings from the headers are not null-terminated (look for the `// nt` comment).

If you have any solutions to these problems (that can be properly comprehended by a complete C/C++ newbie) that would be amazing.