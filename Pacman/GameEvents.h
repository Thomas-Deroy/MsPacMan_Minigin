#pragma once
#include "EventSystem.h"

constexpr EventId PlayerDied = make_sdbm_hash("PlayerDied");
constexpr EventId HealthChanged = make_sdbm_hash("HealthChanged");
constexpr EventId PointsChanged = make_sdbm_hash("PointsChanged");
