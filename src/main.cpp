#include "M5StickCPlus.h"
#include "time.h"
#include "wifi.h"
#include "power.h"

#define NTP_SERVER "pool.ntp.org"
#define TIME_ZONE_OFFSET_HRS -8

// Toggle deep sleep on or off.
// If true then the device will go into deep sleep mode and wakeup when you press button B
// If this is false then the LCD screen will turn off and on when pressing button B to save battery.
#define DEEP_SLEEP true

//
// config.h should include the two macros containing your Wifi SSID and password.
// config-TEMPLATE.h contains an example.  Copy that file as config.h and set the values.
// Do not commit config.h into source control
//
#include "config.h"

bool displayOn = true;

void checkAXPPress()
{
  if (M5.Axp.GetBtnPress())
  {
    do
    {
      delay(20);
    } while (M5.Axp.GetBtnPress());
    M5.Beep.mute();
    ESP.restart();
  }
}

const char *setMessage(const char *message)
{
  static const char *previousMessage = "";
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(40, 0, 2);
  M5.Lcd.println(message);

  const char *result = previousMessage;
  previousMessage = message;
  return result;
}

void setRtcTime(tm &timeInfo)
{
  RTC_TimeTypeDef newTime;
  newTime.Hours = timeInfo.tm_hour;
  newTime.Minutes = timeInfo.tm_min;
  newTime.Seconds = timeInfo.tm_sec;
  M5.Rtc.SetTime(&newTime);

  RTC_DateTypeDef newDate;
  newDate.WeekDay = timeInfo.tm_wday;
  newDate.Month = timeInfo.tm_mon + 1;
  newDate.Date = timeInfo.tm_mday;
  newDate.Year = timeInfo.tm_year + 1900;
  M5.Rtc.SetData(&newDate);
}

void connectWifi()
{
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Connecting to Wifi " WIFI_SSID);
    const char *resetMessage = setMessage("Connecting\n" WIFI_SSID);
    delay(500);
    checkAXPPress();
    setMessage(resetMessage);
  }
  Serial.println("Connected to Wifi " WIFI_SSID);
}

void synchronizeTime()
{
  connectWifi();

  const char *resetMessage = setMessage("Synchronizing");
  configTime(TIME_ZONE_OFFSET_HRS * 3600, 0, NTP_SERVER);
  struct tm timeInfo;
  if (getLocalTime(&timeInfo))
  {
    setRtcTime(timeInfo);
  }
  delay(500);
  setMessage(resetMessage);
}

void setup()
{
  M5.begin();
  Wire.begin(32, 33);

  M5.Lcd.setRotation(3);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextSize(2);

  int setupCount = power_setup();

  setMessage("Synchronizer");

  // Do a synchronize on the first startup
  if (setupCount <= 1)
  {
    synchronizeTime();
  }
}

// Turn on or off the LCD depending when button presses
void doButtonCheck()
{
  if (M5.BtnB.wasPressed())
  {
    if (displayOn)
    {
      M5.Lcd.fillScreen(BLACK);
      M5.Lcd.writecommand(TFT_DISPOFF);
      M5.Axp.ScreenBreath(0);
      M5.Axp.SetLDO2(false);
      M5.Axp.SetLDO3(false);
      displayOn = false;

      if (DEEP_SLEEP)
      {
        // Wait some time for the button to release before going to sleep
        delay(2000);
        power_deep_sleep_wake_on_btn_B();
      }
    }
    else
    {
      M5.Lcd.writecommand(TFT_DISPON);
      M5.Axp.ScreenBreath(12);
      M5.Axp.SetLDO2(true);
      M5.Axp.SetLDO3(true);
      displayOn = true;
    }
  }
}

void loop()
{
  RTC_TimeTypeDef time;
  RTC_DateTypeDef date;

  M5.update();
  doButtonCheck();
  checkAXPPress();

  if (displayOn)
  {
    M5.Lcd.setCursor(0, 30);
    M5.Rtc.GetTime(&time);
    M5.Rtc.GetData(&date);
    M5.Lcd.printf("Date: %04d-%02d-%02d\n", date.Year, date.Month, date.Date);
    M5.Lcd.printf("Time: %02d : %02d : %02d\n", time.Hours, time.Minutes, time.Seconds);

    if (M5.BtnA.isPressed())
    {
      synchronizeTime();
    }
  }

  delay(100);
}