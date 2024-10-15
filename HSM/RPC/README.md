
## Enabling gRPC Support
To add this directory with gRPC support, you need to configure CMake with the -DGRPC_ENABLED option and have gRPC installed. Below are two options to achieve this:

### Option 1 (Recommended):
Follow the instructions in the [gRPC C++ Quickstart Guide](https://grpc.io/docs/languages/cpp/quickstart/) to install gRPC into a directory (e.g., ~/grpc_install_dir).

After installing gRPC, run the following CMake command:

```bash 
cmake -DGRPC_ENABLED=ON -DCMAKE_PREFIX_PATH=$GRPC_INSTALL_DIR ...
```
Replace $GRPC_INSTALL_DIR with the path to your gRPC installation directory.

### Option 2:
Alternatively, you can have CMake automatically fetch gRPC by using the following command:

```bash
cmake -DGRPC_ENABLED=ON -DGRPC_FETCHCONTENT=ON ...
```


