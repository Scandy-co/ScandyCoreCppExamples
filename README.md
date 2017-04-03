# ScandyCoreCppExamples
A set of examples of how to use Scandy Core in a headless Cpp application

# Setup

## Step 1: C++ compiler

* OSX: install Xcode (sorry there's no handy one-liner here...)
* Ubuntu: `sudo apt-get install build-essential`

## Step 2: CMake

* OSX: `brew install cmake`
* Ubuntu: `sudo apt-get install cmake`

## Step 3: Scandy Core SDK

You should install your copy of the Scandy Core SDK into
`dependencies/ScandyCore`. See `dependencies/README.md` for more information;
Briefly, for example, if you've downloaded the ScandyCore package for MacOs, do this:
```
pushd dependencies/
mv ~/Downloads/ScandyCore-0.0.3+1690.1bec63f-Darwin.zip ./
unzip ScandyCore-0.0.3+1690.1bec63f-Darwin.zip
ln -s ScandyCore-0.0.3+1690.1bec63f-Darwin ScandyCore
popd
```

# Building

```bash
mkdir -p build
cd build
cmake ../
make
```

# Running

**NOTE:**
OSX you will need to update your `DYLD_LIBRARY_PATH` path:

```bash
export DYLD_LIBRARY_PATH=$PWD/../dependencies/ScandyCore/lib/
```
*from the `build` dir*

Then on OSX or Linux:

```bash
./basic_usage
./basic_usage_mesh_read
./basic_usage_file_read
```
*from the `build` dir*
