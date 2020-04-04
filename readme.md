# GITUpdater
GITUpdater is a library
## Installing GIT-Update

1. Add automatic version file builder to your project
   - Copy library 'script' folder in your root platform.io project folder
   - add folowing line to your platformio.ini file :   
      ###### extra_scripts = pre:script\buildscript_versioning.py

2. Add library to your project

## Using GIT-Update

1. Add library : 

   add this line to your platformio.ini file 
      ##### ib_deps = https://github.com/Nimo11/GITUpdater
   or copy src folder content to your project folder

2. Add reference
   
   ###### #include "Updater.h"

2. Add this line in your code 
   
   ###### GITUpdater _updater;
   ###### _updater.SetCurrentVersion(BUILD_NUMBER);
   ###### _updater.SetGITProjectURL("your project url");

