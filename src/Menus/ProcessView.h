#pragma once

#include "../BaseMenu.h"
#include "../PageViewer.h"
#include "../ProgDesc.h"

namespace Menu {

class ProcessView : public BaseMenu {
public:
    ProcessView(const ProgDesc&);
    void tick() override;

private:
    ProgDesc m_progDesc;
    PageViewer m_pageViewer;
};

} // namespace Menu
