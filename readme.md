dwv-orthanc-plugin
==================

An [Orthanc](http://www.orthanc-server.com/) plugin to preview DICOM series and instances using  [dwv](https://github.com/ivmartel/dwv/wiki), a pure HTML5/JavaScript **D**ICOM **W**eb **V**iewer.

Released under GNU GPL license (see [license.txt](license.txt)). 
 
[![Build Status](https://travis-ci.org/ivmartel/dwv-orthanc-plugin.svg?branch=master)](https://travis-ci.org/ivmartel/dwv-orthanc-plugin)

Build instructions
------------------
In order to build this project, you need: python, CMake and a C++ compiler. Check out the build steps from the [travis](https://github.com/ivmartel/dwv-orthanc-plugin/blob/master/.travis.yml) file and the build log. The test build are generated using python 2.7, CMake 3.2 and gcc 4.8. The targeted version of Orthanc can be set from CMake, tested version go from 1.0 to 1.3.

The result is the `DwvOrthancPlugin` shared library.

Orthanc configuration
---------------------
Just add the path to the `DwvOrthancPlugin` shared library in the *"Plugins"* section of your Orthanc [configuration file](https://orthanc.chu.ulg.ac.be/book/users/configuration.html). 

Launch
---------------------
Launch Orthanc with the modified configuration file as input (for example `Orthanc ./Configuration.json`) and browse to the Orthanc explorer (default at [http://localhost:8042/app/explorer.html#](http://localhost:8042/app/explorer.html#)). At instance and series level, a preview button allows to launch dwv.

As a check if things go west, DWV should be availalble from [http://localhost:8042/dwv-plugin/dwv/viewers/mobile/index.html](http://localhost:8042/dwv-plugin/dwv/viewers/mobile/index.html).

**Note**: the current configuration launches a new page to visualise the data and needs to have the pop-up blockers disabled for the Orthanc pages. This can be changed in the `replace.js` file by modifying the target of the created form.
