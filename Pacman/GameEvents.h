#pragma once
#include "EventSystem.h"

constexpr EventId PlayerDied = make_sdbm_hash("PlayerDied");
constexpr EventId HealthChanged = make_sdbm_hash("HealthChanged");
constexpr EventId PointsChanged = make_sdbm_hash("PointsChanged");
constexpr EventId AllPelletsEaten = make_sdbm_hash("AllPelletsEaten");

constexpr EventId GhostFrightenedOver = make_sdbm_hash("GhostFrightenedOver");

// Timer events
constexpr EventId StartTimer = make_sdbm_hash("StartTimer");
constexpr EventId EatenAnimationTimer = make_sdbm_hash("EatenAnimationTimer");
constexpr EventId AnimationTimer = make_sdbm_hash("AnimationTimer");
constexpr EventId DeadAnimationTimer = make_sdbm_hash("DeadAnimationTimer");
constexpr EventId Test = make_sdbm_hash("Test");
