
# TODO


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
