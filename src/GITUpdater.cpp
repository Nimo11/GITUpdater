/**
 * GITUpdater.h
 * 
 * GITUpdater, a library for the ESP8266
 * 
 * @author J DAPOT
 * @version 0.0.0
 * @license MIT
 */

#include "GITUpdater.h"

#if defined(ESP8266) || defined(ESP32)

GITUpdater::GITUpdater(Languages lng)
{
    _LM.CurrentLanguage=(int)lng;
    _LM.Dictionary = &dict;
}

/*
            Return last on line version readed
        */
const int GITUpdater::GetOnLineVersion()
{
    return _onLineVersion;
}

/*
            Return current project value
        */
const int GITUpdater::GetCurrentVersion()
{
    return _currentBuild;
}

/*
            Return current project value
        */
const char *GITUpdater::GetGITProjectURL()
{
    return _gitUrl;
}

/*
            set current build project value
        */
void GITUpdater::SetCurrentVersion(int v)
{
    _currentBuild = v;
}

/*
            Return GITHUB project URL
        */
void GITUpdater::SetGITProjectURL(const char *url)
{
    _gitUrl = url;
    ReplaceAll(url, _rawGitURL, "github.com", "raw.githubusercontent.com");
}

/*
    Check on line version and compare to current
    Return true if on line is newer
        */
bool GITUpdater::CheckUpdate()
{
    if ((_rawGitURL == NULL) || (_rawGitURL[0] == '\0'))
    {
        Logln(_LM.Get((int)Msg::ERR_PRJ_URL));
        return false;
    }

    if ((_currentBuild == -1))
    {
        Logln(_LM.Get((int)Msg::ERR_PRJ_NOT_SET));
        return false;
    }

    if (WiFi.status() != WL_CONNECTED)
    {
        Logln(_LM.Get((int)Msg::ERR_WIFI));
        return false;
    }

    String URL = String(_rawGitURL);

    URL.concat(F("/versioning"));

    Logln(_LM.Get((int)Msg::MSG_CHK));

    Log(_LM.Get((int)Msg::MSG_URL));
    Logln(URL.c_str());

    BearSSL::WiFiClientSecure httpsClient;
    HTTPClient http;

    httpsClient.setCiphers({BR_TLS_RSA_WITH_AES_256_CBC_SHA256,
                            BR_TLS_RSA_WITH_AES_128_CBC_SHA256,
                            BR_TLS_RSA_WITH_AES_256_CBC_SHA,
                            BR_TLS_RSA_WITH_AES_128_CBC_SHA});
    httpsClient.setInsecure();

    http.begin(httpsClient, URL);

    int httpCode = http.GET();
    if (httpCode == 200)
    {
        _onLineVersion = httpsClient.readString().toInt();

        Log(_LM.Get((int)Msg::MSG_CUR_VER));
        Logln(_currentBuild);
        Log(_LM.Get((int)Msg::MSG_AVI_VER));
        Logln(_onLineVersion);
        http.end();
        return _currentBuild < _onLineVersion;
    }
    else
    {
        Log(_LM.Get((int)Msg::ERR_CHK_KO));
        Logln(httpCode);
    }
    http.end();
    return false;
}

/*
    Update project
    Return true if update complete successfully
        */
