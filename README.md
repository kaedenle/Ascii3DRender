# Ascii3DRender
## Description
3D Renderer that can render and rotate many 3D polygons.
Current preloaded shapes include:
* Cube
* Pyramid
* Hexagonal Pyramid

## Directions
Run main.exe.

Controls:
* A rotates CW around the y-axis
* D rotates CCW around the y-axis
* S rotates CW around the x-axis
* W rotates CCW around the x-axis
* X rotates CW around the z-axis
* Z rotates CCW around the z-axis
* ESC kills the program
* C changes the shape

## Buggy Behavior
* Known clipping behavior with the shapes and the backface culling algorithm used (most extreme in pyramid but exists in the cube too)
* Unknown what causes this bug
* Concave shapes don't cull correctly
