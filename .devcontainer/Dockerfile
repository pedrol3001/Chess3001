FROM mcr.microsoft.com/devcontainers/cpp:1-debian-12

# Install additional packages for C++ development
RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
    && apt-get -y install --no-install-recommends \
    build-essential \
    cmake \
    gdb \
    make \
    ninja-build \
    valgrind \
    # Install Ruby and dependencies
    ruby-full \
    ruby-dev \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

# Install Ruby gems for Sinatra API
RUN gem install sinatra sinatra-contrib json webrick

# Set up non-root user
ARG USERNAME=vscode
ARG USER_UID=1000
ARG USER_GID=$USER_UID

# Create the user if not already exists
RUN if [ ! $(getent passwd $USERNAME) ]; then \
    groupadd --gid $USER_GID $USERNAME \
    && useradd --uid $USER_UID --gid $USER_GID -m $USERNAME \
    && apt-get update \
    && apt-get install -y sudo \
    && echo $USERNAME ALL=\(root\) NOPASSWD:ALL > /etc/sudoers.d/$USERNAME \
    && chmod 0440 /etc/sudoers.d/$USERNAME; \
    fi

# Set the default user
USER $USERNAME

# Set the working directory
WORKDIR /workspaces/chess3001

ENV ENGINE_PATH="/workspaces/chess3001/engine/app"
