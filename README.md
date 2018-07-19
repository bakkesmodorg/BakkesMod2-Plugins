# BakkesMod2-Plugins
Default plugins for BakkesMod 2. These plugins show how the BakkesMod plugin system can be used to create dynamic training scenarios/gamemodes and other enhancement features for Rocket League.
In order to compile these plugins you need the following dependencies:

## BakkesMod SDK
All plugins need the BakkesMod SDK which can be found [here](https://github.com/Bakkes/BakkesModSDK). Simply add the lib folder to the list of libraries and add the include folder to your additional include directories.
Make sure the build target is set for win32 in release mode and you should be good to go! Most plugins can be compiled with just the BakkesMod SDK, except for:

## Python Project
The python project requires [Boost.Python](https://www.boost.org/doc/libs/1_66_0/libs/python/doc/html/index.html)

## RCONPlugin
In order to build the RCONPlugin, you need the [websocketpp library](https://github.com/zaphoyd/websocketpp)
