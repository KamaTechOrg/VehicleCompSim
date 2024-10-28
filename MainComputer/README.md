# Sensor and Main Control Unit Team Documentation

## Overview
This document provides an overview of the Sensor and Main Control Unit Team's responsibilities, project goals, and implementation guidelines. The team is divided into two main roles: developing a GUI for heuristic configuration in the Main component and creating microcontroller simulation code for sensors. These two roles work together to enable flexible, real-time communication between sensors and a central server.

## Team Responsibilities

### 1. GUI for Heuristics Configuration

The GUI provides an interface that allows users to define, set, and configure a series of rules (heuristics) for the Main Control Unit. These settings are saved as a JSON file, which the Main Unit will read at runtime to apply operational rules and processes. The objectives of this component include:

- **Creating a Flexible Rule-Setting Interface:** Enables users to define various rules, ensuring adaptability to different scenarios.
- **Saving Settings as JSON:** Saves user-defined rules and settings in a JSON file, making them easily accessible to the Main Unit.
- **Real-Time Updates:** Implements the ability to update and reload rules in the Main Unit without requiring a restart or significant code changes.
- **Compatibility:** Ensures the GUI integrates seamlessly with the sensor system, offering straightforward interaction for non-technical users.

**Additional Features:**

- Users can define individual or complex conditions.
- Support for **AND** and **OR** rule logic, allowing flexibility in creating conditional logic for different scenarios.
- The interface also supports defining conditions within a specific time frame, allowing rules to be set for specific durations or intervals.
- The interface supports both multiple scenarios and multiple actions, enabling users to set complex rules with various outcomes.
- **Scenario Management:** Users can add new scenarios, rename existing ones, or delete scenarios as needed.
- **Condition and Action Management:** Users can add, modify, or delete individual conditions and actions within each scenario, providing full control over rule customization.

## Getting Started

### Prerequisites

1. **Visual Studio**: Download and install [Visual Studio](https://visualstudio.microsoft.com/).
2. **Qt**: Download and set up the Qt framework to develop and run the GUI[Qt Creator](https://www.qt.io/download-dev).