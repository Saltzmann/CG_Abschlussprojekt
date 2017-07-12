#ifndef OPTIONS_H
#define OPTIONS_H

//Zentrale Stelle um Simulationsparameter anzupassen

#include <QtGlobal> //für typedefs

namespace Options {
    float const minR = 50.f;
    float const maxR = 100.f;
    float const dropletsMinR = 5.f;
    float const dropletsMaxR = 10.f;
    float const dropCombindedMaxRadius = 350.f;
    float const dropletsCleaningRadiusMultiplier = 0.25f;
    float const trailTimer = 50.f;
    size_t const maxNumberDroplets = 7500; //5k-10k
    size_t const maxNumberNonTrailDrops = 100; //100ish
    size_t const upperSpawnBorderDrops = 150;
    size_t const lowerSpawnBorderDrops = 150;
    size_t const upperSpawnBorderDroplets = 100;
    size_t const lowerSpawnBorderDroplets = 100;
}

#endif // OPTIONS_H
