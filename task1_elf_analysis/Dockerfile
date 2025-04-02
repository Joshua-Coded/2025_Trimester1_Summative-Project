FROM ubuntu:22.04

# Install necessary tools for binary analysis
RUN apt-get update && apt-get install -y \
    binutils \
    gdb \
    strace \
    file \
    build-essential \
    vim \
    less \
    && rm -rf /var/lib/apt/lists/*

# Create a work directory
WORKDIR /analysis

# Create data directory for the binary
RUN mkdir -p /analysis/data

# Add a simple test file for analysis
RUN echo "John" > /analysis/data/entries.txt && \
    echo "Mary" >> /analysis/data/entries.txt && \
    echo "Robert" >> /analysis/data/entries.txt

# Copy the analysis script (will be mounted)
# COPY analyze_elf.sh /analysis/
# RUN chmod +x /analysis/analyze_elf.sh

# Set default command
CMD ["/bin/bash"]
