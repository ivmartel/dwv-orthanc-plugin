DwvExplorer
===========

[Orthanc](http://www.orthanc-server.com/) plugin for the [dwv](https://github.com/ivmartel/dwv/wiki) (DICOM Web Viewer).
This plugin allows to preview DICOM series and instances stored in Orthanc using DWV. It is based on the default OrthancExplorer and just changes the target of the preview buttons. 
 
Build requirements
------------------
 * python ([2.7.8](https://www.python.org/download/releases/2.7.8/)),
 * CMake ([2.8.12](http://www.cmake.org/download/)),
 * a C++ compiler.
 
Build instructions
------------------
 * Checkout the Orthanc source code ([0.8.4](https://sourceforge.net/projects/orthancserver/files/Orthanc-0.8.4.tar.gz/download)),
 * Checkout the DwvExplorer plugin source code (git clone or [download zip](https://github.com/ivmartel/DwvExplorer/archive/master.zip)),
 * Point CMake to the `DwvExplorer` folder and choose a build folder,
 * Run `Configure` in CMake,
 * Set the path to the Orthanc sources in the CMake `ORTHANC_DIR` variable,
 * Run `Generate` in CMake,
 * Compile with your favourite compiler.

Orthanc configuration
---------------------
Just add the path to the `DwvExplorer` shared library (built or from its [releases](https://github.com/ivmartel/DwvExplorer/releases)) in the *"Plugins"* section of an Orthanc [configuration file](https://code.google.com/p/orthanc/wiki/OrthancConfiguration). 

Launch Orthanc with the configuration file as input (for example `Orthanc ./Configuration.json`) and browse to [http://localhost:8042/plugin-dwv/explorer.html#](http://localhost:8042/plugin-dwv/explorer.html#) 
(update if you changed the default port).
