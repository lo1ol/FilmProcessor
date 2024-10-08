#include "ConfirmAsker.h"

#include <string.h>

#include "Tools.h"

ConfirmAsker::ConfirmAsker(const char* greeting, const char* question, Type type)
    : m_greeting(greeting), m_question(question), m_type(type) {}

void ConfirmAsker::tick() {
    auto shift = getEncoderDir();
    if (shift) {
        m_result = !m_result;
        gDisplay.resetBlink(true);
    }

    gDisplay[0] << m_greeting;

    switch (m_type) {
    case Type::HoldConfirm:
        gDisplay[1] << "Hold to confirm";
        gDisplay[3].printBlink("       Ok");
        if (gModeSwitchBtn.hold())
            m_finish = true;
        break;
    case Type::ClickConfirm:
        gDisplay[3].printBlink("       Ok");
        if (gModeSwitchBtn.click())
            m_finish = true;
        break;
    case Type::YesNo:
        gDisplay[1] << m_question;
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
        break;
    }
}
