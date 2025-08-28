# Audio Plugin Development Documentation for Darkglass Anagram

This document contains information on how to build your audio plugins for the [Darkglass Anagram](https://www.darkglass.com/products/anagram/),
specifically for non-Linux hosts like macOS or Windows, through the use of docker.

See [BUILDING-LINUX.md](BUILDING-LINUX.md) for instructions on how to build when using Linux.

NOTE: This document is a WORK IN PROGRESS! Please bare with us while we set up all the documentation, examples and tools.

### Step 0: Install docker

TODO
either docker desktop or alternative (must be compatible with `docker buildx` command)

### Step 1: Build the docker image

To start we build the docker image that contains the toolchain, host tools and extra libraries.
The Dockerfile can be found [here](docker/Dockerfile).

Assuming you have docker installed, you can build it like this:

```sh
docker buildx build \
    --build-arg debug=false \
    --build-arg target=full \
    --tag "darkglass-anagram" \
    /path/to/Plugin-Dev-Setup/docker
```

The debug argument is optional, default is false but can be set to true if needed.
We recommend to name the image with a debug suffix (e.g. `--tag "darkglass-anagram-debug"`) to easily differentiate between release and debug builds.

The target argument is also optional, has the possible choices:
 - full (builds everything, toolchain and all libs)
 - minimal (builds minimal libs only, includes host-cmake, fftw and liblo)
 - kernel (builds libs needed for the Linux kernel)
 - toolchain (builds toolchain only, no extra libs or host tools)

The full build will take approximately 1 hour in total, and require around 15Gb of disk space.

You can verify that you have a valid docker image by running `docker images`. Your output should contain "darkglass-anagram", for example:

```
% docker images
REPOSITORY          TAG       IMAGE ID       CREATED          SIZE
darkglass-anagram   latest    782ca98b7eff   41 minutes ago   14.6GB
```

### Step 2: Run the docker image mounted with your local files

After creating a docker image we need to create a container based on that image, so we can dynamically run things from it.
Because it is a container, it has no direct access to the host filesystem, so we need to map between a folder on the host and the container.

To create a container, run the docker command like this:

```sh
docker run \
    --name "anagram-container-name" \
    -ti \
    -v /path/to/your/project:/root/source \
    "darkglass-anagram:latest"
```

The `--name` argument specifies a container name to use, change it to whatever makes more sense for you.

The `-ti` argument combines `t` for enabling a "TTY" and `i` for making it interactive.
These 2 arguments together is what allows us to run commands inside the container.

The `-v` argument specifies a host folder to mount inside the container folder, using ":" as separator between host and container paths.
This argument can be used multiple times to mount multiple folders.
The container folder does not need to exist beforehand, docker creates it as needed.

Finally the `"darkglass-anagram:latest"` specifies the image name from which to create the container from.

The first time you run this command you will be dropped into a bash shell inside container.
You can close it at anytime with Ctrl+D.
Running it again is possible with `docker start -i "anagram-container-name"` (adjusting name as needed).

### Step 3: Build your own project with the custom toolchain

#### Example using cmake

```sh
# setup your project for building (-S specifies cmake project folder, -B specifies build output folder)
cmake -S /path/to/your/project -B build-anagram

# build your project
$(which cmake) --build build-anagram
```

NOTE: The odd `$(which cmake)` above is required; the build environment uses `cmake` as an alias with a few extra args.

#### Example using raw makefiles

Simply do:

```sh
# build your project (-C specifies the project build folder)
make -C /path/to/your/project
```

### Step 3: Deploy plugin

Assuming you have an Anagram unit running in Developer Mode, you can just do:

```sh
# cd to where your lv2 bundles are located
cd /path/to/your/build/output/dir

# copy over lv2 bundles
scp -O -r *.lv2 root@192.168.51.1:/root/.lv2/

# restart system services that use plugins
systemctl restart jack2 lvgl-app
```
