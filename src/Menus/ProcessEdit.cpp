#include "ProcessEdit.h"

#include "../Memory.h"
#include "../Tools.h"
#include "ProcessMenu.h"

namespace Menu {

ProcessEdit::ProcessEdit() {
    auto printer = [](void* ctx_, uint8_t id, uint8_t line) {
        auto ctx = reinterpret_cast<ProcessEdit*>(ctx_);
        bool choosen = id == ctx->m_listSelector.pos();

        const auto& prog = gMemory.getProg();
        ProgDesc::Action action = prog.steps[id].action;

        if (choosen && ctx->m_step == Step::editAction) {
            action = ctx->m_newAction;
            gDisplay[line].printBlink(ProgDesc::getActionName(action));
        } else {
            gDisplay[line] << ProgDesc::getActionName(action);
        }

        if (prog.actionSupportTime(action)) {
            char formatedTime[6];
            formatTime(prog.steps[id].time, formatedTime);
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

        return gMemory.getProg().numberOfSteps();
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

    auto& prog = gMemory.getProg();

    if (m_step == Step::editTime) {
        if (getTime(prog.steps[currentChem].time))
            gDisplay.resetBlink();
    }

    m_listSelector.tick();

    if (gModeSwitchBtn.click()) {
        // last elem should be finish
        if (currentChem == ARRAY_SIZE(prog.steps) - 1)
            return;
        if (m_step == Step::editAction) {
            if (prog.steps[currentChem].action == ProgDesc::Action::Finish) {
                prog.steps[currentChem + 1].action = ProgDesc::Action::Finish;
                prog.steps[currentChem + 1].time = 0;
            }

            prog.steps[currentChem].action = m_newAction;
        }

        m_step = ADD_TO_ENUM(Step, m_step, 1);

        if (m_step == Step::editTime && !prog.stepSupportTime(currentChem))
            m_step = ADD_TO_ENUM(Step, m_step, 1);

        if (m_step == Step::editAction) {
            m_newAction = prog.steps[currentChem].action;
            if (!prog.stepSupportTime(currentChem))
                prog.steps[currentChem].time = 0;
        }
    }

    if (gBackBtn.click()) {
        gMemory.saveProg();
        gApp.setMenu(new ProcessMenu());
        return;
    }
}

} // namespace Menu
