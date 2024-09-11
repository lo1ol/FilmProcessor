#pragma once

#include "../BaseMenu.h"
#include "../PageViewer.h"

namespace Menu {

class ProcessView : public BaseMenu {
public:
    ProcessView();
    void tick() override;

private:
    PageViewer m_pageViewer;
};

} // namespace Menu
