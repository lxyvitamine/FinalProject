
# cmake setup
install_cmake () {

export MY_INSTALL_DIR=$HOME/.local && mkdir -p $MY_INSTALL_DIR && export PATH="$PATH:$MY_INSTALL_DIR/bin" && \
sudo apt install -y cmake && \
wget -q -O cmake-linux.sh https://github.com/Kitware/CMake/releases/download/v3.19.6/cmake-3.19.6-Linux-x86_64.sh && \
sh cmake-linux.sh -- --skip-license --prefix=$MY_INSTALL_DIR && rm cmake-linux.sh && cmake --version

}


# misc grpc dependencies
setup_grpc_deps () {
    sudo apt-get install build-essential autoconf libtool pkg-config
}


# actually installs grpc, along with protobuf
install_grpc () {
    sudo rm -rf grpc && git clone --recurse-submodules -b v1.35.0 https://github.com/grpc/grpc && \
    cd grpc && mkdir -p cmake/build && pushd cmake/build && cmake -DgRPC_INSTALL=ON \
      -DgRPC_BUILD_TESTS=OFF \
      -DCMAKE_INSTALL_PREFIX=$MY_INSTALL_DIR \
      ../.. && make install && make -j && popd

}


install_cmake && setup_grpc_deps && install_grpc

