#ifndef BATTLE_H
#define BATTLE_H

#include <tuple>
#include <cstdio>
#include <algorithm>
#include <array>

#include "rebelfleet.h"
#include "imperialfleet.h"

template <typename T, T... i>
constexpr auto square_array(std::index_sequence <i...>) {
    return std::array <T, sizeof...(i)>{{i * i...}};
}

template <typename T, T size>
constexpr auto square_array() {
    return square_array(std::make_index_sequence <size>{});
}

template <typename T, T t0, T t1, typename... Ships>
class SpaceBattle {
private:
    size_t imperialCounter;
    size_t rebelCounter;
    std::tuple <Ships...> shipsInBattle;
    static constexpr auto attacks = square_array <T, t1 + 1>();
    T currentTime;

public:
    SpaceBattle(Ships &...ships) : shipsInBattle(ships...) {
        static_assert(t0 > 0, "Invalid t0");
        static_assert(t1 >= t0, "Invalid t1");
        currentTime = t0;
        imperialCounter = 0;
        rebelCounter = 0;
        if constexpr (sizeof...(Ships) != 0)
            countTheShips(std::get <0>(shipsInBattle));
    }

    size_t countImperialFleet() {
        return imperialCounter;
    }

    size_t countRebelFleet() {
        return rebelCounter;
    }

    void tick(T timeStep) {
        if (std::binary_search(attacks.begin(), attacks.end(), currentTime))
            battleSimulation();
        currentTime = (currentTime + timeStep) % (t1 + 1);
    }

private:
    template <size_t n = 0, typename I>
    void countTheShips(ImperialStarship <I>) {
        imperialCounter++;
        if constexpr (n + 1 < sizeof...(Ships))
            countTheShips <n + 1>(std::get <n + 1>(shipsInBattle));
    }

    template <size_t n = 0, typename R, int minSpeed, int maxSpeed, bool canAttack>
    void countTheShips(RebelStarship <R, minSpeed, maxSpeed, canAttack>) {
        rebelCounter++;
        if constexpr (n + 1 < sizeof...(Ships))
            countTheShips <n + 1>(std::get <n + 1>(shipsInBattle));
    }

    template <size_t n = 0, typename I, typename R>
    constexpr void attackRebels(ImperialStarship <I> &imperial, R &ships) {
        if constexpr (n + 1 < sizeof...(Ships))
            attackRebels <n + 1>(imperial, std::get <n + 1>(shipsInBattle));
    }

    template <size_t n = 0, typename I, typename R, int minSpeed, int maxSpeed, bool canAttack>
    constexpr void attackRebels(
        ImperialStarship <I> &imperial, RebelStarship <R, minSpeed, maxSpeed, canAttack> &rebel) {
        if (rebel.getShield() != 0 && imperial.getShield() != 0) {
            attack(imperial, rebel);
            if (rebel.getShield() == 0)
                rebelCounter--;
            if (imperial.getShield() == 0)
                imperialCounter--;
        }

        if constexpr (n + 1 < sizeof...(Ships))
            attackRebels <n + 1>(imperial, std::get <n + 1>(shipsInBattle));
    }

    template <size_t n = 0, typename I>
    constexpr void imperiumAttacks(ImperialStarship <I> &imperial) {
        if (imperial.getShield() != 0)
            attackRebels(imperial, std::get <0>(shipsInBattle));

        if constexpr (n + 1 < sizeof...(Ships))
            imperiumAttacks <n + 1>(std::get <n + 1>(shipsInBattle));
    }

    template <size_t n = 0, typename I>
    constexpr void imperiumAttacks(I &ship) {
        if constexpr (n + 1 < sizeof...(Ships))
            imperiumAttacks <n + 1>(std::get <n + 1>(shipsInBattle));
    }

    void battleSimulation() {
        if constexpr (sizeof...(Ships) != 0) {
            imperiumAttacks <0>(std::get <0>(shipsInBattle));
            if (imperialCounter == 0 && rebelCounter == 0) {
                printf("DRAW\n");
            } else if (rebelCounter == 0) {
                printf("IMPERIUM WON\n");
            } else if (imperialCounter == 0) {
                printf("REBELLION WON\n");
            }
        }
    }
};

#endif
