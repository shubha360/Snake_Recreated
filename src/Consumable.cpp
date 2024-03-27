#include "../include/Consumable.h"

std::random_device Consumable::seed_;
std::mt19937 Consumable::randomEngine_(seed_());
