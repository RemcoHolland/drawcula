FROM alpine:latest

# update image
RUN apk update && apk upgrade

# install dependencies
RUN apk add libstdc++ gcompat git py3-pip

# install lychess-bot
RUN git clone https://github.com/ShailChoksi/lichess-bot.git /home/lichess-bot

WORKDIR /home/lichess-bot

# Copy config
COPY config.yml /home/lichess-bot/config.yml

# Copy engine
COPY /engines/drawcula /home/lichess-bot/engines/drawcula
#
#RUN python3 -m pip install -r requirements.txt
#
#CMD python3 lichess-bot.py -v