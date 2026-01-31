@echo off
setlocal enabledelayedexpansion

::no space in between = and projectName value

set projectName=main
set files= 
set isInclude= False
set isLib= False
set buildLoc= 

for /r %%i in (*.cpp) do (
	set files=!files! %%i
)
for /r %%i in (*.c) do (
	set files=!files! %%i
)

echo C files found: %files%



g++  -o %projectName% %files% -IC:\glfw3\include\GLFW C:\glfw3\lib\libglfw3dll.a -IC:\glad\include -IC:\glm -lgdi32  -Wall -lmingw32 -Os

%projectName%.exe