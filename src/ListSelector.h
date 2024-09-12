#pragma once

#include "Config.h"

#include <stdint.h>

class ListSelector {
public:
    using Printer = void (*)(void* ctx, uint8_t i, uint8_t line);
    using MaxGetter = uint8_t (*)(void* ctx);

    ListSelector() = default;
    ListSelector(Printer, MaxGetter, void* ctx, const char* header = nullptr, uint8_t pos = 0);

    void shift(int8_t);
    void tick();
    uint8_t pos() const { return m_pos; }

private:
    int8_t m_low;
    int8_t m_pos;
    Printer m_printer;
    MaxGetter m_maxGetter;
    void* m_ctx;
    const char* m_header;
};
