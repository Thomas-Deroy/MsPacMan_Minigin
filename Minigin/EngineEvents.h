#pragma once
#include "EventSystem.h"

// Collision events
constexpr EventId OnTriggerEnter = make_sdbm_hash("OnTriggerEnter");
constexpr EventId OnTriggerStay = make_sdbm_hash("OnTriggerStay");
constexpr EventId OnTriggerExit = make_sdbm_hash("OnTriggerExit");

constexpr EventId OnTriggerFriendly = make_sdbm_hash("OnTriggerFriendly");
constexpr EventId OnTriggerEnemy = make_sdbm_hash("OnTriggerEnemy");
constexpr EventId OnTriggerWall = make_sdbm_hash("OnTriggerWall");
constexpr EventId OnTriggerObject = make_sdbm_hash("OnTriggerObject");
constexpr EventId OnTriggerDefault = make_sdbm_hash("OnTriggerDefault");