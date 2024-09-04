#pragma once

class BaseMenu {
public:
    virtual void tick() = 0;
    virtual ~BaseMenu() {}
};
