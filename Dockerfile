FROM stateoftheartio/qt6:6.7-gcc-aqt AS builder

# Switch to root user to create the symbolic link
USER root

# Install additional dependencies and build
RUN mkdir -p /tmp/apt/lists && \
    ln -sf /tmp/apt/lists /var/lib/apt/lists && \
    apt-get update && apt-get install -y \
    cmake \
    libgl1-mesa-dev && \
    rm -rf /tmp/apt/lists


WORKDIR /app

COPY . .

RUN mkdir build && cd build && \
    cmake .. && \
    make

# Start a new stage for the runtime image
FROM stateoftheartio/qt6:6.7-gcc-aqt

WORKDIR /app

# Copy only the built executable from the builder stage
COPY --from=builder /app/build/ServerApp .

EXPOSE 28770

CMD ["./ServerApp", "28770"]