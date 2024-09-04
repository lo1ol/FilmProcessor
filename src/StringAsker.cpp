#include "StringAsker.h"

#include "Tools.h"

StringAsker::StringAsker(const char* greeting, const String& base) : m_result(base), m_greeting(greeting), m_pos(0) {
    if (base.length())
        m_result += ' ';
}

void StringAsker::tick() {
    gDisplay[0] << m_greeting << m_result;
    if (m_result.length() < 10)
        gDisplay[0].printBlink('_');

    auto shift = getEncoderShift();
    if (shift) {
        gDisplay[1].resetBlink(true, 200);
        gDisplay[2].resetBlink(true, 200);
        gDisplay[3].resetBlink(true, 200);
    }

    if (shift < 0 && m_pos < -shift)
        m_pos = 0;
    else
        m_pos += shift;

    if (m_pos >= 13 * 2 + 12 + 1)
        m_pos = 13 * 2 + 12 + 1;

    uint8_t line = 0;
    uint8_t linePos = 0;
    uint8_t i = 0;
    char choosenSym = 0;
    const char* keyboard[] = { "abcdefghijklm", "nopqrstuvwxyz", "1234567890-_  \02\01", nullptr };

    while (keyboard[line]) {
        char sym = keyboard[line][linePos];
        if (sym == 0) {
            linePos = 0;
            line++;
            continue;
        }

        if (sym == ' ') {
            gDisplay[line + 1] << sym;
            ++linePos;
            continue;
        }

        if (m_pos == i) {
            gDisplay[line + 1].printBlink(sym);
            choosenSym = sym;
        } else {
            gDisplay[line + 1] << sym;
        }

        ++linePos;
        ++i;
    }

    if (gModeSwitchBtn.click()) {
        if (choosenSym == kBackSymbol) {
            if (m_result.length())
                m_result.remove(m_result.length() - 1);
        } else if (choosenSym == kAcceptSymbol) {
            m_finish = true;
        } else if (m_result.length() < 10) {
            if (choosenSym == '_')
                choosenSym = ' ';
            m_result += choosenSym;
        }
    }
}
