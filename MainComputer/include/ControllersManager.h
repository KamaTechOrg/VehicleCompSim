#pragma once

#include <unordered_map>
#include <string>

class ControllersManager
{
public:
	std::vector<std::string> getControllersIDS() const;
	std::vector<std::string> getControllersOptions(std::string controllerID) const;

private:
	// <"id", options[]>
    inline static const std::unordered_map<std::string, std::vector<std::string>> _controllers = {
        {"Beep Controller", {
            "too close",
            "stop",
            "clear",
            }
        },
        {"Dashboard Controller", {
            "low fuel",
            "open seat belt",
            }
        },
        {"Climate Control", {
            "increase temperature",
            "decrease temperature",
            "turn on AC",
            "turn off AC",
            "set fan speed",
            }
        },
        {"Headlight Controller", {
            "turn on headlights",
            "turn off headlights",
            "set high beams",
            "set low beams",
            }
        },
        {"Engine Controller", {
            "start engine",
            "stop engine",
            "check engine status",
            "increase RPM",
            "decrease RPM",
            }
        },
        {"Navigation Controller", {
            "set destination",
            "cancel route",
            "recalculate route",
            "start navigation",
            "stop navigation",
            }
        },
        {"Door Controller", {
            "lock all doors",
            "unlock all doors",
            "open driver's door",
            "close driver's door",
            "open trunk",
            "close trunk",
            }
        },
        {"Window Controller", {
            "roll down driver's window",
            "roll up driver's window",
            "roll down passenger window",
            "roll up passenger window",
            "lock windows",
            "unlock windows",
            }
        },
        {"Entertainment System", {
            "play music",
            "pause music",
            "next track",
            "previous track",
            "adjust volume",
            "mute",
            }
        },
        {"Cruise Control", {
            "activate cruise control",
            "deactivate cruise control",
            "increase speed",
            "decrease speed",
            "set cruise speed",
            }
        },
    };
};