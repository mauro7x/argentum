#include <random>

#include "../includes/RandomNumberGenerator.h"

RandomNumberGenerator::RandomNumberGenerator() {}
RandomNumberGenerator::~RandomNumberGenerator() {}

int RandomNumberGenerator::operator()(int min, int max) {
    // Obtengo numero random del HW
    std::random_device rd;
    // Agrego seed al generador
    std::mt19937 generator(rd());
    // Defino el rango
    std::uniform_int_distribution<> distribution(min, max);
    return distribution(generator);
}
