#include "ProcessView.h"

#include "../Memory.h"
#include "../Tools.h"
#include "ProcessMenu.h"

namespace Menu {

ProcessView::ProcessView(const ProgDesc& progDesc) : m_progDesc(progDesc), m_pageViewer(progDesc.numberOfSteps() - 1) {}

void ProcessView::tick() {
    m_pageViewer.shift(getEncoderDir());

    for (uint8_t id = 0, i = m_pageViewer.low(); i != m_pageViewer.high(); ++i, ++id) {
        gDisplay[id] << m_progDesc.getStepName(i);
        gDisplay[id] >> formatTime(m_progDesc.steps[i].time);
    }

    if (gBackBtn.click()) {
        gApp.setMenu(new ProcessMenu(m_progDesc));
        return;
    }
}

} // namespace Menu
