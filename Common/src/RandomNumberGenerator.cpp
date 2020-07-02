#include "../includes/RandomNumberGenerator.h"

#include <random>

RandomNumberGenerator::RandomNumberGenerator() {}
RandomNumberGenerator::~RandomNumberGenerator() {}

float RandomNumberGenerator::operator()(float min, float max) {
    // Obtengo numero random del HW
    std::random_device rd;
    // Agrego seed al generador
    std::mt19937 generator(rd());
    // Defino el rango
    std::uniform_real_distribution<> distribution(min, max);
    return distribution(generator);
}

int RandomNumberGenerator::operator()(int min, int max) {
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<> distribution(min, max);
    return distribution(generator);
}
