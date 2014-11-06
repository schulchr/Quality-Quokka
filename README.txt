All library header files are located in the include directory (GLEW, GLFW/GLUT, GLM/Angel.h). 
All libraries (lib/dll) are located in the lib directory. 
All source files (your .cpp and .h/.hpp files) are located in the src directory.

When building the project on a lab computer, the binary (.exe) built from the Visual Studio setup project may not be executable due to insufficient permissions. To fix this, The binary and dlls in the build directories need to be given executable permissions for the user. The easiest way to do this is to right-click on the root directory of the project (setup-*), Properties, Security, Edit, select user name, check Read & execute, ok.

If Windows complains that it can't find a DLL file when trying to run they built project, make sure the DLL files in the lib directory to your build directory (Debug/Release).
