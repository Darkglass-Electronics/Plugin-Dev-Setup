# Audio Plugin Development Documentation for Darkglass Anagram

This document contains information on how to build your audio plugins for the [Darkglass Anagram](https://www.darkglass.com/products/anagram/),
specifically for Linux hosts.

See [BUILDING-DOCKER.md](BUILDING-DOCKER.md) for instructions on how to build when not using Linux, through the use of docker.

NOTE: This document is a WORK IN PROGRESS! Please bare with us while we set up all the documentation, examples and tools.

### Step 1: Direct toolchain bootstrap

NOTE: This step is meant for Linux hosts, for macOS and Windows see step 1a above.

Simply clone the [mod-plugin-builder](https://github.com/mod-audio/mod-plugin-builder/) git repository, install its dependencies and run the bootstrap script targetting Anagram. Like so:

```sh
# clone MPB
git clone https://github.com/mod-audio/mod-plugin-builder

# install deps, taken from its README file
sudo apt install acl bc curl cvs git mercurial rsync subversion wget \
bison bzip2 flex gawk gperf gzip help2man nano perl patch tar texinfo unzip \
automake binutils build-essential cpio libtool libncurses-dev pkg-config python-is-python3 libtool-bin

# bootstrap toolchain
./mod-plugin-builder/bootstrap.sh darkglass-anagram
```

This will take approximately 1 hour in total, and require around 15Gb of disk space.

### Step 2: Build your own project with the custom toolchain

After having a toolchain we can start building code.  
The mod-plugin-builder repo has a convenient "local.env" shell script that adjusts the local environment to use its custom toolchain, just use it like so:

```sh
# import toolchain setup into current shell
source /path/to/mod-plugin-builder/local.env darkglass-anagram

# setup your project for building
cmake -S /path/to/your/project -B build-anagram

# build your project
$(which cmake) --build build-anagram
```

NOTE: The odd `$(which cmake)` is used here because after "sourcing" the `local.env` file, cmake becomes an alias that sets up a few extra cmake args.

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
