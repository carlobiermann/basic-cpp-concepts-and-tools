# Benchmarking your C++ code


This directory features various code examples from the following videos/talks:

- The Cherno's [C++ benchmarking video](https://www.youtube.com/watch?v=YG4jexlSAjc), 
- The Cherno's [visual C++ benchmarking video](https://www.youtube.com/watch?v=xlAH4dbMVnU)
- Klaus Iglberger's [CppCon 2019, Back to Basics: Move Semantics talks](https://www.youtube.com/watch?v=St0MNEU5b0o)

For these programs I mainly used CLion 2022.2.4 on an early 2015 MacBook Pro (2.9 GHz, 8 GB RAM) running macOS Monterey.
I used a docker image based on `DockerfileCppBuildEnv` from my [docker-dev-environments repository](https://github.com/carlobiermann/docker-dev-enviroments)
to compile the programs in a Docker container in CLion. If you want to find out how to do the same, take a look at 
the CLion documentation on [CLion toolchains in Docker](https://www.jetbrains.com/help/clion/clion-toolchains-in-docker.html#create-docker-toolchain).

### 1. simple-scoped-timer
#### 1.1 How to build and run

**In CLion**

- Right-click on `simple-scope-timer/CMakeLists.txt` and select `Load CMake Project`
- Build and run the program

**From command line**
```bash
$ cd benchmarking-your-code/simple-scoped-timer/
$ mkdir release-build
$ cd release-build/
$ cmake -DCMAKE_BUILD_TYPE=Release .. && make
$ ./simple-scoped-timer
```

### 2. advanced-profiling
#### 2.1 How to build and run

**In CLion**

- Right-click on `advanced-profiling/CMakeLists.txt` and select `Load CMake Project`
- Build and run the program

**From command line**
```bash
$ cd benchmarking-your-code/advanced-profiling/
$ mkdir release-build
$ cd release-build/
$ cmake -DCMAKE_BUILD_TYPE=Release .. && make
$ ./advanced-profiling # This will generate a results.json file in the build directory
```

#### 2.2 Viewing the results
- Open Chrome and enter `chrome://tracing` in the searchbar
- Load the `result.json` file

### 3. move-semantics
#### 3.1 How to build and run

**In CLion**

- Right-click on `move-semantics/CMakeLists.txt` and select `Load CMake Project`
- Build and run the program

**From command line**

```bash
$ cd benchmarking-your-code/move-semantics/
$ mkdir release-build
$ cd release-build/
$ cmake -DCMAKE_BUILD_TYPE=Release .. && make
$ ./move-semantics 
```
