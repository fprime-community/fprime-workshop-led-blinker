FROM --platform=$BUILDPLATFORM ubuntu:20.04 AS cross-compiler-downloader

ENV DEBIAN_FRONTEND=noninteractive
RUN apt update && \
	apt install -y --no-install-recommends \
		ca-certificates \
		curl \
		tar \
		xz-utils && \
	apt-get clean && \
	rm -rf /var/lib/apt/lists/*

ENV ARM_TOOLS_VERSION=11.2-2022.02

FROM --platform=$BUILDPLATFORM cross-compiler-downloader AS amd64-to-arm64

RUN mkdir -p /opt/toolchains && \
	curl -Ls "https://developer.arm.com/-/media/Files/downloads/gnu/$ARM_TOOLS_VERSION/binrel/gcc-arm-$ARM_TOOLS_VERSION-x86_64-aarch64-none-linux-gnu.tar.xz" | tar -JC /opt/toolchains --strip-components=1 -x

FROM --platform=$BUILDPLATFORM cross-compiler-downloader AS amd64-to-arm

RUN mkdir -p /opt/toolchains && \
	curl -Ls "https://developer.arm.com/-/media/Files/downloads/gnu/$ARM_TOOLS_VERSION/binrel/gcc-arm-$ARM_TOOLS_VERSION-x86_64-arm-none-linux-gnueabihf.tar.xz" | tar -JC /opt/toolchains --strip-components=1 -x

FROM --platform=$BUILDPLATFORM cross-compiler-downloader AS arm64-to-arm

RUN mkdir -p /opt/toolchains && \
	curl -Ls "https://developer.arm.com/-/media/Files/downloads/gnu/$ARM_TOOLS_VERSION/binrel/gcc-arm-$ARM_TOOLS_VERSION-aarch64-arm-none-linux-gnueabihf.tar.xz" | tar -JC /opt/toolchains --strip-components=1 -x

FROM --platform=$BUILDPLATFORM ubuntu:20.04 AS builder

ENV DEBIAN_FRONTEND=noninteractive
RUN apt update && \
	apt install -y --no-install-recommends \
		build-essential \
		cmake\
		default-jre \
		python \
		python3.8-venv && \
	apt-get clean && \
	rm -rf /var/lib/apt/lists/*

# create the virtual environment and install the requirements
COPY fprime/requirements.txt /
RUN python3 -m venv /opt/venv && \
	. /opt/venv/bin/activate && \
	pip install -r requirements.txt

COPY --from=amd64-to-arm64 /opt/toolchains /opt/toolchains/amd64-to-arm64
COPY --from=amd64-to-arm /opt/toolchains /opt/toolchains/amd64-to-arm
COPY --from=arm64-to-arm /opt/toolchains /opt/toolchains/arm64-to-arm

# build the project
COPY . /workspace
WORKDIR /workspace
ARG BUILDARCH
ARG TARGETARCH
RUN . /opt/venv/bin/activate && \
	if [ -n "$TARGETARCH" ] && [ "$BUILDARCH" != "$TARGETARCH" ]; then \
		export ARM_TOOLS_PATH="/opt/toolchains/$BUILDARCH-to-$TARGETARCH"; \
		if [ "$TARGETARCH" = "arm64" ]; then \
			export FPRIME_BUILD_TARGET=aarch64-linux; \
		elif [ "$TARGETARCH" = "arm" ]; then \
			export FPRIME_BUILD_TARGET=arm-hf-linux; \
		fi; \
	fi && \
	fprime-util generate $FPRIME_BUILD_TARGET && \
	fprime-util build $FPRIME_BUILD_TARGET 

FROM scratch

COPY --from=builder /workspace/build-artifacts/ .
