#Introduction
---

GAT350, or Computer Graphics is a class oriented on teaching the different ways we can render graphics with a basic framework.

There are two main parts to this solution the Software Rasterizer and the Ray Tracer.



## Software Rasterizer

A high-performance software rasterizer implemented in C++ with OpenGL integration, designed for educational purposes and to demonstrate the fundamentals of rendering pipelines.
It demonstrates how we can use the CPU-based rasterization process to mimic the actions of a GPU rendering pipeline.

---

## Features

- **Custom Rasterization Pipeline**: Implements the essential stages of a rasterization pipeline (vertex processing, clipping, rasterization, and fragment shading).
- **OpenGL Interoperability**: Leverages OpenGL for window management and optional GPU-assisted operations.
- **Shader Support**: Includes basic vertex and fragment shader simulation.
- **Viewport Transformation**: Maps 3D coordinates to 2D screen space with configurable viewports.
- **Z-Buffering**: Ensures correct depth handling for visible surfaces.
- **Importing models**: Supports importing models with a .obj.
- **Post Processing**: Supports various post processing effects such as Edge Detection and Sharpen


![alt text](https://github.com/crazyman222214/GAT350-----Computer-Graphics/blob/master/Build/SoftRastLighting.png)
![alt text](https://github.com/crazyman222214/GAT350-----Computer-Graphics/blob/master/Build/Edge.png)
![alt text](https://github.com/crazyman222214/GAT350-----Computer-Graphics/blob/master/Build/Sharpen.png)



## Ray Tracing

## Features

- **Physically Based Rendering (PBR)**: Simulates realistic lighting using ray tracing techniques.
- **Ray-Object Intersection**: Efficient algorithms for calculating intersections with spheres, planes, and triangles.
- **Shading Models**: Implements Lambertian (diffuse), metal, dielectric, and emissive shading models.
- **Reflection and Refraction**: Simulates light behavior on reflective and transparent surfaces using Snell's law.
- **Shadows and Soft Shadows**: Supports shadow rays for accurate shadow computation with optional soft shadows.
- **Global Illumination**: Includes path tracing for realistic lighting and color bleeding.

![alt text](https://github.com/crazyman222214/GAT350-----Computer-Graphics/blob/master/Build/coolballs.png)
![alt text](https://github.com/crazyman222214/GAT350-----Computer-Graphics/blob/master/Build/CornellBox.png)

  
## Installation

The required dependencies are 
- SDL
- STB
- OpenGL/GLM (Open GL Mathematics)


## Attribution

- SDL: [https://www.libsdl.org](https://www.libsdl.org)
- STB: [https://github.com/nothings/stb](https://github.com/nothings/stb)
- OpenGL: [https://www.opengl.org](https://www.opengl.org)


  
