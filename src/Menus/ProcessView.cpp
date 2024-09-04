#include "ProcessView.h"

#include "../Memory.h"
#include "../Tools.h"

namespace Menu {

ProcessView::ProcessView(const ProgDesc& progDesc) : m_progDesc(progDesc), m_listViewer(progDesc.numberOfSteps()) {}

void ProcessView::tick() {
    m_listViewer.shift(getEncoderDir());

    for (uint8_t id = 0, i = m_listViewer.low(); i != m_listViewer.high(); ++i, ++id) {
        gDisplay[id] << (m_listViewer.choosen(i) ? ">" : " ");
        gDisplay[id] << m_progDesc.getStepName(i);
        gDisplay[id] >> formatTime(m_progDesc.steps[i].time);
    }
}

} // namespace Menu
