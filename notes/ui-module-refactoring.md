# Start of refactoring - 2024-10-04

- "Better to get your dopamine from improving your ideas than from having them validated"
    - Nat Friedman (https://nat.org/)

## Plan:
- currently my ui consists of single ui-element/strings
- I have no interactive capabilites beyond toggle a specific feature by setting a predefined name/behavior
- The new plan is to create uints/modules that will provide predefined ui-interactivity with commonly occuring objects, such as scenes, worldobjects in a scene, and simultors
- The first module will be a list of worldobjects with the capability to toggle certain features, such as vilibility, renedere, isactive, rigidbody, and type of transform realtiev eto parent object

## Things to resolve:

### Levels of abstraction:
-CURRENT PLAN:
    - element -> unit -> module -> ui
        - UI is made up of modules, which consists of units and/or elements
- PROBLEM:
    - I don't know what level of abstrction is appropriate
    - I won't have the answer until i've started trying.

### FIRST BRAINSTORM

- Levels OF abstraction and their properties:
    - primitive list:
        - textured rectangle [includes simple colored squares, chars, etc.]
            - i.e. everything is a textured rectangle
            - all ui tree leafs are primitives
            - 
    - element
        - use multiple primites to create a visualy reasonable ui element
        - list:
            - string label or input [list of chars with interactive properties], checkbox [e.g. 2 squares that can change colors], range [number value with interactive +- toggle]
        - Properties: texture (plain colors are trated as textures for cleaner shader code), 
        - elmeents are the basic building blocks that capture properties of object
    - unit list:
        - worldobject listrow
        - worldobject listrow-with-toggles
        - worldobject property label row 
            - e.g. displays transform.position.x
        - worldobject property input row
            - e.g. be able to change transform.position.x using text
        - 
- RENDERING:
    - FUNDAMENTAL QUESTION:
        - AT WHAT LEVEL OF ANSTRACTION DO WE START BATCHING?
            - do we perform a gpu-draw-call for every
                - primitive?
                - element?
                - unit?
                - module?
                - UI?
            - by making the actual draw call at the level of the primitive we get full control of each draw, BUT IT MIGHT FILL THE GPU BANDWIDTH FAILY QUICKLY
            - BUT THE NUMBER OF UI ELMEENT WONT BE *THAT* GREAT
                - but we need to branch somewhere, unless we place the gpu-call in the ui primitive, then we get a pure and simple 2D texture shader.
            - It makes more sense to maybe render each element, where the element consists of many member functions that performs the branching. Maybe one method for each element type, OR
                - e.g. UI::Elememt.render() -> switch the different element types
            - maybe using an init/update method to set the element children primitve properties. Then  one identical render method loops through the primitives.
- LEVEL OF ABSTRACTION CONCLUSION:
    - `Primites` should be used for cleaner shaders, The **ATOM OF RENDERING** of the ui [only a rectangle with texture is used!][maybe a separate debug shader!]
    - `Elements` are used to deal with an easliy overviewable amount of primitives. **THE ATOM OF INTERACTION** [string-elements]
    - `Units` enables interaction with **ONE DIMENSION OF A PHYSIMOS OBJECT** [toggleable properties, model property changer]
    - `Modules` encapuslates a significant part of **APP UTILIY** [scene objects, full interactivity with one object, navigate simulators, scene settings, etc.]

<br>

- CONCLUSION:
    - <u>Shader only render primitives</u>, which means textured rectangles.
    - <u>Elments </u> responsibility is to organize the location of <u>1) a collection of primitives</u>, <u>2) and provide click-event notification</u>.
    - The units orchestrate <u>a collection of useful **elements** for a physimos objects</u>.
    - The UI-modules are designed to capture and provide the user with <u>meaningful global interactivity</u>.


### Practical notes after first brainstorm:
- Module:
    - is contained within a module-container
        - the container is drawn using a single primitive: the module-container-primitive
            - aka. modConPrim
    - Recieves a collection of physimos objects/properties
    - Example naming for WOToggleList:
        - WOToggleList name:
            - <listname>_WOToggleList
                - e.g. primary_WOToggleList
        - WOToggleUnit name:
            - <ParentModuleName>_<WorldObject.name>
                - e.g. primary_WOToggleList_house1_obj
        - Elementname within unit:
            - <ParentUnitName>_<ElementType>_<name> 
                - primary_WOToggleList_house1_obj_ShaderToggle
                    - this is the deepest click detection?
                    - the click events will loop through element container coordinates/dimension to check for matches

