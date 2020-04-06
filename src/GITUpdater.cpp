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
        Serial.println(F("Project URL not set"));
        return false;
    }

    if ((_currentBuild == -1))
    {
        Serial.println(F("Current project not set"));
        return false;
    }

    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println(F("Wifi not connected"));
        return false;
    }

    String URL = String(_rawGitURL);

    URL.concat(F("/versioning"));

    Serial.println(F("Checking for firmware updates."));

    Serial.print(F("Firmware version URL: "));
    Serial.println(URL);

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

        Serial.print(F("Current firmware version: "));
        Serial.println(_currentBuild);
        Serial.print(F("Available firmware version: "));
        Serial.println(_onLineVersion);
        http.end();
        return _currentBuild < _onLineVersion;
    }
    else
    {
        Serial.print("Firmware version check failed, got HTTP response code ");
        Serial.println(httpCode);
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
    if (true)
    {
        Serial.println("Preparing to update");

        BearSSL::WiFiClientSecure httpsClient;

        httpsClient.setCiphers({BR_TLS_RSA_WITH_AES_256_CBC_SHA256,
                                BR_TLS_RSA_WITH_AES_128_CBC_SHA256,
                                BR_TLS_RSA_WITH_AES_256_CBC_SHA,
                                BR_TLS_RSA_WITH_AES_128_CBC_SHA});
        httpsClient.setInsecure();

        String URL = String(_rawGitURL);

        URL.concat("/bin/firmware.bin");

        t_httpUpdate_return ret = ESPhttpUpdate.update(httpsClient, URL);

        switch (ret)
        {
        case HTTP_UPDATE_FAILED:
            Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
            return false;
            break;

        case HTTP_UPDATE_NO_UPDATES:
            Serial.println(F("HTTP_UPDATE_NO_UPDATES"));
            return false;
            break;
        case HTTP_UPDATE_OK:
            return true;
            break;
        }
    }
    else
    {
        Serial.println(F("Already on latest version"));
        return false;
    }

    return false;
}

void GITUpdater::ReplaceAll(std::string source, char *dest, const std::string &from, const std::string &to)
{
    std::string newString;
    //newString.reserve(source.length());  // avoids a few memory allocations

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

#endif