#pragma once

#include "RadioLib.h"

// How low can the battery run before entering pseudo-sleep
#ifndef NASTYSOLAR_SLEEP_MV
#define NASTYSOLAR_SLEEP_MV 3450
#endif

// How high must the battery recharge before wake is acceptable
// Provides hysteresis - avoid waking until we're sure we've got decent power
#ifndef NASTYSOLAR_RECHARGE_MV
#define NASTYSOLAR_RECHARGE_MV 3600
#endif

// While in low-voltage shutdown, how often should the device wake to check the battery
#ifndef NASTYSOLAR_BOOTCHECK_MINUTES
#define NASTYSOLAR_BOOTCHECK_MINUTES 30
#endif

// While running, how often should the device check if battery is too low
#ifndef NASTYSOLAR_REBOOTCHECK_MINUTES
#define NASTYSOLAR_REBOOTCHECK_MINUTES 5
#endif

extern void nastySolarBootCheck();