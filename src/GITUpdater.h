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
#include "LanguageManager.h"

class GITUpdater
{

public:

    enum class Languages
    {
        LANG_EN,
        LANG_FR
    };
    
    enum class Msg
    {
        ERR_PRJ_URL,
        ERR_PRJ_NOT_SET,
        ERR_WIFI,
        ERR_CHK_KO,
        ERR_UDP_FAILED,
        ERR_UDP_NO_UPD,

        MSG_CHK,
        MSG_URL,
        MSG_CUR_VER,
        MSG_AVI_VER,
        MSG_PREP,
        MSG_NO_UPD
    };
    
    GITUpdater(Languages lng);

    /*
            Return last on line version readed
        */
    const int GetOnLineVersion();

    /*
            Return current project value
        */
    const int GetCurrentVersion();

    /*
            set current build project value
        */
    void SetCurrentVersion(int v);

    /*
            Return GITHUB project URL
        */
    const char *GetGITProjectURL();

    /*
            Set GITHUB project URL
        */
    void SetGITProjectURL(const char *url);

    /*
            Check on line version and compare to current
            Return true if on line is newer
        */
    bool CheckUpdate();

    /*
            Check on line version and compare to current
            Return true if on line is newer
        */
    bool Updates();

    /*
            Hold running state
    */
    String State;


private:
    //on line version
    int _onLineVersion;

    //current build
    int _currentBuild = -1;

    // project URL
    const char *_gitUrl;

    // raw project URL
    char _rawGitURL[250];

    LanguageManager _LM;

    void Logln(const char *msg);

    void Logln(int i);

    void Log(const char *msg);

    void ReplaceAll(std::string str, char *dest, const std::string &from, const std::string &to);


    std::map<int, std::map<int, const char *>> dict;
};

#endif
#endif