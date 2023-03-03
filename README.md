# SeleRandr

## TO DO: 

- [x] Get a List of Resolutions and change the display resolution
- [x] Get a List of Refresh Rates and change display refresh rate 
- [x] Change Refresh Rate list dynamically 
- [x] Button to change Rotation of display
- [x] Slider to change scale of display
- [x] Get a List of output and on/off selected output
- [x] Change Position of Display if there is more than one display
- [x] Hide Output / Position options if there is only one output
- [x] Custom Resolution Creator

- [ ] Change the list of Resolutions dynamically

## Build 
Main Program:

```
gcc `pkg-config --cflags gtk+-3.0` -o "selerandr"  "selerandr.c" `pkg-config --libs gtk+-3.0`
```

Custom Resolution creator:

```
gcc `pkg-config --cflags gtk+-3.0` -o "selerandr-cr"  "selerandr-cr.c" `pkg-config --libs gtk+-3.0`
```

standalone GUI for xrandr, with options to Resolutions, Refresh rate, rotate, scale. on/off output and add custom resolutions
<p align="center" width="100%">
<img width=46% src="https://github.com/ItzSelenux/ItzSelenux.github.io/blob/main/res/pages/selerandr/1.png?raw=true">
<img width=47% src="https://github.com/ItzSelenux/ItzSelenux.github.io/blob/main/res/pages/selerandr/2.png?raw=true">
<img src="https://github.com/ItzSelenux/ItzSelenux.github.io/blob/main/res/pages/selerandr/3.png?raw=true">
<img src="https://github.com/ItzSelenux/ItzSelenux.github.io/blob/main/res/pages/selerandr/4.png?raw=true">
</p>



#About:

Created by ItzSelenux Using GTK3, in C with Geany

