# TODO


# 2025

## Goals

- simulations
- shaders
	- lighting
	- outline
- vector visualization
- object builder mode
	- mesh
	- parametric
	- terrain generator
- drawing utility
- scripting
	- language?
- physimos introspection
	- process
	- object files
	- assembly

---

### 2025-02-19
- New UI shader class(es?)
	- color, texture, string?
- UI Texture class 
	- load static textures on startup
		- colors
		- icon
	- request static textures (simply return the texture id on passed enum)
- UI Editor
	- change textures


### 2025-02-17
- UI primitive UI editor
	- make a UI component with a bound ui primitive object
	- live reload shaders, textures, dimensions, etc.
- new shader interface [do what makes sense when making the editor above]
- texture
	- static
		- icons
	- vector 'texture'
- 2D transform object
	- might be used in A LOT of places
		- sub-part of scene object
		- ui
		- drawing
		- etc.
	- LEARNING TOOL thats built into physimos and not in a python notebook!



### 2025-02-14
- pimage
	- BMP tests (especially a few different bmp files : different souces, try large sizes, aspect ratios, etc.)
	- glTexture
	- bitmap
- plib
	- process
		- envvar
- pmodel
	- OBJ
		- loading!


### 2025-02-13
- ptest
	- pimage
- pimage
	- bitmap
	- BMP
- plib
	- fs !


### 2025-02-12
- ptest
	- pimage
- pimage
	- bitmap object
	- texture object
		- dynamic : dynamically bound to in memory bitmap
		- static  : one time loading into gl texture - may discard bitmap from process memory
	- font
		- bitmap
		- vector
	- loaders
		- bmp

### 2025-02-11
- |o| ui
	- |o| primitive
		- |o| transform
		- |o| void component context pointer
		- |o| boundObject pointer in component
	- |o| improve ~~modules~~ components
- ptest
	- pbitmap
- pimage
	- bitmap object
		- loaders
			- bmp
	- texture object
		- dynamic : dynamically bound to bitmap
		- static  : explicit loading into gl texture
- pmodel
	- pmesh
	- pmaterial


### 2025-02-10
- pmodel
	- |o| Obj
	- mesh
- render using pmodel from obj-file
- draw vector in scene
	- x, y, z
	- start with one at scene origin



### 2025-02-09
- Vec3
	- |o| update transform
	- rotation - euler angles
	- methods
		- distanceBetween(vec)
			- difference?
		- angleBetween(vec3)
		- normalize()
		- unitVectorPointingTowards(vec3)
- |o| transforms:
	- |o| separate transform for initial, current, and previous steps
- draw vector in scene
	- x, y, z
	- start with one at scene origin



### 2025-02-08
- Vec3
- ptest

### 2025-02-07
- PVec3
	- class
		- operations
		- methods
	- model
	- renderer
- |o| UI
	- |o| play around with new component system
- |o| scene library


### 2025-02-06
- PScene
	- |o| move current global functions into namespace
	- || Scene object
		- |o| usable class
		- || interface for scene contents
	- PCamera
		- PVec?
- UI
	- play around with new components
		- make appropriate adjustments
		- write thoughts down
	- UiPObject
		- setPobject is part of base calss and calls update method
			- is that possible ?
		

### 2025-02-05
- UiPScene
	- |o| list Pobjects
	- |o| reload UiPObject
	- new object
- |o| UiPObject
	- |o| update pos on inc/dec

### 2025-02-04
- |o| automatically update primitive properly when set as child/parent
	- |o| specifically the setX and setY methods should be reloaded on child after parent is set
- Expand the UiWorldObject
	- UiPObject
	- properties
		- visibility
		- position step size
		- rotate
		- scale
	- icon textures
		- increase/decrease
- UiWorldScene
	- world object list
		- add new
		- buttons per world object
			- populate UiWorldObject with clicked object
			- toggle visibility
			- delete
	- cameras
	- simulators


### 2025-02-03
- nested HoriRef::Right is not working properly
- linear primitives structure for components
- |o| extend primitive object with pointers to scene object!
	- an updateComponent() to load 'static' scene object data, and  
		- |o|  e.g. position, current shader, etc.
	- |o| interactive ui primitives bound to methods on scene object
		- |o| e.g. toggle wireframe, move up 1m, set velocity, etc.


### 2025-02-02
- |o| font/text rendering
	- |o| rearrange the ascii table with 30 at bottom left, as BMP structure
	- |o| display words using primitives
		- |o| rendered text will automatically be added to primitive on 'setText'
- |o| create nested element
	- |o| location is relative to parent
	- proper list- ??
- |o| detect primitive interaction
	- |o| id
	- |o| hover
	- click
- primitive flags
	- clickable
	- |o| hovarable
- bind worldobject properties to primitive (next next step)



### 2025-01-31/02-01:
- add more code to all subsystems
	- explore and add features until natural subsystems emerge
	- many shaders !

1. |o| proper window callback
	- implement mouse pointer location widget
2. |o| highlight on hover
	- mouse pointer change
	- |o| ui element color change
