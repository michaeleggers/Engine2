# Engine2
A small engine with SDL2/OpenGL using the MD2 format from Quake2 to render animated 3D models.

***About***
This program was part of my work I did at the Hong Kong University of Science and Technology for a Game Programming course.
It is not supposed to be a polished, finished program, although it is perfectly working. There are some design-flaws, but
the goal was to learn the OpenGL API and GLSL. Although, nowadays 3D animation is probably done via a skeleton system all the time
but for simplicity I used the MD2 model format. All the model frames get interpolated overtime to generate a smooth animation. This solution is
quite nice as it uses the vertex shader in order to do the interpolation for all the vertecies as the interpolation code is always the same
and thus perfectly for parallelization. However, I am fully aware of the fact that the model loading takes a little too long at the 
moment. Another topic to work on in the future. Furthermore, at the moment the program uses a variable delta time for its physics update
as it was quite easy to implement for testing but as it gives non-deterministic behavior it is probably the most uncool thing about
the whole program right now.
