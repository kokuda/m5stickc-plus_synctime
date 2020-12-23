#include "M5StickCPlus.h"

// RTC_DATA_ATTR variables will persist across deep sleeps
RTC_DATA_ATTR int setupCount = 0;

void print_wakeup_reason()
{
    esp_sleep_wakeup_cause_t wakeup_reason;
    wakeup_reason = esp_sleep_get_wakeup_cause();
    switch (wakeup_reason)
    {
    case 1:
        Serial.println("Wakeup caused by external signal using RTC_IO");
        break;
    case 2:
        Serial.println("Wakeup caused by external signal using RTC_CNTL");
        break;
    case 3:
        Serial.println("Wakeup caused by timer");
        break;
    case 4:
        Serial.println("Wakeup caused by touchpad");
        break;
    case 5:
        Serial.println("Wakeup caused by ULP program");
        break;
    default:
        Serial.println("Wakeup was not caused by deep sleep");
        break;
    }
}

int power_setup()
{
    //Increment boot number and print it every call to setup
    ++setupCount;
    Serial.println("Boot number: " + String(setupCount));
    print_wakeup_reason();

    return setupCount;
}

void power_deep_sleep_wake_on_btn_B()
{
    esp_sleep_enable_ext0_wakeup(GPIO_NUM_39, 0);
    esp_deep_sleep_start();
}
