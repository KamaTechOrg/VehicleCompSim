#pragma once
namespace globalConstants {
    enum bufferInfo {
        Time = 0,
        SourceId,
        DestinationId,
        Length
    };

    enum ConnectionState {
        Online,
        Connecting,
        Offline
    };
}
