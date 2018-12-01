#ifndef IMPERIALFLEET_H
#define IMPERIALFLEET_H

#include <type_traits>
#include <cassert>
#include "rebelfleet.h"

template <typename U>
class ImperialStarship {
    U shield;
    U attackPower;
public:
    using valueType = U;

    ImperialStarship(U shield, U attackPower)
        : shield(shield), attackPower(attackPower) {}

    U getShield() {
        return shield;
    }

    void takeDamage(U damage) {
        if (damage > shield) {
            shield = 0;
        } else {
            shield -= damage;
        }
    }

    U getAttackPower() {
        return attackPower;
    }
};


template <typename U>
using DeathStar = ImperialStarship <U>;

template <typename U>
using ImperialDestroyer = ImperialStarship <U>;

template <typename U>
using TIEFighter = ImperialStarship <U>;


//template <typename I, typename R>
//void attack(ImperialStarship <I> &imperialShip, XWing <R> &rebelShip) {
//    rebelShip.takeDamage(imperialShip.getAttackPower());
//    imperialShip.takeDamage(rebelShip.getAttackPower());
//}
//
//template <typename I, typename R>
//void attack(ImperialStarship <I> &imperialShip, StarCruiser <R> &rebelShip) {
//    rebelShip.takeDamage(imperialShip.getAttackPower());
//    imperialShip.takeDamage(rebelShip.getAttackPower());
//}

template <typename I, typename R>
void attack(I &imperialShip, R &rebelShip) {
    rebelShip.takeDamage(imperialShip.getAttackPower());
    imperialShip.takeDamage(rebelShip.getAttackPower());
}

template <typename I, typename R>
void attack(ImperialStarship <I> &imperialShip, Explorer <R> &rebelShip) {
    rebelShip.takeDamage(imperialShip.getAttackPower());
}

#endif