3. || nested ui elements
4. |o| improved font



<br>

---
---
---

<br>

# 2024


## LONG TERM
- simulation backend can perform distributed simuilation computation
- all objects and simulation handling is done through the UI/GUI/Editor/World

## SHORT TERM
- run simulations that does not keep up with 60 fps!
- lighting shader
- obj loader can handle n-edged convex  polygon faces (render calls or force triangulation on load)
- simulation backend can perform compute using parallel code and GPU
- timing object
	- for each scene



## 2024-10-04/05/06
- each UI element has its own render-call
- add a world object list with toggleable properties
	- visibility, active, texture, rigidbody, parent-transform, etc.
- Potential UI structure:
	- Element [char, string?, square, checkbox]-> Unit [toggle worldobject properties] -> Module [list of toggle-units]
- Make sure all WO with velocities/rigidbodies are reset when clicking on STOP


## 2024-09-29/30/1
- |o| move objects to new rendering pipeline!
- |o| move simulator container to new pipeline
- |o| lean out the wireframe generation (get rid of duplicate lines)
- |o| Create transform object for world objects
- add a child of a rotating world object [to make sure the transform is inherited!]
- UI
	- clean up rendering pipline
	- input callback function
	- be able to reposition world object in world
	- change text-color


## 2024-09-28
- Implement on at least one type of world object with new RENDPIPE [expect quite a few hours!]
	- NEW WORLD OBJECT SPECIFICALLY FOR THIS PURPOSE
		- name = rendpipe_obj


## 2024-09-27
- |o|render the first simulator in its world container
- |x| Implement render class for world objects
	- |x| automate the world object creation
		- |x| scene file!
