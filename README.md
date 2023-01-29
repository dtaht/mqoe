# MQOE - An emulator for high speed packet processing, and queuing.

This start life as an attempt to write a sim to exersise the
LibreQos codebase. It then started to grow and grow.

## Why

As I looked at the python and rust explosions of veneers
over the C functions, the error checking, the close to the kernel
needs, verses what I wanted in an emulator, I decided that perhaps
it would be best to outline my ideas in my native tongue, C.

Creating a lookup table required programming the AST
Creating fdtimers to use or synthetic time, hard
Pulling from postgres, etc. 
I wanted to use the tc_flower classifier and other
htb offloads if at all possible, as well as network namespaces

Rust had all these really great abstractions, but ... a lookup table...

## Adopting the C17 standard

It turned out there were C libraries already written that
did what I needed, in most cases faster than anything else.


