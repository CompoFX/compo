## Introduction

Nomad's Compo SDK allows you easily apply word-class image effects on your iOS Applications.

The SDK Contains These Filters
  1. Grey Scale
  2. Lighten
  3. Sepia
  4. Hitam Putih
  5. Scribe
  6. Hike
  7. Cinta
  8. Analog
  9. Thermo
  10. Desaku
  11. Kelud
  12. Hero

## Requirements

Compo SDK needs iOS 7 or above and Apple LLVM Compiler

## Integration

iOS

- Drop Both `CompoView.framework` and `Compo.bundle` onto your project. Just instantiate CompoView using StoryBoard or from Scratch.
- Rename `main.m` file to `main.mm` so it'll be compiled as Objective-C++
- Add "-ObjC" on "Other Linker Flags" inside Build Settings
- Enjoy
  
Android

- Drop `compo.jar` and `x86` `armeabi` `armeabi-v7a` to `libs` directory on your project
- Drop `Compo.bundle` to your `assets` directory
- Put the CompoView to your designer (the view needs to be square)
- Enjoy
 
