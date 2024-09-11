#include "ConfirmAsker.h"

#include <string.h>

#include "Tools.h"

ConfirmAsker::ConfirmAsker(const char* greeting) : m_greeting(greeting) {}

void ConfirmAsker::tick() {
    auto shift = getEncoderDir();
    if (shift) {
        m_result = !m_result;
        gDisplay.resetBlink(true);
    }

    gDisplay[0] << m_greeting;
    gDisplay[3] << "   ";
    if (!m_result)
        gDisplay[3].printBlink("No");
    else
        gDisplay[3] << "No";

    gDisplay[3] << "     ";
    if (m_result)
        gDisplay[3].printBlink("Yes");
    else
        gDisplay[3] << "Yes";

    if (gModeSwitchBtn.click())
        m_finish = true;
}
