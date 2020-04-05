/**
 * GITUpdater.h
 * 
 * GITUpdater, a library for the ESP8266
 * 
 * @author J DAPOT
 * @version 0.0.0
 * @license MIT
 */

#ifndef GITUpdater_h
#define GITUpdater_h

#if defined(ESP8266)

#include <Arduino.h>

#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266httpUpdate.h>
#include "BearSSLHelpers.h"
#include <string.h>


class GITUpdater
{

public:
    /*
            Return last on line version readed
        */
    const int   GetOnLineVersion();

    /*
            Return current project value
        */
    const int   GetCurrentVersion();

    /*
            set current build project value
        */
    void   SetCurrentVersion(int v);

    /*
            Return GITHUB project URL
        */
    const int   GetGITProjectURL();

    /*
            Set GITHUB project URL
        */
    void SetGITProjectURL(const char* url);

    /*
            Check on line version and compare to current
            Return true if on line is newer
        */
    void CheckUpdate();

    /*
            Check on line version and compare to current
            Return true if on line is newer
        */
    bool Updates();

private:
    //on line version
    int _onLineVersion;

    //current build
    int _currentBuild;

    // on line version URL
    const char *_gitUrl;

    String getMAC();
};

#endif
#endif