# Image Reader and Viewer

This project is a C++ image reader and viewer for OpenEXR and other formats, using OpenImageIO and OpenEXR libraries. It provides command-line parsing, image processing, and a GLUT-based OpenGL viewer.

## Features
- Read images using OpenImageIO
- Write images using OpenImageIO
- Basic image processing (vertical flip, pixel access)

## Dependencies
- **OpenEXR 2.5.8**
- **OpenImageIO 2.4.17**
- **OpenGL** and **GLUT** (for viewer)

## Build Instructions

1. **Install and build dependencies:**
   - NOTE - not all versions of OpenEXR and OpenImageIO play nicely together. The ones listed above should work
   - Follow Dr. Tessendorf's [guide to building openexr and OpenImageIO](https://people.computing.clemson.edu/~jtessen/cpsc6040/html/lectures/file_formats/build_install_openexr_openimageio_starter_recording.mp4) 

2. **Clone this repository:**
   ```sh
   git clone <repo-url>
   cd hw_01_image_reader
   ```

3. **Build the project:**
   ```sh
   make
   ```

## Usage

```sh
./imgviewer -image <image_file> [options]
```

- `-image <image_file>`: Path to the image file to display.
- `-t`: Runs unit tests and exits.

## Controls
- pressing the "j" key will save a copy of the image. 

## Notes
- Make sure your `Makefile` includes the correct `-I` and `-L` flags for your OpenEXR and OpenImageIO installation paths.
- Tested with OpenEXR 2.5.8 and OpenImageIO 2.4.17.
