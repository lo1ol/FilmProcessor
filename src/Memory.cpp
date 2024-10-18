#include "Memory.h"

#include <ArduinoJson.h>
#include <EEPROM.h>

#include <assert.h>

#include "Tools.h"

#define SETTINGS_SIZE 100

static_assert(sizeof(Settings) <= SETTINGS_SIZE);

Memory::Memory() {
    m_progNum = 0;
    while (true) {
        ProgDesc progDesc;
        EEPROM.get(SETTINGS_SIZE + sizeof(progDesc) * m_progNum, progDesc);

        bool hasEnd = false;
        for (uint8_t i = 0; i != sizeof(progDesc.name); ++i) {
            if (!progDesc.name[i]) {
                hasEnd = true;
                break;
            }
        }

        if (!hasEnd) {
            progDesc.name[0] = 0;
            EEPROM.put(SETTINGS_SIZE, progDesc);
            return;
        }

        if (progDesc.name[0] == 0)
            return;

        ++m_progNum;
    }
}

void Memory::saveProg() {
    uint8_t id = 0;
    while (true) {
        ProgDesc progDesc;
        EEPROM.get(SETTINGS_SIZE + sizeof(progDesc) * id, progDesc);

        if (progDesc.name[0] == 0) {
            break;
        }

        if (!strcmp(progDesc.name, m_cachedProg.name))
            break;

        ++id;
    }

    EEPROM.put(SETTINGS_SIZE + sizeof(ProgDesc) * id, m_cachedProg);

    if (id != m_progNum)
        return;

    ++m_progNum;
    ProgDesc lastProgDesc;
    lastProgDesc.name[0] = 0;
    EEPROM.put(SETTINGS_SIZE + sizeof(ProgDesc) * m_progNum, lastProgDesc);
}

void Memory::deleteProg() {
    uint8_t id = 0;
    while (true) {
        ProgDesc progDesc;
        EEPROM.get(SETTINGS_SIZE + sizeof(progDesc) * id, progDesc);

        if (progDesc.name[0] == 0)
            MyAssert(false);

        if (!strcmp(progDesc.name, m_cachedProg.name))
            break;

        ++id;
    }

    while (true) {
        ProgDesc progDesc;
        EEPROM.get(SETTINGS_SIZE + sizeof(progDesc) * (id + 1), progDesc);
        EEPROM.put(SETTINGS_SIZE + sizeof(progDesc) * id, progDesc);

        if (progDesc.name[0] == 0)
            break;

        ++id;
    }
    --m_progNum;
}

uint8_t Memory::getProgNum() const {
    return m_progNum;
}

void Memory::getProg(uint8_t i, ProgDesc& progDesc) const {
    EEPROM.get(SETTINGS_SIZE + sizeof(ProgDesc) * i, progDesc);
}

void Memory::setProg(uint8_t id) {
    EEPROM.get(SETTINGS_SIZE + sizeof(ProgDesc) * id, m_cachedProg);
}

uint8_t Memory::progId() {
    uint8_t id = 0;
    while (true) {
        ProgDesc progDesc;
        EEPROM.get(SETTINGS_SIZE + sizeof(progDesc) * id, progDesc);

        if (progDesc.name[0] == 0)
            MyAssert(false);

        if (!strcmp(progDesc.name, m_cachedProg.name))
            return id;

        ++id;
    }
}

void Memory::dump() const {
    uint8_t progId = 0;
    Serial.println("[");
    while (true) {
        ProgDesc progDesc;
        EEPROM.get(SETTINGS_SIZE + sizeof(ProgDesc) * progId, progDesc);
        if (progDesc.name[0] == 0)
            break;

        if (progId)
            Serial.println(",");

        JsonDocument doc;

        doc["name"] = progDesc.name;

        JsonArray steps = doc["steps"].to<JsonArray>();
        doc["steps"] = steps;

        for (uint8_t stepId = 0; progDesc.steps[stepId].action != ProgDesc::Action::Finish; ++stepId) {
            JsonObject step = steps.add<JsonObject>();
            switch (progDesc.steps[stepId].action) {
            case ProgDesc::Action::Dev:
                step["action"] = "Dev";
                break;
            case ProgDesc::Action::Bleach:
                step["action"] = "Bleach";
                break;
            case ProgDesc::Action::Fix:
                step["action"] = "Fix";
                break;
            case ProgDesc::Action::Dev2:
                step["action"] = "Dev2";
                break;
            case ProgDesc::Action::ExtraBath:
                step["action"] = "Extra";
                break;
            case ProgDesc::Action::Wash:
                step["action"] = "Wash";
                break;
            case ProgDesc::Action::Wait:
                step["action"] = "Wait";
                break;
            case ProgDesc::Action::Finish:
            case ProgDesc::Action::last_:
                MyAssert(false);
                break;
            }

            if (progDesc.stepSupportTime(stepId)) {
                char formatedTime[6];
                formatTime(progDesc.steps[stepId].time, formatedTime);
                step["time"] = formatedTime;
            }
        }

        serializeJson(doc, Serial);

        ++progId;
    }

    Serial.println("]");
}

