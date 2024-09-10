#pragma once

#include "Config.h"

#include <stdint.h>

class PageViewer {
public:
    using Printer = void (*)(void* ctx, uint8_t i, uint8_t line);
    using MaxGetter = uint8_t (*)(void* ctx);

    PageViewer() = default;
    PageViewer(Printer, MaxGetter, void* ctx);

    void tick();
    void shift(int8_t);

private:
    uint8_t m_low = 0;
    Printer m_printer;
    MaxGetter m_maxGetter;
    void* m_ctx;
};
