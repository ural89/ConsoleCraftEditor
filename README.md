# ConsoleCraftEditor
### Editor for ConsoleCraftEngine
#### Written in C++
#### Will work in Linux

## Prerequisites

To build ConsoleCraftEditor, you'll need to install `ninja` and `ncurses`. Follow the instructions below to install these dependencies on a Linux system.

### Installing Ninja

Ninja is a small build system with a focus on speed. To install `ninja`, open your terminal and run:

```sh
sudo apt-get update
sudo apt-get install ninja-build
```
### Installing ncurses
```sh
sudo apt-get update
sudo apt-get install libncurses5-dev libncursesw5-dev
```

## Build:
```
mkdir build
cd build
cp ../imgui.ini .
cmake -G Ninja ..
ninja
./ConsoleCraftEditor
```

## Running a project:
```
To run project from editor you need to open editor with terminal. Your game will run on this terminal when you click run.
```
## Video:
[![Watch the video](https://img.youtube.com/vi/gedbAzRl_F8/0.jpg)](https://youtu.be/gedbAzRl_F8)
