#include "Memory.h"

ProgDesc gProgs[] = {
    { "c-41",
      {
          { ProgDesc::Action::Dev, 240 },
          { ProgDesc::Action::Bleach, 360 },
          { ProgDesc::Action::Fix, 240 },
          {
              ProgDesc::Action::Finish,
          },
      } },
    { "kek1",
      {
          { ProgDesc::Action::Finish, 240 },
      } },
    { "kek2",
      {
          { ProgDesc::Action::Finish, 240 },
      } },
    { "kek3",
      {
          { ProgDesc::Action::Finish, 240 },
      } },
    { "kek4",
      {
          { ProgDesc::Action::Finish, 240 },
      } },
};

uint8_t Memory::getProgNum() {
    return 5;
}

ProgDesc Memory::getProg(uint8_t i) {
    return gProgs[i];
}