void Memory::load() {
    JsonDocument doc;
    DeserializationError error;

    uint8_t progId = 0;
    while (true) {
        String progBuf;
        progBuf.reserve(450);
        while (true) {
            Serial.println("Ready to read chunk");
            String chunk = Serial.readString();
            progBuf += chunk;
            chunk.trim();

            if (chunk.length() == 0)
                break;
        }
        Serial.println("Finish read prog");

        error = deserializeJson(doc, progBuf.c_str());
        if (error)
            goto finish;

        auto object = doc.as<JsonObject>();

        ProgDesc progDesc;
        if (!object.containsKey("name")) {
            Serial.println("Process name has to be specified");
            goto finish;
        }

        const char* name = object["name"];
        if (strlen(name) > sizeof(progDesc.name) + 1) {
            Serial.print("Process name too long: ");
            Serial.println(name);
            goto finish;
        }

        strcpy(progDesc.name, name);

        if (!object.containsKey("steps")) {
            Serial.println("Process steps have to be specified");
            goto finish;
        }

        int stepId = 0;
        for (JsonObject stepDesc : object["steps"].as<JsonArray>()) {
            if (!stepDesc.containsKey("action")) {
                Serial.println("Process step has to specify name");
                goto finish;
            }
            const char* action = stepDesc["action"];

            if (!strcmp(action, "Dev"))
                progDesc.steps[stepId].action = ProgDesc::Action::Dev;
            else if (!strcmp(action, "Bleach"))
                progDesc.steps[stepId].action = ProgDesc::Action::Bleach;
            else if (!strcmp(action, "Fix"))
                progDesc.steps[stepId].action = ProgDesc::Action::Fix;
            else if (!strcmp(action, "Dev2"))
                progDesc.steps[stepId].action = ProgDesc::Action::Dev2;
            else if (!strcmp(action, "Extra"))
                progDesc.steps[stepId].action = ProgDesc::Action::ExtraBath;
            else if (!strcmp(action, "Wash"))
                progDesc.steps[stepId].action = ProgDesc::Action::Wash;
            else if (!strcmp(action, "Wait"))
                progDesc.steps[stepId].action = ProgDesc::Action::Wait;
            else {
                Serial.print("Unknow step name");
                Serial.println(action);
                goto finish;
            }

            if (progDesc.stepSupportTime(stepId)) {
                if (!stepDesc.containsKey("time")) {
                    Serial.println("Process step has to specify time");
                    goto finish;
                }

                const char* time = stepDesc["time"];
                progDesc.steps[stepId].time = unformatTime(time);
            }

            ++stepId;
        }

        progDesc.steps[stepId].action = ProgDesc::Action::Finish;

        EEPROM.put(SETTINGS_SIZE + sizeof(ProgDesc) * progId, progDesc);
        ++progId;
    }

finish:
    if (error && error != DeserializationError::EmptyInput) {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
    }
    if (progId == 0)
        return;

    m_progNum = progId;

    ProgDesc lastProgDesc;
    lastProgDesc.name[0] = 0;
    EEPROM.put(SETTINGS_SIZE + sizeof(ProgDesc) * progId, lastProgDesc);
    Serial.print(progId);
    Serial.println(" program was loaded");
}

Settings Memory::getSettings() {
    Settings settings;
    EEPROM.get(0, settings);

    bool isCorrupted = false;
    if (settings.songId >= SongId::last_)
        isCorrupted = true;

    if (!ProcessSettings::getVolumeName(settings.lastProcessSettings.volume))
        isCorrupted = true;
    if (!ProcessSettings::getAgitationName(settings.lastProcessSettings.agitation))
        isCorrupted = true;

    if (isCorrupted) {
        settings.songId = SongId::Simpsons;
        settings.lastProcessSettings.agitation = ProcessSettings::Agitation::Jobo;
        settings.lastProcessSettings.volume = ProcessSettings::Volume::ml300;
    }

    return settings;
}

void Memory::saveSettings(Settings settings) {
    EEPROM.put(0, settings);
}
