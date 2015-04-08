#pragma once

#include <string>

typedef int IDType;
typedef std::string IDDescriptor;
typedef int WeightType;

const WeightType ALIGNMENT = 0;
const WeightType COHESION = 1;
const WeightType SEPARATION = 2;

const IDType INVALID_ID = -1;
const IDType HIGHEST_ID = 9999;

const int WIDTH = 1024;
const int HEIGHT = 768;

#define MESSAGE_MANAGER gpGame->getMessageManager()
#define GRAPHICS_SYSTEM gpGame->getGraphicsSystem()
#define SPRITE_MANAGER gpGame->getSpriteManager()
#define UNIT_MANAGER gpGame->getUnitManager()
#define INPUT_MANAGER gpGame->getInputManager()
#define GAME_GUI gpGame->getGUI()
//#define INPUT_MANAGER gpGame->getInputManager();