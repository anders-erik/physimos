

# Features

## Scene

- || scene objects
	- || scene_object (minimal rendered object)
		- name
		- mesh
		- texture
		- transform
	- physics object 
		- can bind a scene_object, generate a bounding box, and use a generated bounding box to update the scene_objects transform
		- vectors to show velocity, acceleration, forces, rotation, etc.
	- physics field
		- types
			- gravity
			- electric
			- etc.
		- render vectors
			- vary arrow density 
			- arrow color
			- arrow size
	- scene time object (from timing utility)
		- absolute elapsed time
		- frames rendered
		- physics updates
		- etc.