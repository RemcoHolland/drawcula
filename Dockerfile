FROM ubuntu:25.04

# Set lichess bot token environment variable
ARG LICHESS_BOT_TOKEN
ENV LICHESS_BOT_TOKEN=$LICHESS_BOT_TOKEN

# update image
RUN apt-get update && apt-get upgrade -y && apt-get install -y --no-install-recommends \
    cmake build-essential git python3 python3-venv python3-virtualenv python3-pip

# install lychess-bot
RUN git config --global http.sslverify false && \
    git clone https://github.com/RemcoHolland/drawcula.git /home/drawcula && \
    git clone https://github.com/lichess-bot-devs/lichess-bot.git /home/lichess-bot

# Configure CMake
RUN cmake -B /home/drawcula/build -S /home/drawcula -DCMAKE_BUILD_TYPE=Release && \
    cmake --build /home/drawcula/build --target drawcula --config Release

RUN cp /home/drawcula/build/drawcula /home/lichess-bot/engines/drawcula && \
    cp /home/drawcula/config.yml /home/lichess-bot/config.yml

# set working directory
WORKDIR /home/lichess-bot

# install virtual environment and start the bot
CMD python3 -m venv venv && \
    virtualenv venv -p python3 && \
    . ./venv/bin/activate && \
    python3 -m pip install -r requirements.txt && \
    python3 lichess-bot.py -v