- FIXED RENDERING PIPLINES
	- |o| Define
		- |o| .znotes/cmake-refactoring.md
			- [each rendered object has to have a renderer-object attached to it; each renederer obejct has to be inlizalied with an explicit renering pipline that specifices vertex data format, shader to use, data source]
	- Implement on at least one type of world object
		- NEW WORLD OBJECT SPECIFICALLY FOR THIS PURPOSE
			- name = rendpipe_obj
		- NOTES
			- Modelccp/hhp is taking on a central role in managing the model interface for both the world objects AND the renderer!
				- IT should (maybe, I can't guarentee it right now... lol) keep the vertices, certex count etc.
			- I should make a tranform-like structure for position-scale-rotaion etc. - this would include the current Vec3 that I am using!
			- probably a rigidbody-library that uses the tranform=lib


## 2024-09-26
- `compile and run program using CMake`
- rename files to have cc/hh extension instead of cpp/hpp


## 2024-09-25
### TODO
- |o| vector math library
- render the first simulator in its world container



## 2024-09-21/26
- Cmake!
	- restructuring src folder into directories
		- world objects directory
		- simulator directory
		- renderers
- render the fist simulator in its world container
- import first png file
- UI widget containers

## 2024-09-20

### TODO:
- Simulator
	- |o| separate 3D space (inside wireframe)
	- |o| simple particle gravity
	- start simulation 'button'
- UI improvements
	- change color of text
	- containers for widgets
		- z values will be equal -> z fighting!
	- click on world objects
		- indicate that by altering the color of the object in world object widget



## 2024-09-17/18/19
- UI 
	- |o| timing widget
		- fps, frame, time
	- |o| start/pause/stop
	- hardware widget
	- |o| world objects widget
		- |o| click on name - turn object on/off
	- |o| prevent reloading textures
- ui-file
	- |x| nested elements
	- widgets
- Simulation 'Environments'
	- each environment has a separate goal
		- e.g. compare algorithms, specific visulaization, accuracy, etc.
	- Each environment has it's one physical location/dimensions in the world
- detect click on worldObjects
	- matrix/vector math functions
- strings
	- change color
	- alpha character background
	- (BIG PROBLEM IS IMPORTING TEXTURE WITH ALPHA....)


## 2024-09-14/15

### TODO: ONE top-level bullet point done [or one UI sub-bullet]
- Object Renderer class
	- Shader program
	- vao's, vbo's
	- gl-config (setting vao, vbo, uniforms [, switch shader], etc.)
	- rendering call
- Rigid body
	- use rendering class
	- draw all (12?) lines of a cube
- simulation widget
	- 1D gravity
		- inital condition
		- start + cancel
			- resource utilization
		- Write simulation output
			- simulation file format
				- input
				- output
					- file
						- my own format?
						- format formats?
					- graphs
		- Minimal graph
- Logging
	- loaders
	- errors
	- General output
	-
- UI
	- Multiple UI's
		- Scene UI
		- Simulation UI/widgets
		- Hardware UI
			- cpu/memory
			- GPU's
				- resources/usage
				- drivers
		- Log
			- Errors !
	- timer info:
		- fps
		- frame counter
		- world time
	- start/pause/stop
	- list of world objects
		- selectable list-items to get more info on specifc objects
	- All three of the above menus should be toggleable/hidable
	- resource utilization
		- GPU usage timeseries (~60sec)
		- CPU
		- App memory usage



## 2024-09-13

### TODO:
- |o| obj-no-light-shaders/program
	- |o| shaderprogram based on obj-files (v, vn, vt)
	- |o| no ambient, specular, etc. information. Just plain texture.
- |o| import obj+mtl with texture
	- |o| Make a building in blender that we can import and 'walk' into
- |o| improved world ground collisions
	- |o| bounce on object vertex hit
	- |o| rotation on collision




## 2024-09-07 - 2024-09-10

### TODO:
- physics simulation improvement
	- rotation when falling of ground
	- collition between world objects
- Add an improved font
- obj-file material loader
	- Add a simple building
- FPS style navigation
	- Make build wall impenetrable
- Add non-flat ground
- UI
	- List all world object
		- Dynamically select to track specific object
	- Reset Inital value button
	- Hot reload
		- New ui elements
		- new objects, materials, etc. 
- Slight randomization to world objects



## 2024-09-06

### TODO:
- |o| enable ui element texture
- |o| detect and respond to ui element click
- |o| physics (non-simulation run)
	- |o| worldscene object - gets updated from the main loop
		- |o| controls the camera
		- |o| holds all world objects
			- |o| give worldobject a flag to toggle gravity
		- |o| physics updates
			- |o| apply gravtiy
			- |o| bounce on ground
		- |o| calls the world renderer
- |o| Obj-file loader
- |o| Add dynamic world object information to UI


## 2024-09-04

### TODO
- refactor
	- |o| windowing functionality
	- |o| Input update (raw input handling, camera, ui, simulation)
	- |o| WorldRenderer
- Detect ui click
- Add multiple cubes to the world with a simple one statement creation



## 2024-08-31 - 2024-08-03

### TODO
- |o| generalize bmp loader (2024-08-31)
- |o| render text (2024-09-01)
- |o| show fps
- refactor 
- detect ui element click
- ground as world-object
- detect objects intersecting with ground
- render using indices and drawElements


## 2024-08-30


### DONE
1. |o| texture rendering
2. |o| have ui renderer able to render several rectangles
	- 2 options: vao for each element OR <ins>__update the vao each render call__</ins>.
3. |o| define a simple file format for ui elements
4. bitmap file loader
	- caveat: only works for specific file ("media/A_100x100.bmp", 3 byte channel/24bit [RGB])

<br><br>

## 2024-08-29

- file restructuring?
- moved vao+vbo to world object class

<br><br>

## 2024-08-28

### Next time:
- render ui elements with different corodinates on the same render call

### Done
- o ui class
- o drew square
- draw 2 squares

<br><br>

## 2024-08-25/26/27

### 27
 
- middle mouse button to rotate camera
- improved wasd movement
- added vsFormat to .settings file to retain array initilizing formatting
- camera class : moved definition out of class block




### TODOS:
- jupyter notebook
	- script for local to global
	- |o| *multiple points* to detect tranform distortion
- |o| move camera in any direction
	- |o| rotate
- loaders
	- minimal object loader
		- my own color property?
	- bmp image loader
- UI
	- map location and size of element to window
		- necessary for mouse input !
	- render first letter
	- my own file format
- Physics
	- intersection detection
		- start by minimal detection
		- categorize verices/object
			- non-moving object
			- potentially moving
			- always moving 


<br> <br>
## 2024-08-23 - stream

### Comming up
- test and upgrade euler angle rotation
- camera + some pan/rotate
- simple vertex loader from file
- UI
	- specify size and location of element in pixel-units
	- render text
- collision detection


### TODAY
- o : coordinate system - z is up - camera facing +x
- o : add 3d cube
- o : implement euler angles 
- specify size and location of ui element in pixel units


### Since last time
- Added python notebooks for transformation visualizations



<br> <br>

## 2024-08-21 - stream

### Things to do Before/During Next Stream:
- setup intuitive global/inertial coordinates system/ref. frame (right handed, z=up)
- implement minimal euler angles on objects to enable rotations
- camera + some pan/rotate
- simple vertex loader from file
- UI
	- specify size and location of element in pixel-units
	- render text
- collision detection


### TODAY
- o shader-program class/objects - one button UI
- o dynamic simulation, not precalculated closed form


### Since last episode
- Moved from WSL to Ubuntu
	- That made the depth test work
	- window decoratations
- grayscale coloring based on clip-space z-value


<br> <br>

## 2024-08-16 - stream

NEXT SESSION:
- UI shader
- dynamic simulation, not analytic form visualization
- depth test
- projection and view matrix


TODAY:
- o Refactor Simulation object manipulation
- o Rotate object about z-axis
- UI - start simulation
	- create start button
		- UI shader
	- o detect start button click and start sim
- o simulate throw of triangle in 3D
	- bounce on ground
	- INPUT:
		- xv0, vv0, 




## 2024-08-14

TODAY:
- x simulate throw of triangle in 3D
- x UI - start simulation
- o : improve timing cycle
- o : simple perspective
- o : Matrix math


NEXT TIME:
- improve matrix library
	- seperate file
	- consistent objects
- perspective/camera matrices!
	- d
