libtiny3d : 3D Software Renderer Library
=======================================

A lightweight 3D graphics engine built from scratch using C without using OpenGL or DirectX libraries. This project implements a full 3D rendering pipeline using mathematical primitives and a custom floating-point canvas. Built as part of the CO1020: Computer Systems Programming course at the University of Peradeniya.

Project Structure
-----------------
````
libtiny3d/
├── include/             
│   ├── canvas.h
│   ├── math3d.h
│   ├── renderer.h
│   ├── lighting.h
│   └── animation.h
│
├── src/                 
│   ├── canvas.c
│   ├── math3d.c
│   ├── renderer.c
│   ├── lighting.c
│   └── animation.c
│
├── tests/            
│   ├── test_math.c
│   └── test_visual.c
│
├── demo/                
│   └── main_soccerball.c
│
├── build/               
│   └── demo.mp4
│
├── documentation/
│   └── GroupXX_report.pdf
│
├── README.md
└── Makefile
````

Building the Project
--------------------

Use the provided Makefile to compile and run different components:

`make all`
    Builds all executables: soccer_ball, test_math, test_visual.

`make image`
    Runs the soccerball executable and generates the soccerball video.
    Converts clock_face_static.pgm to clock_face_static.png.
    Cleans up soccerball frames and mp4, keeping only the PNG.

`make cube_video`
    Runs the cube executable, generates frames, and creates cube_rotation.mp4.

`make soccerball_video`
    Runs the soccerball executable and creates soccerball_rotation.mp4.

`make visual_video`
    Runs test_visual and creates visual_rotation.mp4.

`make videos`
    Runs all video generation commands (cube, soccerball, visual).

`make clean`
    Removes build artifacts, executables, .pgm, .png, and .mp4 files generated.

Features
--------

1. Canvas & Line Drawing
   - Sub-pixel floating-point canvas (canvas_t)
   - Bilinear filtering for smooth pixel blending
   - DDA line drawing with adjustable thickness

2. 3D Math Library
   - Vector and matrix operations
   - Cartesian and spherical coordinate conversions
   - Fast normalization using inverse square root
   - Quaternion support and SLERP interpolation

3. Software Rendering Pipeline
   - Full transformation stack: Model -> View -> Projection -> Screen
   - Depth-sorted wireframe rendering with circular clipping
   - Procedural or file-based 3D mesh loading (truncated icosahedron)

4. Lighting & Animation
   - Lambertian shading based on edge and light angle
   - Multiple light sources
   - Cubic Bézier curve animation for smooth, synchronized motion

Demo Output
-----------

- Clock Face: Static line drawing in 15° intervals from canvas center
- Rotating Cube: Manual transform demo via test_math.c
- Soccer Ball: Animated wireframe icosahedron
- Visual Test: Multiple moving, lit shapes with sync

Dependencies
------------

- ANSI C (C99)
- GNU Make
- No external libraries used
- ffmpeg required for video conversion (PGM to MP4)

Contributors
------------

- Vidusini Abesekara (E/23/001)
- Thenuk Piyathilake (E/23/274)


AI Tool Usage
-------------

This project made limited use of AI tools to assist with development and documentation:

- **ChatGPT** was used for code explanations, algorithm suggestions, and structuring this README.
- **DeepSeek** was used to generate code snippets and optimize mathematical functions during development.

All AI-generated content was carefully reviewed, modified, and tested to ensure complete understanding and correctness. The final submission reflects the author's own learning, implementation, and decision-making throughout the project.

