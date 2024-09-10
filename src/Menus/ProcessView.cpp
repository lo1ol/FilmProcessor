#include "ProcessView.h"

#include "../Memory.h"
#include "../Tools.h"
#include "ProcessMenu.h"

namespace Menu {

ProcessView::ProcessView(const ProgDesc& progDesc) : m_progDesc(progDesc) {
    PageViewer::Printer printer = [](void* ctx_, uint8_t i, uint8_t line) {
        auto ctx = reinterpret_cast<ProcessView*>(ctx_);
        gDisplay[line] << ctx->m_progDesc.getStepName(i);
        if (ctx->m_progDesc.stepSupportTime(i)) {
            char formatedTime[6];
            formatTime(ctx->m_progDesc.steps[i].time, formatedTime);

            gDisplay[line] >> formatedTime;
        }
    };

    auto maxGetter = [](void* ctx_) -> uint8_t {
        auto ctx = reinterpret_cast<ProcessView*>(ctx_);
        return ctx->m_progDesc.numberOfSteps() - 1;
    };

    m_pageViewer = PageViewer(printer, maxGetter, this);
}

void ProcessView::tick() {
    m_pageViewer.shift(getEncoderDir());
    m_pageViewer.tick();

    if (gBackBtn.click()) {
        gApp.setMenu(new ProcessMenu(m_progDesc));
        return;
    }
}

} // namespace Menu
