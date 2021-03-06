#ifndef __RANDOM_NUMBER_GENERATOR_H__
#define __RANDOM_NUMBER_GENERATOR_H__

class RandomNumberGenerator {
   public:
    RandomNumberGenerator();
    ~RandomNumberGenerator();

    float operator()(float min, float max) const;
    int operator()(int min, int max) const;
};

#endif
