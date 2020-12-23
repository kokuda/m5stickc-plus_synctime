
/**
 * power_setup - Call this in the setup() function to initialize the power functions
 * Returns the number of times that the device has woken from a sleep
 */

int power_setup();

/**
 * power_deep_sleep_wake_on_btn_B - Puts the device into a deep sleep that will wake when you press Button B
 * Make sure that Button B is not pressed when this is called or the device will immediately wake up.
 */

void power_deep_sleep_wake_on_btn_B();
