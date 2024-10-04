
# 2024-10-04 - Reflection on completion
- The inital thought from last week were good AND flawed
    - The falwed part was that the inital brainstorm didn't generate the structure for immediate implementaiton, but
    - the good part was that I essensially formed all the constituen parts more clearly in my head.
- The refactoring too place over a few days following the initial brainstorm, and took a completely different shape that originally planned
- SPECIFICS:
    - I originally planned a linear pipeline
        - scene -> worldobject -> model -> renderer -> vao -> vbo -> gpu call
    - instead it turned out that it made more sense to make a connecting world object like so
        - scene -> world object -> model -> vao+vbo
        -                       -> transform 
        -                       -> render
- takeaway:
    - "Consistent incremental improvement is the fundamental action of anything worthwhile."
    - I set the stage a week ago, but the result came from the cumultaive effort over multiple days.
    - Just keep trying to refine your thought and ideas, and you'll get there.
        - "Better to get your dopamine from improving your ideas than from having them validated"
            - Nat Friedman (https://nat.org/)

<br>



# 2024-09-27

## Manage the gl-interface variations
Currentl yi've got several different types of vectex-formats, file formats, and in-code formats. 

One solution would be to only keep one, making the rendering pipline sraight forwarfd.

Another solutoin would be to give each rendering type their own name/types.

I prefer the second one.

### Implementation plan : RENDPIPE = api for the librended !
- world objects creation has to have stricter boundaries! Specifying vertex data format, coloring/texturing, shader, model information source(file, filytype, etc.) has to be - ENFORCED
    - ENUMS:
        - Model source
            - in-code
            - file
                - pos
                - obj
            - SRC-Enum
                - code
                - pos
                - obj
        - vertex data formats:
            - naming convension:
                - p = position coordinates
                - n = normal coordinates
                - t = texture coordinates
                - c = color coordinates
            - VDF-Enum:
                - p3c3 (3 positions and 3 colors per vertex [learn-opengl tutorial example])
                - p3n3t2 (3 positions, 2 texture coordinates, and 3 vertex normals per vertex [obj-file ])
        - shaders
            - Sturcture
                - each shader needs to be supporting only a perticular type of vertex data format
            - SHADER-Enum:
                - worldShader
                - worldObjShader
                - wireframeShader
                - UI shader
- pipeline variations:
    - each pipeline needs each of one of the enums above
    - use a struct to hold a combination of enums
    - RENDPIPE-Enum
        - learnopengl (p3c3 + worldShader + code)
        - wireframe_primitive (p2 + wireframeShader + code)
        - wireframe_model (p2 + wireframeShader + model) [requires creating wireframe mesh unles there is a wireframe file for the model]
        - worldObject (p3n3t2 + worldObjShader + model)
        - worldObject_noLight (p3n3t2 + worldObjShader + model)
        - ui_string
        - ui_texture
    - be able to switch pipline during runtime
        - e.g. worldobject --> worldObject_noLight
- Each new WorldObject has to initilize a renderer of RENDPIPE-Enum-type
- RENDPIPE source object chain:
    - main-init  (physimos) -> scene (libscene) -> worldObject (libscene) -> renderer(librender) -> rendcore (librend) -> opengl
    - main-update           -> scene-update     -> worldObject
    - main-render           -> scenete-rendere  -> worldObejct.rend()      -> renderer.rend()   

### Additional considerations
- models transforms
    - math interface
    - inherited object transforms
        - parent/child relationship for worldObjects

# 2024-09-28
- World object : pointers to objects kept
    - scene
    - parent object
    - children objects
    - model
    - transform
    - renderer/shader
    - rigidbody?
- WorldObject : key methods
    - constructor
        - requires all parameters to set up a minimally viable rendered object (pointers to objects, or enum representing a specific configuration)
            - model (probably as string name / enum value)

UPDATES:
- worldscene:
    -
