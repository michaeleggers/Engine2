# Engine2
A small engine with SDL2/OpenGL using the MD2 format from Quake2 to render animated 3D models.

***About***

My first ever try using OpenGL in order to learn it in 2016.

It loads models from .md2 files and animates the model's keyframes via linear interpolation on the GPU
and features a simple phong lighting model.

***Building***

The project was updated to VS 2017 and will build for x86.

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
|Space      |heroine swings her sword    				  |

***Code Overview***

****Loading models****

The program uses the md2 file format (from id's Quake 2) to draw animated
meshes. The following steps will create such a model ready for rendering:

```Cpp
// excerpt of main.cpp
Texture goblin_texture("./models/goblin/cathos.pcx");
md2Model goblin_model("./models/goblin/tris.md2");

Character goblin;
goblin.setModel(&goblin_model);
goblin.setTexture(&goblin_texture);

goblin_model.pushGPU();
```

****Interpolating the model's keyframes on the vertex shader****

Since the md2 format does not store bones but versions of the same
model in different poses it is necessary to interpolate between these
keyframes in order to get a smooth animation. This is being done on the
vertex shader. Two VBOs are bound to OpenGL's state machine holding the
vertex information of the current and next frame.
The interpolation on the vertex shader simply looks like that:

```glsl
// excerpt of md2_animation_shader.vs
#version 140

in vec3 currentFrame;
in vec3 nextFrame;

...

uniform float interpolation;

void main(){

	vec3 interpolated = currentFrame;
	if (interpolation >= 0.0f) interpolated += interpolation * (nextFrame - currentFrame);
	gl_Position = projection * view * transform * vec4(interpolated, 1.0f);
	...
}
```



***Some impressions***

NPCs follow the character (but they don't actually hurt them... yet)
![alt text](https://github.com/michaeleggers/Engine2/blob/master/Engine2%202016-11-29%2010-58-18-29.gif "engine in action")

Hero is a heroine in the most recent version and loves hot cheetos!
![alt text](https://github.com/michaeleggers/Engine2/blob/master/heroine3.gif "phong illumination model")
