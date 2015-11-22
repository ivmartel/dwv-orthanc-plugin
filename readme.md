dwv-orthanc-plugin
==================

An [Orthanc](http://www.orthanc-server.com/) plugin to preview DICOM series and instances using  [dwv](https://github.com/ivmartel/dwv/wiki), a pure HTML5/JavaScript **D**ICOM **W**eb **V**iewer.

Released under GNU GPL license (see [license.txt](license.txt)). 
 
[![Build Status](https://travis-ci.org/ivmartel/dwv-orthanc-plugin.svg?branch=master)](https://travis-ci.org/ivmartel/dwv-orthanc-plugin)

Build instructions
------------------
In order to build this project, you need: python, CMake and a C++ compiler. Check out the build steps from the [travis](https://github.com/ivmartel/dwv-orthanc-plugin/blob/master/.travis.yml) file and the build log. The current build is generated using python 2.7, CMake 2.8 and gcc 4.6.

The result is the `DwvOrthancPlugin` shared library.

Installation
------------
Just add the path to the `DwvOrthancPlugin` shared library in the *"Plugins"* section of your Orthanc [configuration file](https://orthanc.chu.ulg.ac.be/book/users/configuration.html). 

Orthanc configuration
---------------------
Launch Orthanc with the modified configuration file as input (for example `Orthanc ./Configuration.json`) and browse to the Orthanc explorer (default at [http://localhost:8042/app/explorer.html#](http://localhost:8042/app/explorer.html#)). At instance and series level, a preview button allows to launch dwv.

**Note**: the current configuration launches a new page to visualise the data and needs to have the pop-up blockers disabled for the Orthanc pages. This can be changed in the `replace.js` file.
