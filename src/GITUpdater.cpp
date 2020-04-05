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
const char*   GITUpdater::GetGITProjectURL()
{
    return _gitUrl;
}

/*
            set current build project value
        */
void  GITUpdater::SetCurrentVersion(int v)
{
    _currentBuild=v;
}

/*
            Return GITHUB project URL
        */
void GITUpdater::SetGITProjectURL(const char *url)
{
    _gitUrl = url;
}

/*
    Check on line version and compare to current
    Return true if on line is newer
        */
void GITUpdater::CheckUpdate()
{
    if ((_gitUrl == NULL) || (_gitUrl[0] == '\0'))
    {
        Serial.println(F("Project URL not set"));
        return;
    }

    if ((_gitUrl == NULL) || (_gitUrl[0] == '\0'))
    {
        Serial.println(F("Project URL not set"));
        return;
    }

    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println(F("Wifi not connected"));
        return;
    }

    String URL = String(_gitUrl);

    URL.concat(F("/include/version.h"));

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

    http.begin(URL);

    int httpCode = http.GET();
    if (httpCode == 200)
    {
        _onLineVersion = httpsClient.readString().toInt();

        Serial.print(F("Current firmware version: "));
        Serial.println(_currentBuild);
        Serial.print(F("Available firmware version: "));
        Serial.println(_onLineVersion);
    }
    else
    {
        Serial.print("Firmware version check failed, got HTTP response code ");
        Serial.println(httpCode);
    }
    http.end();
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

        String URL = String(_gitUrl);

        URL.concat("/.pio/build/esp12e/firmware.bin");

        t_httpUpdate_return ret = ESPhttpUpdate.update(URL);

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

String GITUpdater::getMAC()
{
    uint8_t mac[6];
    char result[14];

    snprintf(result, sizeof(result), "%02x%02x%02x%02x%02x%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    return String(result);
}

#endif