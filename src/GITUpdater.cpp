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
    //_rawGitURL = ReplaceAll(url, "github.com", "raw.githubusercontent.com").c_str();
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
        return _currentBuild<_onLineVersion;
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
    if (false)
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

        t_httpUpdate_return ret = ESPhttpUpdate.update(httpsClient,URL);

        switch (ret)
        {
        case HTTP_UPDATE_FAILED:
            Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
            return true;
            break;

        case HTTP_UPDATE_NO_UPDATES:
            Serial.println("HTTP_UPDATE_NO_UPDATES");
            return false;
            break;
        }
    }
    else
    {
        Serial.println("Already on latest version");
        return false;
    }

    return false;
}

std::string GITUpdater::ReplaceAll(std::string &str, const std::string &from, const std::string &to)
{
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos)
    {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
    }
    return str;
}

#endif