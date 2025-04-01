FROM ubuntu:22.04

# Install necessary packages
RUN apt-get update && apt-get install -y \
    build-essential \
    nasm \
    gcc \
    make \
    libpthread-stubs0-dev \
    python3 \
    python3-dev

# Set working directory
WORKDIR /app

# Keep container running
CMD ["/bin/bash"]
