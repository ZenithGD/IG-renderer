# IG-renderer
A CPU renderer written in standard c++

## Installation and requirements

Just clone this repo in your machine and you're done!

Please ensure you've got a C++ compiler installation on your machine. This project uses
[Premake 5](https://premake.github.io) as its build system, so you can customize your
environment as you wish.

- For GNU/Makefile users:
```
premake5 gmake
```

- For Visual studio users:
```
premake5 vs2022
```

Please refer to the [Premake Docs](https://premake.github.io/docs/Using-Premake/) if you need another action for your custom environment.

---

## Usage

```bash
./bin/ToneMapper <in_file{bmp|ppm}> <out_file.{bmp|ppm}>
```

---

## Debugging

We all know how annoying C/C++ can be with memory management, so here's a little tool to help. First install Valgrind on your machine via package manager or from the [official website](https://valgrind.org). After that, execute Valgrind with the executable as an argument:

```bash
valgrind --track-origins=yes --verbose ./bin/ToneMapper
```