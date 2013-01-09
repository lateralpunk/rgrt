cscie175
========

This project demonstrates a CUDA based ray tracer using a regular grid acceleration scheme.  Timings and error checking are provided for both the CPU & GPU implementations.  There are 9 build files provided which highlight the different aspects of the ray tracer.  The files are regular C++ code located in the build directory.  Since we demonstrate loading of mesh data stored in PLY files, the relevant models are located in the models directory. It is important to run the application from the root directory where the executable is placed in order for the software to find the models.  So always run from the root directory (rgrt-cuda). We also support rendering of rectangles and spheres.  

Here are some useful commands:

1) make all # cleans and makes the project, only really need to do this once.
2) Then there are a bunch of useful make commands that easily run the application with the right command-line parameters for the convenience of the user e.g. make build03-g1, will run build03 (scene) with grid acceleration on.  Other options are described below.

The cuda directory contains all the code associated with the project.  Of most interest to the reader are files:
1) Constants.h
2) GeometricObject.h/inl
3) Grid.h/inl * very important
4) Material.inl
5) RenderOpt.inl
6) ShadeRec.h
7) Sphere.inl
8) World.h/inl * very important
9) cpuraytracer.inl * very very important
10) rgrt-cuda.inl * very very very important

On top of the 9 provided build files, I have allowed for much customization for the user from the command-line.  They can control such things as resolution, whether or not to use the grid acceleration, how many samples to take for anti-aliasing, etc.  Here is a print out of the usage menu (e.g. rgrt-cuda -help)

crystal:rgrt-cuda k5patel$ ./rgrt-cuda -help
USAGE:  rgrt-cuda {-b=build01} {-o=build01.bmp} {-g=1} {-w=400} {-h=400} {-n=1} {-z=1.0} {-m=0} {-s=1} {-p=1} {-help}
All arugments are optional. By default, build01 test scene is executed. 
The defaults are determined by the build scene. 
-b  The build files supported by the sytem and overridden by the ones below.
-o: output (GPU) image saved to disk.  By default the naming scheme:
      "build01_g-1_p-0_wh-600x600_n-1_z-1.0_cpu-64.74_gpumem-1.30_gpu-0.01_gputotal-1.31_error-0.000050.bmp"
      which represents the build name, if grid was used, the resolution, 
      number of samples, zoom, cpu time, gpu memory time, gpu computation time, gpu total time, error.
      NOTE: Your directory may become full of BMPs :)
-g: (0,1) representing whether or not grid acceleration is used. 1 by default. 
-w: width of final image (max=16,384).
-h: height of final image (max=16,384).
-n: number of samples used per pixel for anti-aliasing (integer, max=256).
-z: camera zoom factor (float).
-m: (0,1,2) 0 to render on both GPU & CPU. 1 to render on GPU only. 2 to render on CPU only. 1 & 2 imply no error checking. 0 by default. 
-s: (0,1) 0 to not save to file, 1 to save to file.  Note, that either way, we do create a GPU/CPU buffers to store pixel data so as to be a fair test. 1 by default. 
-p: (0,1) 0 to not use host pinned memory, 1 to use pinned memory.  Default 0. Since lots of our scenes have a lot of mesh data, resonance really chokes on asking for too much pinned memory.
-help: this menu.

Example: ./rgrt-cuda -b=build01 -g=1 -s=0 -m=1

would result in:
-bash-3.2$ ./rgrt-cuda -b=build01 -g=1 -s=0 -m=1
*** Configuration - b: build01, o: auto-gen, g: 1, w: 4000, h: 4000, n: 16, z: 13.00, m: 1, s: 0, p: 0 ****
Using device 0: Tesla C1060
Starting ray-tracing...
Reorganizing Data Structures...
  Not going to use host pinned memory...
  Using Grid acceleration...
    Organizing grid cells...
      Stats: # of cells that have 0 objects, 1 object, 2 objects, etc...
      num_cells = 45
      numZeroes = 0, numOnes = 3, numTwos = 5
      numThrees = 7 numGreater = 30
    Finished grid cell organization.
Finished Consolidation.
World Stats: 
  Total # of GeometricObjects: 307
  Approximate bytes of data: 49624 bytes
Starting GPU ray tracing...
  # of threads in a block: 16 x 8 (128)
  # of blocks in a grid  : 250 x 500 (125000)
  GPU memory access time: 143.837997 (ms)
  GPU computation time  : 5233.520996 (ms)
  GPU processing time   : 5377.358887 (ms)
Finished ray-tracing...

This means we requested build01, with an automatic filename output (BMP), with grid, 4000x4000 and 16 samples per pixel, zoom of 16, mode of 1 (meaning only render on GPU not CPU), s=0 don't save the file output since we don't want to generate lots of unnecessary BMP and are just interested in the runtime, and finally with no pinned memory being used.

So you should play with m=1, different resolutions, zoom level, and sample size to try out variety of tests.  Also running one of the mesh builds with p=1 on Resonance would show you that it performs worse than p=0.
