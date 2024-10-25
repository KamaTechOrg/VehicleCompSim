FROM ubuntu:latest AS builder


RUN apt-get update
RUN apt-get install -y build-essential cmake git  build-essential autoconf libtool pkg-config
WORKDIR /hsm-rpc

ARG GRPS_BRANCH=v1.67.0

RUN git clone --recurse-submodules -b ${GRPS_BRANCH} --depth 1 --shallow-submodules https://github.com/grpc/grpc

ARG MAKE_JOBS_N=4

WORKDIR /hsm-rpc/grpc/cmake/build

RUN cmake \
    -DGRPC_ENABLE_TESTS=OFF \
    -DGRPC_BUILD_TESTS=OFF \
    -DGRPC_INSTALL=ON \
    -DCMAKE_INSTALL_PREFIX=/usr/local \
    ../..

RUN make -j ${MAKE_JOBS_N}
RUN make install

WORKDIR /hsm-rpc

COPY . ./HSM

WORKDIR /hsm-rpc/HSM/build

RUN cmake -DGRPC_ENABLED=ON -DCMAKE_PREFIX_PATH=/usr/local -DCMAKE_BUILD_TYPE=Release ..

RUN make -j $MAKE_JOBS_N


FROM ubuntu:latest AS runner
WORKDIR /hsm-rpc

COPY --from=builder /hsm-rpc/HSM/build/RPC/hsm_grpc_server_main /hsm-rpc/hsm_grpc_server_main

VOLUME [ "/data" ]

WORKDIR /data

ENV HSM_PORT=50051

EXPOSE $HSM_PORT

CMD ["sh", "-c", "/hsm-rpc/hsm_grpc_server_main --port ${HSM_PORT}"]
