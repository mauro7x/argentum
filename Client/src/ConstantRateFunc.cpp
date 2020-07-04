#include "../includes/ConstantRateFunc.h"

//-----------------------------------------------------------------------------
// Métodos privados

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

ConstantRateFunc::ConstantRateFunc() : exit(false) {
    json config = JSON::loadJsonFile(paths::config(CONFIG_FILEPATH));
    this->rate = 1000 / (int)config["fps"];
}

void ConstantRateFunc::run() {
    // Variables para controlar el frame-rate
    auto t1 = std::chrono::steady_clock::now();
    auto t2 = t1;
    std::chrono::duration<float, std::milli> diff;
    int rest = 0, behind = 0, lost = 0;
    int it = 1;

    // Loop principal
    while (!exit) {
        _func(it);

        // Controlamos el rate y verificamos pérdida de frames.
        // Idea de implementación:
        // https://eldipa.github.io/book-of-gehn/articles/2019/10/23/Constant-Rate-Loop.html
        it = 0;
        t2 = std::chrono::steady_clock::now();
        diff = t2 - t1;
        rest = rate - std::ceil(diff.count());

        if (rest < 0) {
            fprintf(stderr, ">> ConstantRateFunc: pérdida de frames.\n");
            behind = -rest;
            lost = rate + (behind - behind % rate);
            rest = rate - behind % rate;
            t1 += std::chrono::milliseconds(lost);
            it += std::floor(lost / rate);
        }

        // fprintf(stderr, "MAIN-LOOP: Sleeping for %i ms.\n", rest);
        std::this_thread::sleep_for(std::chrono::milliseconds(rest));
        t1 += std::chrono::milliseconds(rate);
        it += 1;
    }
}

void ConstantRateFunc::quit() {
    this->exit = true;
}

ConstantRateFunc::~ConstantRateFunc() {}

//-----------------------------------------------------------------------------
