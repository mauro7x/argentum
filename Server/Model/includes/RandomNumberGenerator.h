#ifndef __RANDOM_NUMBER_GENERATOR_H__
#define __RANDOM_NUMBER_GENERATOR_H__

class RandomNumberGenerator {
    public:
        RandomNumberGenerator();
        ~RandomNumberGenerator();

        unsigned int operator()(unsigned int min, unsigned int max);
};

#endif
