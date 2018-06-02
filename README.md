# Engine2
A small engine with SDL2/OpenGL using the MD2 format from Quake2 to render animated 3D models.

***About***

My first ever try using OpenGL in order to learn it in 2016.

It loads models from .md2 files and animates the model's keyframes via linear interpolation on the GPU
and features a simple phong lighting model.

***Controls***

|Keyboard   |Action                                       |
|-----------|---------------------------------------------|
|1          |free camera active (fly around the scene)    |
|2          |game camera active (cam follows heroine)     |
|Up         |Pitch cam upwards (free camera only)         |
|Down       |Pitch cam downwards (free camera only)       |
|W          |free cam: dolly forward                      |
|           |game cam: run into the screen                |
|S          |free cam: dolly backwards                    |
|           |game cam: run towards the screen             |
|A          |free cam: strave left                        |
|           |game cam: run left                           |
|D          |free cam: strave right                       | 
|           |game cam: run right                          |



***Building***

The project was updated to VS 2017 and will build for x86.


***What it does***
![alt text](https://github.com/michaeleggers/Engine2/blob/master/Engine2%202016-11-29%2010-58-18-29.gif "engine in action")
