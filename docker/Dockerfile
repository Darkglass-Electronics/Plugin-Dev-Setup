FROM debian:13
LABEL maintainer="Filipe Coelho <falktx@darkglass.com>"

# optional argument for debug builds, e.g. --build-arg debug=true
ARG debug=false

# optional argument for specifying build target, possible choices:
# - full (builds everything, toolchain and all libs)
# - minimal (builds minimal libs only, includes host-cmake, fftw and liblo)
# - kernel (builds libs needed for the Linux kernel)
# - toolchain (builds toolchain only, no extra libs or host tools)
ARG target=full

# allow to build as root
ENV CT_ALLOW_BUILD_AS_ROOT_SURE=y
ENV FORCE_UNSAFE_CONFIGURE=1

# debian settings
ENV DEBIAN_FRONTEND=noninteractive

# enable arm64
RUN dpkg --add-architecture arm64

# update and upgrade system
RUN apt-get update -qq && apt-get upgrade -qqy && apt-get clean

# install packages for buildroot
RUN apt-get install -qqy qemu-user-static && \
    apt-get install -qqy libc6:arm64 libstdc++6:arm64 zlib1g:arm64 && \
    apt-get install -qqy locales acl bash bash-completion bc curl cvs git mercurial rsync subversion sudo wget dosfstools && \
    apt-get install -qqy bison bundler bzip2 cpio flex gawk gperf gzip help2man nano perl patch tar texinfo unzip && \
    apt-get install -qqy python-is-python3 python3-dev python3-setuptools && \
    apt-get install -qqy automake binutils build-essential device-tree-compiler libtool-bin ncurses-dev jq && \
    apt-get install -qqy libfreetype6-dev libx11-dev libxcursor-dev libxinerama-dev libxrandr-dev pkg-config && \
    apt-get clean

# install packages for plugin validation
RUN apt-get install -qqy lv2-dev sordi && \
    apt-get clean

# misc stuff
RUN locale-gen en_US.UTF-8
RUN echo "source /etc/bash_completion" >> /root/.bashrc

# rust support
RUN curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | bash -s -- -y --no-modify-path --profile minimal
RUN ~/.cargo/bin/rustup install nightly
RUN ~/.cargo/bin/rustup target add aarch64-unknown-linux-gnu
RUN ~/.cargo/bin/rustup target add aarch64-unknown-linux-gnu --toolchain nightly

# checkout mod-plugin-builder
WORKDIR /root
RUN git clone https://github.com/moddevices/mod-plugin-builder --depth=1

# bootstrap mod-plugin-builder target
RUN if [[ "${debug}" == "true" ]]; then export debug_suffix="-debug"; fi
RUN sed -i 's/CT_LOG_PROGRESS_BAR=y/CT_LOG_PROGRESS_BAR=n/' mod-plugin-builder/toolchain/generic-aarch64.config
RUN ./mod-plugin-builder/bootstrap.sh darkglass-anagram${debug_suffix} ${target} && \
    ./mod-plugin-builder/.clean-install.sh darkglass-anagram${debug_suffix} && \
    rm -f ~/mod-workdir/*/..config.tmp && \
    rm -f ~/mod-workdir/*/.br2* && \
    rm -f ~/mod-workdir/*/.config && \
    rm -f ~/mod-workdir/*/Makefile && \
    rm -f ~/mod-workdir/*/staging && \
    rm -rf ~/mod-workdir/*/build && \
    rm -rf ~/mod-workdir/*/images && \
    rm -rf ~/mod-workdir/*/plugins && \
    rm -rf ~/mod-workdir/download

# setup environment for building
RUN echo "source /root/mod-plugin-builder/local.env darkglass-anagram${debug_suffix}" >> /root/.bashrc

# CMD
CMD ["bash"]
