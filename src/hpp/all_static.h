#pragma once

#include "wall.h"
#include "gate.h"
#include "goal.h"

#include "tower.h"
#include "chargeTower.h"

const int amountTypeTowers = 2;

/**
 * add another type of tower
 * 
 * include the file for tower
 * increase amountTypeTowers depending on how many types exist
 * add type definition creation in CMap::GenerateTowers 
 * add reading in CMap::GenerateNewMap
 * */