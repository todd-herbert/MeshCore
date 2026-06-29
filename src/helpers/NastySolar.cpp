#include "NastySolar.h"
#include "bluefruit.h"
#include "target.h"

// Quick and dirty hack to intercept boot and check voltage
// If too low, refuse to boot; sleep and check again later

#ifdef NASTY_SOLAR

#pragma message "Building with nasty solar hack to protect battery"

#if !defined(RAK_4631) && !defined(PROMICRO)
#error Nasty solar hack not yet implemented for this device
#endif

#endif



void nastySolarBootCheck()
{

    uint32_t mv = board.getBattMilliVolts();

    // If voltage not high enough to EXIT the pseudo-sleep state
    if (mv < NASTYSOLAR_RECHARGE_MV) {
        // Blink the LED

        #if defined(PIN_LED)
            uint32_t pinLED=PIN_LED;
        #elif defined(PIN_LED2)
            uint32_t pinLED=PIN_LED2; // Blue LED on RAK4631
        #endif

        pinMode(pinLED, OUTPUT);
        digitalWrite(pinLED, HIGH);
        delay(500);
        digitalWrite(pinLED, LOW);

        // Shutdown bluetooth for minimum power draw
        Bluefruit.begin();
        Bluefruit.Advertising.stop();
        Bluefruit.setTxPower(-40); // Minimum power

        // Init the radio, to place into a low power state
        SX1262 radio = new Module(P_LORA_NSS, P_LORA_DIO_1, P_LORA_RESET, P_LORA_BUSY);
        radio.begin();
        radio.sleep();
        SPI.end();
        Wire.end();   // Probably redundant
        Serial.end(); // Probably redundant

        #ifdef SX126X_POWER_EN
            pinMode(SX126X_POWER_EN, OUTPUT);
            digitalWrite(SX126X_POWER_EN, LOW);
        #endif

        sd_power_mode_set(NRF_POWER_MODE_LOWPWR);

        // Wait, then reboot
        delay(NASTYSOLAR_BOOTCHECK_MINUTES * 60 * 1000UL);
        NVIC_SystemReset();

        // We shouldn't be able to reach here - shutting down / reset
        while (true)
            yield();
    }
}