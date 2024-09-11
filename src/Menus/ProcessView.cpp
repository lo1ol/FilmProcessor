#include "ProcessView.h"

#include "../Memory.h"
#include "../Tools.h"
#include "ProcessMenu.h"

namespace Menu {

ProcessView::ProcessView() {
    PageViewer::Printer printer = [](void*, uint8_t i, uint8_t line) {
        auto& prog = gMemory.getProg();
        gDisplay[line] << prog.getStepName(i);
        if (prog.stepSupportTime(i)) {
            char formatedTime[6];
            formatTime(prog.steps[i].time, formatedTime);

            gDisplay[line] >> formatedTime;
        }
    };

    auto maxGetter = [](void*) -> uint8_t { return gMemory.getProg().numberOfSteps() - 1; };

    m_pageViewer = PageViewer(printer, maxGetter, this);
}

void ProcessView::tick() {
    m_pageViewer.shift(getEncoderDir());
    m_pageViewer.tick();

    if (gBackBtn.click()) {
        gApp.setMenu(new ProcessMenu());
        return;
    }
}

} // namespace Menu
