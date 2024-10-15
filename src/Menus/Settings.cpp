#include "Settings.h"

#include <stdint.h>

#include "../Tools.h"
#include "MainMenu.h"

namespace Menu {

const char* Settings::getSettingName(Setting setting) {
    switch (setting) {
    case Setting::Melody:
        return "Song";
    case Setting::Agitation:
        return "Agitation";
    default:
        return "";
    }
}

const char* Settings::getSettingValue(Setting setting) {
    switch (setting) {
    case Setting::Melody:
        return Melody::getSongName(m_settings.songId);
    case Setting::Agitation:
        return "Jobo";
    default:
        return "";
    }
}

void Settings::changeValue(Setting setting, int8_t shift) {
    gDisplay.resetBlink();

    switch (setting) {
    case Setting::Melody:
        gMelodyPlayer.stop();
        m_settings.songId = ADD_TO_ENUM(SongId, m_settings.songId, shift);
        gMelodyPlayer.setSong(m_settings.songId);
        gMelodyPlayer.start();
        break;
    case Setting::Agitation:
        break;
    case Setting::last_:
        break;
    }

    gMemory.saveSettings(m_settings);
    m_settings.applySettings();
}

Settings::Settings() {
    m_settings = gMemory.getSettings();
    auto printer = [](void* ctx_, uint8_t id, uint8_t line) {
        auto ctx = reinterpret_cast<Settings*>(ctx_);
        const char* settingName = getSettingName(static_cast<Setting>(id));
        const char* settingValue = ctx->getSettingValue(static_cast<Setting>(id));
        bool choosen = id == ctx->m_listSelector.pos();

        if (choosen && ctx->m_phase == Phase::OnChoose)
            gDisplay[line].printBlink(settingName);
        else
            gDisplay[line] << settingName;

        if (choosen && ctx->m_phase == Phase::OnSet)
            gDisplay[line].printBlink(settingValue, true);
        else
            gDisplay[line] >> settingValue;
    };

    auto maxGetter = [](void*) { return static_cast<uint8_t>(Setting::last_); };

    m_listSelector = ListSelector(printer, maxGetter, this);
}

void Settings::tick() {
    if (m_phase == Phase::OnChoose)
        m_listSelector.shift(getEncoderDir());

    Setting setting = static_cast<Setting>(m_listSelector.pos());

    if (m_phase == Phase::OnSet) {
        auto shift = getEncoderDir();
        if (shift)
            changeValue(setting, shift);

        // TODO
    }

    m_listSelector.tick();

    if (gModeSwitchBtn.click())
        m_phase = ADD_TO_ENUM(Phase, m_phase, 1);

    if (gBackBtn.click()) {
        if (m_phase == Phase::OnSet) {
            m_phase = Phase::OnChoose;
            return;
        }

        gApp.setMenu(new MainMenu());
        return;
    }
}

} // namespace Menu
