#include "ProcessSettings.h"

#include "../Settings.h"
#include "../Tools.h"
#include "ProcessExecutor.h"
#include "ProcessMenu.h"

namespace Menu {

const char* ProcessSettings::getSettingName(Setting setting) {
    switch (setting) {
    case Setting::Agitation:
        return "Agitation";
    case Setting::Volume:
        return "Volume";
    case Setting::Start:
        return "Start";
    default:
        return "";
    }

    return "";
}

const char* ProcessSettings::getSettingValue(Setting setting) {
    switch (setting) {
    case Setting::Agitation:
        return ::ProcessSettings::getAgitationName(m_processSettings.agitation);
    case Setting::Volume:
        return ::ProcessSettings::getVolumeName(m_processSettings.volume);
    default:
        return "";
    }
}

void ProcessSettings::changeValue(Setting setting, int8_t shift) {
    gDisplay.resetBlink();

    switch (setting) {
    case Setting::Agitation:
        m_processSettings.agitation = ADD_TO_ENUM(::ProcessSettings::Agitation, m_processSettings.agitation, shift);
        break;
    case Setting::Volume:
        if (shift < 0)
            m_processSettings.volume =
                static_cast<::ProcessSettings::Volume>(static_cast<int>(m_processSettings.volume) - 100);

        if (shift > 0)
            m_processSettings.volume =
                static_cast<::ProcessSettings::Volume>(static_cast<int>(m_processSettings.volume) + 100);

        if (m_processSettings.volume < ::ProcessSettings::Volume::ml300)
            m_processSettings.volume = ::ProcessSettings::Volume::ml300;

        if (m_processSettings.volume > ::ProcessSettings::Volume::ml700)
            m_processSettings.volume = ::ProcessSettings::Volume::ml700;
        break;
    case Setting::Start:
    case Setting::last_:
        break;
    }
}

ProcessSettings::ProcessSettings() {
    auto printer = [](void* ctx_, uint8_t id, uint8_t line) {
        auto ctx = reinterpret_cast<ProcessSettings*>(ctx_);
        const char* settingValue = ctx->getSettingValue(static_cast<Setting>(id));
        bool choosen = id == ctx->m_listSelector.pos();

        gDisplay[line] << getSettingName(static_cast<Setting>(id));

        if (choosen && ctx->m_phase == Phase::OnSet)
            gDisplay[line].printBlink(settingValue, true);
        else
            gDisplay[line] >> settingValue;
    };

    auto maxGetter = [](void*) { return static_cast<uint8_t>(Setting::last_); };

    m_listSelector = ListSelector(printer, maxGetter, this);

    m_processSettings = gMemory.getSettings().lastProcessSettings;
}

void ProcessSettings::tick() {
    if (m_phase == Phase::OnStart) {
        m_confirmAsker.tick();
        if (m_confirmAsker.finish()) {
            if (m_confirmAsker.result()) {
                auto settings = gMemory.getSettings();
                settings.lastProcessSettings = m_processSettings;
                gMemory.saveSettings(settings);
                gApp.setMenu(new ProcessExecutor());
            } else {
                m_phase = Phase::OnChoose;
            }
        }
        return;
    }

    if (m_phase == Phase::OnChoose)
        m_listSelector.shift(getEncoderDir());

    Setting setting = static_cast<Setting>(m_listSelector.pos());

    if (m_phase == Phase::OnSet) {
        auto shift = getEncoderDir();
        if (shift)
            changeValue(setting, shift);
    }

    m_listSelector.tick();

    if (gModeSwitchBtn.click()) {
        gDisplay.resetBlink(true);
        if (setting == Setting::Start) {
            m_phase = Phase::OnStart;
            m_confirmAsker = ConfirmAsker("Run process");
        } else if (m_phase == Phase::OnChoose) {
            m_phase = Phase::OnSet;
        } else if (m_phase == Phase::OnSet) {
            m_phase = Phase::OnChoose;
        }
    }

    if (gBackBtn.click()) {
        if (m_phase != Phase::OnChoose) {
            m_phase = Phase::OnChoose;
            return;
        }

        gApp.setMenu(new ProcessMenu());
        return;
    }
}

} // namespace Menu