bool GITUpdater::Updates()
{
    State = "";

    if (CheckUpdate())
    {
        Logln(_LM.Get((int)Msg::MSG_PREP));

        BearSSL::WiFiClientSecure httpsClient;

        httpsClient.setCiphers({BR_TLS_RSA_WITH_AES_256_CBC_SHA256,
                                BR_TLS_RSA_WITH_AES_128_CBC_SHA256,
                                BR_TLS_RSA_WITH_AES_256_CBC_SHA,
                                BR_TLS_RSA_WITH_AES_128_CBC_SHA});
        httpsClient.setInsecure();

        String URL = String(_rawGitURL);

        URL.concat(F("/bin/firmware.bin"));

        t_httpUpdate_return ret = ESPhttpUpdate.update(httpsClient, URL);

        switch (ret)
        {
        case HTTP_UPDATE_FAILED:
            char buffer[50];
            sprintf(buffer, _LM.Get((int)Msg::ERR_UDP_FAILED), ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
            Logln(buffer);
            return false;
            break;

        case HTTP_UPDATE_NO_UPDATES:
            Logln(_LM.Get((int)Msg::ERR_UDP_NO_UPD));
            return false;
            break;
        case HTTP_UPDATE_OK:
            return true;
            break;
        }
    }
    else
    {
        Logln(_LM.Get((int)Msg::MSG_NO_UPD));
        return false;
    }

    return false;
}

void GITUpdater::Log(const char *msg)
{
    Serial.print(msg);
    State.concat(msg);
}

void GITUpdater::Logln(const char *msg)
{
    Serial.println(msg);
    State.concat(msg);
    State.concat("<br>");
}

void GITUpdater::Logln(int i)
{
    char buffer [33];
    Logln(itoa (i,buffer,10));
}

void GITUpdater::ReplaceAll(std::string source, char *dest, const std::string &from, const std::string &to)
{
    std::string newString;

    std::string::size_type lastPos = 0;
    std::string::size_type findPos;

    while (std::string::npos != (findPos = source.find(from, lastPos)))
    {
        newString.append(source, lastPos, findPos - lastPos);
        newString += to;
        lastPos = findPos + from.length();
    }

    // Care for the rest after last occurrence
    newString += source.substr(lastPos);

    strcpy(dest, newString.c_str());
}

std::map<int, std::map<int, const char *>> dict PROGMEM = {
    {(int)GITUpdater::Languages::LANG_EN, {{(int)GITUpdater::Msg::ERR_PRJ_URL, "Project URL not set."}, {(int)GITUpdater::Msg::ERR_PRJ_NOT_SET, "Current project version not set"}, {(int)GITUpdater::Msg::ERR_WIFI, "Wifi not connected"}, {(int)GITUpdater::Msg::ERR_CHK_KO, "Firmware version check failed, got HTTP response code "}, {(int)GITUpdater::Msg::ERR_UDP_FAILED, "HTTP utpdate failed. Error (%d): %s"}, {(int)GITUpdater::Msg::ERR_UDP_NO_UPD, "No update aviable"},

                               {(int)GITUpdater::Msg::MSG_CHK, "Checking for firmware updates."},
                               {(int)GITUpdater::Msg::MSG_URL, "Firmware version URL: "},
                               {(int)GITUpdater::Msg::MSG_CUR_VER, "Current firmware version: "},
                               {(int)GITUpdater::Msg::MSG_AVI_VER, "Available firmware version: "},
                               {(int)GITUpdater::Msg::MSG_PREP, "Preparing to update"},
                               {(int)GITUpdater::Msg::MSG_NO_UPD, "Already on latest version"}}},

    {(int)GITUpdater::Languages::LANG_FR, {{(int)GITUpdater::Msg::ERR_PRJ_URL, "L'URL du projet n'est pas définie"}, {(int)GITUpdater::Msg::ERR_PRJ_NOT_SET, "La version du projet actuel n'est pas définie"}, {(int)GITUpdater::Msg::ERR_WIFI, "Le Wifi n'est pas disponible"}, {(int)GITUpdater::Msg::ERR_CHK_KO, "Impossible de vérifier la version en ligne. Le code erreur était "}, {(int)GITUpdater::Msg::ERR_UDP_FAILED, "Echec de mise à jour . Le code erreur était (%d): %s"}, {(int)GITUpdater::Msg::ERR_UDP_NO_UPD, "Pas de mise à jour disponible"},

                               {(int)GITUpdater::Msg::MSG_CHK, "Vérification en cours."},
                               {(int)GITUpdater::Msg::MSG_URL, "URL du Firmware : "},
                               {(int)GITUpdater::Msg::MSG_CUR_VER, "Version actuelle du firmware : "},
                               {(int)GITUpdater::Msg::MSG_AVI_VER, "Version disponible du firmware : "},
                               {(int)GITUpdater::Msg::MSG_PREP, "Préparation de la mise à jour"},
                               {(int)GITUpdater::Msg::MSG_NO_UPD, "Aucune nouvelle version n'est disponible"}}}};

#endif