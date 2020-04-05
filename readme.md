# GITUpdater
GITUpdater is a library
## Installing GIT-Update

1. Add automatic version file builder to your project
   - Copy library 'script' folder in your root platform.io project folder
   - add folowing line to your platformio.ini file :   
       **extra_scripts = pre:script\buildscript_versioning.py**

2. Add library to your project

## Using GIT-Update

1. Add library : 

   add this line to your platformio.ini file 
      **https://github.com/Nimo11/GITUpdater.git#master**
   or copy src folder content to your project folder

2. Add reference
   
    **#include "Updater.h"**
    **GITUpdater _updater;**

3. Add this line in your setup code 
      
    **_updater.SetCurrentVersion(BUILD_NUMBER);**
    **_updater.SetGITProjectURL("your project url");**

Now you can use :

##### CheckUpdate

            Check on line version and compare to current
            Return true if on line is newer

##### Updates

            Check on line version and compare to current
            Return true if on line is newer

##### GetOnLineVersion

            Return last on line version readed

##### SetCurrentVersion

            set current build project version value

##### GetCurrentVersion

            Return current project value

##### SetGITProjectURL

            Set GITHUB project URL. For example https://github.com/Nimo11/GITUpdater

##### GetGITProjectURL

            Return GITHUB project URL





