
## 3D Viewer

#### This program is designed to view 3D wireframe models (3D Viewer) in the C programming language.

*Models are loaded from .obj files and displayed on the screen with the ability to rotate, scale and move.*

- The program developed in C language of C11 standard using gcc compiler, using QT libraries and modules.
- The program provide the ability to:
    - Load a wireframe model from an obj file (vertices and surfaces list support only).
    - Translate the model by a given distance in relation to the X, Y, Z axes.
    - Rotate the model by a given angle relative to its X, Y, Z axes.
    - Scale the model by a given value.
- The graphical user interface contain:
    - A button to select the model file and a field to output its name.
    - A visualisation area for the wireframe model.
    - Button/buttons and input fields for translating the model.
    - Button/buttons and input fields for rotating the model.
    - Button/buttons and input fields for scaling the model.
    - Information about the uploaded model - file name, number of vertices and edges.
- The program allow customizing the type of projection (parallel and central)
- The program allow setting up the type (solid, dashed), color and thickness of the edges, display method (none, circle, square), color and size of the vertices
- The program allow choosing the background color
- Settings be saved between program restarts
- The program allow saving the captured (rendered) images as bmp and jpeg files.
- The program allow recording small screencasts by a special button - the current custom affine transformation of the loaded object into gif-animation (640x480, 10fps, 5s)