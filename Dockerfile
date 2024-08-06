FROM stateoftheartio/qt6:6.7-gcc-aqt

# Install additional dependencies
RUN apt-get update && apt-get install -y \
    cmake \
    libgl1-mesa-dev \
    && rm -rf /var/lib/apt/lists/*

# Set the working directory in the container
WORKDIR /app

# Copy the CMakeLists.txt and source files
COPY . .

# Build the project
RUN mkdir build && cd build && \
    cmake .. && \
    make

# Expose the port your server will run on (adjust if needed)
EXPOSE 28770

# Run the server application
CMD ["./build/ServerApp", "28770"]