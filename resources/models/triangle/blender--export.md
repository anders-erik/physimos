
# Axes

## Forward axis = y

## Up axis = z


# Import Reasoning

If i set up the blender viewport to match the output settings, then the output coordinates as if x=right, y=into screen, thus if the vertices are in that plane then the ..

## Opengl coordinates?

Maybe blender assumes opengl coordinates relative to the viewport?

So to predict output axes the following step has to be made:

1. viewport should be set to correctly match the model as if it were currently being rendered using the opengl coordinate system (which to be fait it probably does?), and
2. set export axes to match the axes as seen in viewport from step 1.


