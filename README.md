# SeleRandr

## TO DO: 

- [x] Get a List of Resolutions and change the display resolution
- [x] Get a List of Refresh Rates and change display refresh rate 
- [x] Change Refresh Rate list dynamically 
- [x] Button to change Rotation of display
- [x] Slider to change scale of display
- [x] Get a List of output and on/off selected output
- [ ] Custom Resolution Creator
- [ ] Change Position of Display if there is more than one display
- [ ] Change the list of Resolutions dynamically

## Build 
```
gcc `pkg-config --cflags gtk+-3.0` -o "selerandr"  "selerandr.c" `pkg-config --libs gtk+-3.0`
```
standalone GUI for xrandr, with options to Resolutions, Refresh rate, rotate, scale. on/off output and add custom resolutions
<img src="https://github.com/ItzSelenux/ItzSelenux.github.io/blob/main/res/pages/selerandr/1.png?raw=true">
<img src="https://github.com/ItzSelenux/ItzSelenux.github.io/blob/main/res/pages/selerandr/2.png?raw=true">
<img src="https://github.com/ItzSelenux/ItzSelenux.github.io/blob/main/res/pages/selerandr/3.png?raw=true">
