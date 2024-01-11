# ADE stands for Abstract Doom Editor

## Why to make another editor?

Existing doom editors are powerful tools with various helpful features closing needs of the most demanding mappers. But still there is a place to make mapping easier and more convenient primarily through the implementation of an abstraction layer that allows you to automate work on lindefs and sectors, tags and actions, and so on. Also we will try to keep eye on total performance of editor, decrease memory footprint to provide smooth experience and to allow mappers to make bigger and more detailed maps. And lastly we want to provide broad support for different operating systems such as windows, linux and macos.

## Goals

### Ensure greater accessibility

- aimed to be cross-compatible supporting windows, linux and macos (experimental)

### Provide high performance

- it's written on C++17
- uses fast library based on immediate graphical mode [Dear Imgui](https://github.com/cnjinhao/nana/tree/master) for GUI rendering

### Make mapping easier and more convenient

- abstraction layer over standard wad format
- scripting language Lua for extension
