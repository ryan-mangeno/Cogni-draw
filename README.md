# Cogni-draw

AI-Driven 2D to 3D Sketching Application

Cogni-draw is an interactive prototype application that leverages AI techniques to convert 2D sketches into rudimentary 3D representations. By right-clicking and dragging over the framebuffer, users can capture their sketch to save them as PNG images to then be transformed into a 3D model, viewable in the model viewer dock.

---

## Features

- **Region Capture**: Right-click and drag to select a rectangular region of the framebuffer, which is then saved as a PNG image.
- **AI Export**: ChatGPT api with 4o-mini and then dalle, working on segmentation api, might use meshy
- **Note**: I can download a model directly from meshy's website and this works, I have the api script for meshy, but need to buy tokens to use it. The demo down below is a proof of concept for now.
## Prerequisites

- Just MSVC for now
- [GLFW](https://www.glfw.org/) (for window and input management)
- [GLAD](https://github.com/Dav1dde/glad) or another OpenGL loader
- [stb_image.h](https://github.com/nothings/stb/blob/master/stb_image.h) (for image loading)
- [stb_image_write.h](https://github.com/nothings/stb/blob/master/stb_image_write.h) (for PNG output)
- [stb_image_resize.h](https://github.com/nothings/obbg/blob/master/stb/stb_image_resize.h) (for image resizing)

## Demo I/O (stb save -> o4-mini vision -> dalle -> meshy(not included yet) -> OpenGL
![demo](https://github.com/user-attachments/assets/12c550a5-b08e-49d3-a131-dd064d4ae018)

https://github.com/user-attachments/assets/3ab01c99-9fdb-47bc-91bb-3cba51efd5b3


## Building 

1. Clone the repository:
   ```bash
   git clone https://github.com/anakin004/Cogni-draw.git
   cd Cogni-draw
