DwvExplorer
===========

[Orthanc](http://www.orthanc-server.com/) plugin for the [dwv](https://github.com/ivmartel/dwv/wiki) (DICOM Web Viewer).
This plugin allows to preview DICOM series and instances stored in Orthanc using DWV. It is based on the default OrthancExplorer and just changes the target of the preview buttons. 
 
Build requirements
------------------
 * python,
 * CMake,
 * a C++ compiler.
 
Build instructions
------------------
 * Checkout the Orthanc source code,
 * Checkout the DwvExplorer plugin source code,
 * Point CMake to the `DwvExplorer` folder and choose a build folder,
 * Run `Configure` in CMake,
 * Set the path to the Orthanc sources in the CMake `ORTHANC_DIR` variable,
 * Run `Generate` in CMake,
 * Compile with your favourite compiler.

Orthanc configuration
---------------------
Just add the path to the `DwvExplorer` shared library in the *"Plugins"* section of an Orthanc [configuration file](https://code.google.com/p/orthanc/wiki/OrthancConfiguration). 

Launch Orthanc with the configuration file as input (for example `Orthanc ./Configuration.json`) and browse to [http://localhost:8042/plugin-dwv/explorer.html#](http://localhost:8042/plugin-dwv/explorer.html#) 
(update if you changed the default port).
