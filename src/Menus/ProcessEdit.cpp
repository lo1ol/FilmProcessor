#include "ProcessEdit.h"

#include "../Memory.h"
#include "../Tools.h"

namespace Menu {

ProcessEdit::ProcessEdit(const ProgDesc& progDesc) : m_progDesc(progDesc), m_listViewer(progDesc.numberOfSteps()) {}

void ProcessEdit::tick() {
    if (m_step == Step::select)
        m_listViewer.shift(getEncoderDir());

    uint8_t currentChem = m_listViewer.pos();

    for (uint8_t id = 0, i = m_listViewer.low(); i != m_listViewer.high(); ++i, ++id) {
        gDisplay[id] << (m_listViewer.choosen(i) ? ">" : " ");

        if (!m_listViewer.choosen(i) || m_step == Step::select) {
            gDisplay[id] << m_progDesc.getStepName(i);

            if (m_progDesc.stepSupportTime(i))
                gDisplay[id] >> formatTime(m_progDesc.steps[i].time);
            continue;
        }

        if (m_step == Step::editAction) {
            auto shift = getEncoderDir();
            if (shift)
                gDisplay.resetBlink();

            m_progDesc.steps[currentChem].action =
                ADD_TO_ENUM(ProgDesc::Action, m_progDesc.steps[currentChem].action, shift);
            gDisplay[id].printBlink(m_progDesc.getStepName(i));
            if (m_progDesc.stepSupportTime(i))
                gDisplay[id] >> formatTime(m_progDesc.steps[i].time);
        }

        if (m_step == Step::editTime) {
            if (getTime(m_progDesc.steps[currentChem].time))
                gDisplay.resetBlink();
            gDisplay[id] << m_progDesc.getStepName(i);
            gDisplay[id].printBlink(formatTime(m_progDesc.steps[i].time), true);
        }
    }

    if (gModeSwitchBtn.click()) {
        // last elem should be finish
        if (currentChem == ARRAY_SIZE(m_progDesc.steps) - 1)
            return;

        if (m_step == Step::editAction) {
            if (m_changeEnd) {
                m_progDesc.steps[currentChem + 1].action = ProgDesc::Action::Finish;
                m_progDesc.steps[currentChem + 1].time = 0;
            }

            m_listViewer.setMax(m_progDesc.numberOfSteps());
        }

        m_step = ADD_TO_ENUM(Step, m_step, 1);

        if (m_step == Step::editTime && !m_progDesc.stepSupportTime(currentChem))
            m_step = ADD_TO_ENUM(Step, m_step, 1);
        if (m_step == Step::editAction) {
            m_changeEnd = m_progDesc.steps[currentChem].action == ProgDesc::Action::Finish;
        }
    }
}

} // namespace Menu
