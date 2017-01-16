# bfx64 - *The 64-Bit Object File Extractor*

This readme contains information about the **bfx64** extractor, along with usage and setup details.


##What is bfx64?
bfx64 is a `C` and `C++` fact extractor that analyzes compiled object files to generate a picture of the underlying source code. From a collection fo input object files, bfx64 will analyze each, resolve references, and output a lightweight but powerful program model that can be visualized or queried. With few program options, bfx64 is easy to use while remaining useful for a large number of analysis scenarios.

bfx64 is based on the orginal **bfx** extractor created by Jingwei Wu at the University of Waterloo in 2004. The link for information about bfx can be found [here](http://www.swag.uwaterloo.ca/qldx/). While bfx64 was created with a similar data schema to bfx, bfx64 adds support for 64 and 32 bit object files as well as object files that have little or big endianess. In addition, bfx64 generates a complete linked model from the getgo whereas bfx requires additional model querying to generate the final linked model.

While ClangEx is developed to extract facts to a program model, users must download additional tools from the [Sofware Architecture Group (SWAG)](http://www.swag.uwaterloo.ca) at the Univesity of Waterloo to visualize or query their programs. This guide wil cover how these tools are configured in the "setup" section.

### Features of bfx64
Currently, bfx64 supports the following features:

* Support for both `C` and `C++` languages compiled into object files using the latest Executable and Linking Format (ELF) standard.
 
* Partial program analysis. For instance, users can choose to only analyze two of the ten object files in their project. bfx64 will resolve references between these two files but do no more.

* Support for some language features such as functions, variables, and classes.

### bfx64 Metamodel
//TODO

### When Should I Not Use bfx64?
While bfx64 is great for quick, partial analysis that fits nicely into existing workflows, there are many cases that make bfx64 disadvantageous for analysis. First, if your `C` or `C++` program is not compilable, bfx64 is not for you. This means source code needs to be compiled into object files before analysis can begin. However it **DOES NOT** mean that linking needs to succeed.

The second disadvantage is that, by analyzing object files, a lot of language features in both `C` and `C++` are glossed over. If you look at the schema, you may notice that there isn't much data captured in the way of variables and functions and that features like structs, enums, and class inheritance isn't captured at all. bfx64 tends to be very quick for analysis but at a cost of skipping over features. Note that in future versions of bfx64, these features are still expected to be missing since object files simply don't capture the type of information necessary.

##Installation Details
###Prerequisites
bfx64 uses `CMake 3.0.0` or greater to run as well as two external libraries; `ELFIO` for ELF format processing and `Boost 1.5.8` or greater for file processing. As such, if you're missing any of these requirements, bfx64 will not build from source until these prerequisties are satisfied. This section will cover how to properly install each of these prerequisites on your system. If you satisfy them, feel free to skip over.

####Installing CMake
First, CMake should be installed. On Linux, this is as simple as running:
```
$ sudo apt-get install cmake
```
This will install the most current version of CMake based on your sources list. Be sure to check the version to ensure you have a version of CMake greater than `3.0.0`. This can be done by running:
```
$ cmake --version
```

---
If you want the most current version of CMake, this will require a bit of additional work. First, download the most recent version from the [CMake website](http://www.cmake.org) and download the Linux source version. The commands below show how CMake is installed from the `3.7.0` Linux source version. Change the version label in the download link to download a different version.

First, we download and unzip the source files:

```
$ wget https://cmake.org/files/v3.7/cmake-3.7.0.tar.gz
$ tar xvzf cmake-3.7.0.tar.gz
$ cd cmake-3.7.0.tar.gz
```

Next, install CMake by configuring the Makefile and running it. This can be done by doing the following:
```
$ ./configure
$ make
$ make install
```
That's it! You can check if CMake is correctly installed by running:
```
$ cmake --version
```

####Installing Boost Libraries
Boost libraries are also required. This process is very simple on Debian/Ubuntu systems.

Simply run the following command to download and install Boost libraries to your system's `include` path:
```
$ sudo apt-get install libboost-all-dev
```

**IMPORTANT NOTE:** Boost libraries are also needed on your system *even if* you are simply running the executable built on another system. Follow the instructions above to get the necessary Boost libraries to run the portable executable.

####Installing ELFIO Libraries
ELFIO libraries are also required to compile bfx64. Unlike Boost, there is a little bit more work required before ELFIO is installed.

First, run the following command to download ELFIO 3.2 from the internet and unzip it to your current directory:
```
$ wget https://downloads.sourceforge.net/project/elfio/ELFIO-sources/ELFIO-3.2/elfio-3.2.zip
$ gunzip elfio-3.2.zip
```

Next, go to the ELFIO directory and run the `./install-sh.sh` script. This will copy all required ELFIO headers into your `/usr/include` directory. ELFIO can now be included in any `C++` project of your choice. To do this run the following:
```
$ cd elfio-3.2
$ ./install-sh.sh
```

That's it! The `./install-sh.sh` should have installed ELFIO properly.

###Building bfx64
Now that the prerequisties are all satisfied, you can now download and build bfx64! If all prerequisties are truly satisfied, bfx64 should build without issue.

First, we must checkout the current version of bfx64 from GitHub. This will be downloaded to your current working directory. The bfx64 repository has all required files and libraries.

To download, run the following:
```
$ git checkout https://github.com/bmuscede/bfx64.git
```

Next, we want to build the source code. This guide will build bfx64 to the `bfx64-Build` directory that is adjacent to the `bfx64` library. If you want to build to a different directory, replace the following `bfx64-Build`'s in the following commands to the directory of your choice.

To build, run the following command:
```
$ mkdir bfx64-Build
$ cd bfx64-Build
$ cmake -G "Unix Makefiles" ../bfx64
$ make
```

To verify that bfx64 built, ensure the `bfx64-Build` directory has an executable named `bfx64`. Additionally, run the following to check if it runs:
```
$ ./bfx64 --help
```
You should see a help message with all available commands.

###Installing Additional Anaylsis Tools
There are two specific tools that are required to perform analysis on TA program models generated by bfx64. Both of these tools  allow for querying and visualizing bfx64 models. This guide will specify how to install these programs.

First, download the SWAGKit tarball from the University of Waterloo's [SWAG website](http://www.swag.uwaterloo.ca/swagkit/index.html) and unzip it to the directory of your choice:
```
$ wget http://www.swag.uwaterloo.ca/swagkit/distro/swagkit_linux86_bin_v3.03b.tar.gz
$ tar -xvzf swagkit_linux86_bin_v3.03b.tar.gz
```

Next, we want to add SWAGKit's binary path to your environment variables. To do this, simply do the following. **Note:** Replace the path to SWAGKit as shown in the following commands with the path to SWAGKit on your system!
```
$ echo "#SWAGKit Environment Variables:" >> ~/.bashrc
$ echo "export SWAGKIT_PATH=<REPLACE_WITH_SWAGKIT_PATH>" >> ~/.bashrc
$ echo "export PATH=$SWAGKIT_PATH/bin:$PATH" >> ~/.bashrc
```

That's it! You should be able to test if this worked by doing the following. The `bash` command will reload bash's environment variables and `grok` and `lsedit` are two analysis programs widely used during TA analysis.
```
$ bash
$ grok
$ lsedit
```

If both `Grok` and `LSEdit` started successfully, SWAGKit was configured on your computer. You are now able to run `ClangEx` and analyze program models!