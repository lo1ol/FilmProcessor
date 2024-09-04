#include "ProcessMenu.h"

#include "../Memory.h"
#include "../Tools.h"

namespace Menu {

namespace {
constexpr const char* kActions[] = { "Process", "View", "Edit", "Create based on", "Delete" };
} // namespace

ProcessMenu::ProcessMenu(const ProgDesc& progDesc) : m_progDesc(progDesc), m_listViewer(ARRAY_SIZE(kActions)) {}

void ProcessMenu::tick() {
    m_listViewer.shift(getEncoderDir());

    for (uint8_t id = 0, i = m_listViewer.low(); i != m_listViewer.high(); ++i, ++id) {
        gDisplay[id] << (m_listViewer.choosen(i) ? ">" : " ");
        gDisplay[id] << kActions[i];
    }
}

} // namespace Menu
