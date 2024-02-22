# GodotCppCMakeTemplate

Easy to use starting point for Godot C++ workflows using CMake.

You can easily add this to any existing project you are currently working, it does not pollute your godot project files so it is also easy to remove if you decide you do not need C++.

### How to use

You will need Git and CMake.

- Create Godot project or use an existing one, set up default scene and exit Godot editor.

- In command line or terminal, navigate to a location where you want to clone this repository, it can be inside your godot project but I suggest putting it nearby outside the godot project directory.

- Clone this repository.<br>
```
git clone https://github.com/Noxagonal/GodotCppCMakeTemplate.git
```

- Generate project files, you will need to provide path to the Godot executable you'll be using with your project, as well as path to the godot project directory. Replace the paths in below example, I recommend absolute paths for now.<br>
```
cd GodotCppCMakeTemplate
mkdir build
cd build
cmake -D GODOT_EXECUTABLE_PATH="<path to godot executable>" -D GODOT_PROJECT_FOLDER_PATH="<path to godot project root>" ..
```

- Open the generated project or solution file in ```build``` directory and start debugging like in any other application, that's it. In Visual Studio, pressing F5 or starting a debug session will open the default scene and start the game, insert breakpoints where-ever you need them. Editing code and re-launching, will immediately recompile and apply the changes. I provided a simple SampleNode class to get started, the sources can be find in the ```src``` folder/filter.

### Things to consider

- There is a bug in godot-cpp ```CMakeLists.txt``` that makes ```release``` version on Visual Studio throw a lot of link errors because it uses the wrong runtime library. As a workaround, find ```submodules/godot-cpp/CMakeLists.txt``` file, in that file, remove or comment out this line ```set(GODOT_COMPILE_FLAGS "${GODOT_COMPILE_FLAGS} /MDd") # /Od /RTC1 /Zi```. It should be on line 88. This will make the release version build properly. Problem is that the compile flags are determined at configuaration time, not at compile time which is needed for Visual Studio, so debug version of the runtime libraries are incorrectly used always, these flags are redundant so removing them doesn't hurt.
- A new ```bin``` directory is created in your Godot project root, it typically has 2 or 3 files. If you want to remove C++ from your project, just delete the ```bin``` directory.
- Debug version of your generated code is used in editor, and only the debug version. Release version is only required when you export release version of your project.
- You'll need to compile your code on every platform you wish your application to be used.

### Future work

Contributions and critique welcome. I only worked on this on VS2022 on Windows, so there are some bits here and there that require some ironing out. For example "x86-64" is hard coded so, Arm isn't possible yet.

---

&copy; 2024 - Niko Kauppi (Noxagonal)
