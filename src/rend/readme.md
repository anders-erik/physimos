
# librend / libphyrend


## Rendering calls

- triangle
- line
- point
- 

## coordinate systems

- real_universe
- home (ring)
- 3D
- ndc
- 2d


## libphysurf

Provides a cross platform rendering interface optimized for ease of use. 

Expected capabilities:
- easy render calls
- shader modification
- render to stand alone image/window
- save surface to file

Relies primarily on Core OpenGL interface. If hardware acceleration is not available, Mesa software renderer is used as backup. Future might use Vulkan/WebGPU/etc.


Draw calls:

- camera
- line
- triangle
- mesh
- wire
- arc
- circle
- bezier
- geom...
- 


(NDC: raw Opengl coordinates, no transforms)
- lineNDC
- TriangleNDC

(2D: transforms any input to surface coordinates, which is a 2d matrix of pixel data)
- line2D

(3D: transforms according to a set camera)
- line 3D

