cls

del Controller.exe Window.res Controller.obj

cl.exe /c /EHsc	Controller.cpp
rc.exe Window.rc

link.exe Controller.obj Window.res user32.lib gdi32.lib /SUBSYSTEM:WINDOWS

Controller.exe
