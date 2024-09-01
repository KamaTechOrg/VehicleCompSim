FROM stateoftheartio/qt6:6.7-gcc-aqt AS builder

# Switch to root user for installations
USER root

# Install additional dependencies
RUN apt-get update && apt-get install -y \
    cmake \
    libgl1-mesa-dev \
    python3-pip \
    && rm -rf /var/lib/apt/lists/*

# Install aqt
RUN pip3 install aqtinstall

# Install required Qt6 modules using aqt
RUN aqt install-qt linux desktop 6.7.0 gcc_64 -m qtwebsockets -m qtnetwork -m qtwidgets

# Set up the Qt environment
ENV PATH="/opt/Qt/6.7.0/gcc_64/bin:${PATH}"
ENV CMAKE_PREFIX_PATH="/opt/Qt/6.7.0/gcc_64:${CMAKE_PREFIX_PATH}"

WORKDIR /app

# Copy your source code
COPY . .

# Build the application
RUN mkdir build && cd build && \
    cmake .. && \
    make

# Start a new stage for the runtime image
FROM stateoftheartio/qt6:6.7-gcc-aqt

# Set up the Qt runtime environment
ENV LD_LIBRARY_PATH="/opt/Qt/6.7.0/gcc_64/lib:${LD_LIBRARY_PATH}"

WORKDIR /app

# Copy only the built executable from the builder stage
COPY --from=builder /app/build/ServerApp .

EXPOSE 28770

CMD ["./ServerApp", "28770"]