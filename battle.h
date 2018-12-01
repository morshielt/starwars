#ifndef BATTLE_H
#define BATTLE_H

#include <iostream>
#include <tuple>
#include <cstdio>
#include <algorithm>
#include <iostream> //do usunięcia potem <chyba>
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
public:
    std::tuple<Ships...> shipsInBattle;
    static constexpr auto attacks = square_array <T, t1 + 1>();

    SpaceBattle(Ships ...ships) : shipsInBattle(ships...) {
        printf("rozmiar: %lu\n", sizeof...(Ships));
        static_assert(t0 > 0, "Invalid t0");
        static_assert(t1 >= t0, "Invalid t1");
        current_time = t0;
        imperialCounter = 0;
        rebelCounter = 0;
        if constexpr (sizeof...(Ships) != 0) {
            countTheShips(std::get<0>(shipsInBattle));
        }
    }

private:
    T current_time;

    template<size_t n = 0, typename I>
    void countTheShips(ImperialStarship<I>) {
        imperialCounter++;
        if constexpr (n + 1 < sizeof...(Ships)) {
            countTheShips<n + 1>(std::get<n + 1>(shipsInBattle));
        }
    }

    template<size_t n = 0, typename R, int minSpeed, int maxSpeed, bool canAttack>
    void countTheShips(RebelStarship<R, minSpeed, maxSpeed, canAttack>) {
        rebelCounter++;
        if constexpr (n + 1 < sizeof...(Ships)) {
            countTheShips<n + 1>(std::get<n + 1>(shipsInBattle));
        }
    }

    template<size_t n = 0, typename I, typename R>
    constexpr void drugaPetla(ImperialStarship<I> &imperial, R &ships) {
        printf("druga imperial: %lu\n", n);
        if constexpr (n + 1 < sizeof...(Ships)) {
            drugaPetla<n + 1>(imperial, std::get<n + 1>(shipsInBattle));
        }
    }


    template<size_t n = 0, typename I, typename R, int minSpeed, int maxSpeed, bool canAttack>
    constexpr void drugaPetla(ImperialStarship<I> &imperial, RebelStarship<R, minSpeed, maxSpeed, canAttack> &rebel) {
        printf("druga rebel: %lu\n", n);
        if (rebel.getShield() != 0 && imperial.getShield() != 0)  {
            std::cout << "rebel shield przed " << rebel.getShield() << "\n";
            std::cout << "imperial attack power : " << imperial.getAttackPower() << "\n";

            attack(imperial, rebel);
            std::cout << "rebel shield po " << rebel.getShield() << "\n";

            if (rebel.getShield() == 0) {
                std::cout << "rebel DEAD\n";
                rebelCounter--;
            }
            if (imperial.getShield() == 0) {
                std::cout << "imperial DEAD\n";
                imperialCounter--;
            }
        }

        if constexpr (n + 1 < sizeof...(Ships)) {
            drugaPetla<n + 1>(imperial, std::get<n + 1>(shipsInBattle));
        }
    }

    template<size_t n = 0, typename I>
    constexpr void pierwszaPetla(ImperialStarship<I> &imperial) {
        printf("pierwsza imperial: %lu\n", n);
        if (imperial.getShield() != 0) {
            drugaPetla(imperial, std::get<0>(shipsInBattle));
        }
        if constexpr (n + 1 < sizeof...(Ships)) {
            pierwszaPetla<n + 1>(std::get<n + 1>(shipsInBattle));
        }
    }

    template<size_t n = 0, typename I>
    constexpr void pierwszaPetla(I &ship) {
        printf("pierwsza rebel: %lu\n", n);
        if constexpr (n + 1 < sizeof...(Ships)) {
            pierwszaPetla<n + 1>(std::get<n + 1>(shipsInBattle));
        }
    }

public:
    size_t countImperialFleet() {
        return imperialCounter;
    }

    size_t countRebelFleet() {
        return rebelCounter;
    }


    void battleSimulation() {
        //attack(std::get<0>(shipsInBattle), std::get<1>(shipsInBattle));
        //XpierwszaPetla<0, Ships...>(shipsInBattle);
        if constexpr (sizeof...(Ships) != 0) {
            pierwszaPetla<0>(std::get<0>(shipsInBattle));
            if (imperialCounter == 0 && rebelCounter == 0) {
                printf("DRAW\n");
            } else {
                if (rebelCounter == 0) {
                    printf("IMPERIUM WON\n");
                }
                if (imperialCounter == 0) {
                    printf("REBELLION WON\n");
                }
            }
        }
    }

    void tick(T timeStep) {
        if (std::binary_search(attacks.begin(), attacks.end(), current_time)) {
            std::cout << "Atakujemy w " << current_time << '\n';
            battleSimulation();
            //pierwszaPetla <0, Ships...>(shipsInBattle);
            //tu dopisać atakowanie z iteracją po tupli
        } else {
            std::cout << "NIE atakujemy w " << current_time << '\n';
        }
        current_time = (current_time + timeStep) % (t1 + 1);
    }
};

#endif
