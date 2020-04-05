# GITUpdater V0.1

GITUpdater is a 8266 library to add automatic updater to your project. GITUpdater allows you to check the version of your project available on GITHub and to carry out the update if you wish. 


## Installing GIT-Update

1. Add automatic version file builder to your project
   - Copy library 'script' folder in your root platform.io project folder
   - add folowing line to your platformio.ini file :   
       **extra_scripts = pre:script\buildscript_versioning.py**
                       **post:script\copy_bin.py**
2. Add library to your project

## Using GIT-Update

1. Add library : 

   add this line to your platformio.ini file 
      **lib_deps = https://github.com/Nimo11/GITUpdater.git#master**
   or copy src folder content to your project folder

2. Add reference
   
    **#include "Updater.h"**

    **#include "version.h"**

    **GITUpdater _updater;**

4. Add this line in your setup code 
      
    **_updater.SetCurrentVersion(BUILD_NUMBER);**

    **_updater.SetGITProjectURL("your project url");**

## Now you can use :

##### void CheckUpdate()

            Check on line version and compare to current
            Return true if on line is newer

##### bool Updates()

            Check on line version and compare to current
            Return true if on line is newer

##### GetOnLineVersion()

            Return last on line version readed

##### SetCurrentVersion()

            set current build project version value

##### GetCurrentVersion()

            Return current project value

##### SetGITProjectURL()

            Set GITHUB project URL. For example https://github.com/Nimo11/GITUpdater

##### GetGITProjectURL()

            Return GITHUB project URL





