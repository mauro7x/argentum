#include "../includes/ConstantRateFunc.h"

//-----------------------------------------------------------------------------
// Métodos privados

void ConstantRateFunc::_updateIt(int& it) {
    uint32_t ms = (it * rate) + delta_ms;
    it = ms / original_rate;
    delta_ms = ms % original_rate;
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// API Pública

ConstantRateFunc::ConstantRateFunc()
    : original_rate(ORIGINAL_RATE), delta_ms(0), exit(false) {
    json config = JSON::loadJsonFile(paths::config(CONFIG_FILEPATH));
    int fps = config["fps"];

    if (fps <= 0 || fps > MAX_FPS_ALLOWED) {
        throw Exception(
            "ConstantRateFunc::ConstantRateFunc: invalid fps in config file.");
    }

    rate = 1000 / (int)config["fps"];
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
        _updateIt(it);
        _func(it);

        // Controlamos el rate y verificamos pérdida de frames.
        // Idea de implementación:
        // https://eldipa.github.io/book-of-gehn/articles/2019/10/23/Constant-Rate-Loop.html
        it = 0;
        t2 = std::chrono::steady_clock::now();
        diff = t2 - t1;
        rest = rate - std::ceil(diff.count());

        if (rest < 0) {
            fprintf(stderr, ">> Ciclo principal: pérdida de frame/s.\n");
            behind = -rest;
            lost = rate + (behind - behind % rate);
            rest = rate - behind % rate;
            t1 += std::chrono::milliseconds(lost);
            it += std::floor(lost / rate);
        }

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
