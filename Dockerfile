FROM ubuntu:latest

# update image
RUN apt-get update && apt-get install -y --no-install-recommends \
    git

# install dependencies
#RUN apk add libstdc++ gcompat git python3 py3-pip py3-virtualenv
#RUN apt install -y git python3 python3-pip python3-venv

# install lychess-bot
RUN git config --global http.sslverify false && \
    git clone https://github.com/lichess-bot-devs/lichess-bot.git /home/lichess-bot

#WORKDIR /home/lichess-bot

# Copy config
#COPY config.yml /home/lichess-bot/config.yml

# Copy engine
#OPY engines/drawcula /home/lichess-bot/engines/drawcula

#RUN python3 -m venv venv
#RUN virtualenv venv -p python3
#RUN source ./venv/bin/activate
#RUN python3 -m pip install -r requirements.txt --break-system-packages

#CMD python3 lichess-bot.py -v