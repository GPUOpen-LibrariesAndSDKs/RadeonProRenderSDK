

# AMD Radeon:tm: ProRender SDK

![](Resources/doc/doc1.png)

AMD Radeon:tm: ProRender is a powerful physically-based rendering engine that enables creative professionals to produce stunningly photorealistic images.
https://www.amd.com/en/technologies/radeon-prorender

This repo contains :
- the AMD Radeon:tm: ProRender library: includes and built binaries.
- some tutorials demo samples.

## Building and running the Tutorials

On Visual Studio:
```
> cd tutorials
> ..\premake5\win\premake5.exe vs2019
then open tutorials/Tutorials.sln
```

On Linux:  
Dependencies on Linux for the tutorials: GLEW, GLUT, Pthread.
```
> cd tutorials
> sudo chmod +x ../premake5/linux64/premake5
> ../premake5/linux64/premake5 gmake
> make -j config=release_x64
```

On MacOS:
```
> cd tutorials
> sudo chmod +x ../premake5/osx/premake5
> ../premake5/osx/premake5 gmake
> make -j config=release_x64
```

To run a demo, just select the executable and start it, example:
```
> cd tutorials/Bin
> ./05_basic_scene64
this demo generates the rendering output images: 05_00.png, 05_01.png, 05_02.png, 05_03.png
```

See detailed documentation at 
https://radeon-pro.github.io/RadeonProRenderDocs/sdk/tutorials.html

## Additional Info

- Radeon:tm: ProRender GPUOpen web site
https://gpuopen.com/radeon-pro-render/

- Radeon:tm: ProRenderSDK documentation
https://radeon-pro.github.io/RadeonProRenderDocs/sdk/about.html



