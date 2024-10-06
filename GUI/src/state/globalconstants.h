#pragma once
namespace globalConstants {
    enum bufferInfo {
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
}
