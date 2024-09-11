#include "ProcessEdit.h"

#include "../Memory.h"
#include "../Tools.h"
#include "ProcessMenu.h"

namespace Menu {

ProcessEdit::ProcessEdit(const ProgDesc& progDesc) : m_progDesc(progDesc) {
    auto printer = [](void* ctx_, uint8_t id, uint8_t line) {
        auto ctx = reinterpret_cast<ProcessEdit*>(ctx_);
        bool choosen = id == ctx->m_listSelector.pos();

        ProgDesc::Action action = ctx->m_progDesc.steps[id].action;

        if (choosen && ctx->m_step == Step::editAction) {
            action = ctx->m_newAction;
            gDisplay[line].printBlink(ProgDesc::getActionName(action));
        } else {
            gDisplay[line] << ProgDesc::getActionName(action);
        }

        if (ctx->m_progDesc.actionSupportTime(action)) {
            char formatedTime[6];
            formatTime(ctx->m_progDesc.steps[id].time, formatedTime);
            if (choosen && ctx->m_step == Step::editTime)
                gDisplay[line].printBlink(formatedTime, true);
            else
                gDisplay[line] >> formatedTime;
        }
    };

    auto maxGetter = [](void* ctx_) -> uint8_t {
        auto ctx = reinterpret_cast<ProcessEdit*>(ctx_);
        if (ctx->m_step == Step::editAction && ctx->m_newAction == ProgDesc::Action::Finish)
            return ctx->m_listSelector.pos() + 1;

        return ctx->m_progDesc.numberOfSteps();
    };

    m_listSelector = ListSelector(printer, maxGetter, this);
}

void ProcessEdit::tick() {
    uint8_t currentChem = m_listSelector.pos();
    if (m_step == Step::select)
        m_listSelector.shift(getEncoderDir());

    if (m_step == Step::editAction) {
        auto shift = getEncoderDir();
        if (shift)
            gDisplay.resetBlink();

        m_newAction = ADD_TO_ENUM(ProgDesc::Action, m_newAction, shift);
    }

    if (m_step == Step::editTime) {
        if (getTime(m_progDesc.steps[currentChem].time))
            gDisplay.resetBlink();
    }

    m_listSelector.tick();

    if (gModeSwitchBtn.click()) {
        // last elem should be finish
        if (currentChem == ARRAY_SIZE(m_progDesc.steps) - 1)
            return;
        if (m_step == Step::editAction) {
            if (m_progDesc.steps[currentChem].action == ProgDesc::Action::Finish) {
                m_progDesc.steps[currentChem + 1].action = ProgDesc::Action::Finish;
                m_progDesc.steps[currentChem + 1].time = 0;
            }

            m_progDesc.steps[currentChem].action = m_newAction;
        }

        m_step = ADD_TO_ENUM(Step, m_step, 1);

        if (m_step == Step::editTime && !m_progDesc.stepSupportTime(currentChem))
            m_step = ADD_TO_ENUM(Step, m_step, 1);
        if (m_step == Step::editAction)
            m_newAction = m_progDesc.steps[currentChem].action;
    }

    if (gBackBtn.click()) {
        gMemory.saveProg(m_progDesc);
        gApp.setMenu(new ProcessMenu(m_progDesc));
        return;
    }
}

} // namespace Menu
