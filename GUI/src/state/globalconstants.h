#pragma once

#include <QString>

namespace globalConstants {

    enum BufferInfo {
        Time = 0,
        SourceId,
        DestinationId,
        Length,
        Buffer
    };

    enum ConnectionState {
        Online,
        Connecting,
        Offline
    };

    enum Icons {
        Start,
        Stop,
        CloudOff,
        Cloud,
        Delete,
        EyeOff,
        Eye,
        Pause,
        Publish,
        Record,
        Replay,
        ToggleSwitchMiddle,
        ToggleSwitchOff,
        ToggleSwitch,
        ArrowRight,
        ArrowLeft,
        AddProject,
    };

    constexpr const char* getIconPath(Icons icon) {
        switch (icon) {
            case Icons::Start: return "resources/icons/start.svg";
            case Icons::Stop: return "resources/icons/stop.svg";
            case Icons::CloudOff: return "resources/icons/cloud-off.svg";
            case Icons::Cloud: return "resources/icons/cloud.svg";
            case Icons::Delete: return "resources/icons/delete.svg";
            case Icons::EyeOff: return "resources/icons/eye-off.svg";
            case Icons::Eye: return "resources/icons/eye.svg";
            case Icons::Pause: return "resources/icons/pause.svg";
            case Icons::Publish: return "resources/icons/publish.svg";
            case Icons::Record: return "resources/icons/record.svg";
            case Icons::Replay: return "resources/icons/replay.svg";
            case Icons::ToggleSwitchMiddle: return "resources/icons/toggle-switch-middle.svg";
            case Icons::ToggleSwitchOff: return "resources/icons/toggle-switch-off.svg";
            case Icons::ToggleSwitch: return "resources/icons/toggle-switch.svg";
            case Icons::ArrowRight: return "resources/icons/arrow-right.svg";
            case Icons::ArrowLeft: return "resources/icons/arrow-left.svg";
            case Icons::AddProject: return "resources/icons/add-project.svg";
            default: return "";
        }
    }
}