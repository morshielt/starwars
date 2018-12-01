#ifndef REBELFLEET_H
#define REBELFLEET_H

#include <type_traits>
#include <cassert>

//enum class TypeOfShip { XWing, Explorer, StarCruiser };


template <typename U, int minSpeed, int maxSpeed, bool canAttack>
class RebelStarship {
    U speed;
    U shield;
    U attackPower;
public:
    using valueType = U;

    template <bool x = canAttack>
    RebelStarship(U shield, U speed, U attackPower, typename std::enable_if <(x)>::type * = 0)
        : speed(speed), shield(shield), attackPower(attackPower) {
        assert(minSpeed <= speed && speed <= maxSpeed);
        //TODO na moodle mówią, że static_assert'y, ale to nie działa
    }

    template <bool x = !canAttack>
    RebelStarship(U shield, U speed, typename std::enable_if <(x)>::type * = 0)
        : speed(speed), shield(shield) {
        assert(minSpeed <= speed && speed <= maxSpeed);
    }

    U getShield() {
        return shield;
    }

    U getSpeed() {
        return speed;
    }

    void takeDamage(U damage) {
        if (damage > shield) {
            shield = 0;
        } else {
            shield -= damage;
        }
    }

    template <bool x = canAttack>
    U getAttackPower(typename std::enable_if <(x)>::type * = 0) {
        return attackPower;
    }
};

template <typename U>
using XWing = RebelStarship <U, 299796, 2997960, true>;

template <typename U>
using Explorer = RebelStarship <U, 299796, 2997960, false>;

template <typename U>
using StarCruiser = RebelStarship <U, 99999, 299795, true>;

#endif