   FROM node:10.21.0

   COPY . /usr/scr/wot

   #RUN rm bdstart.sh
   RUN apt-get update

   # I think you need to install following 
   #RUN apt-get -y install libgtkextra-dev libgconf2-dev libnss3 libasound2 libxtst-dev libxss1
   RUN apt-get -y install git python3 python3-pip git arduino curl libxss1

   # i think you need to run these commands:
   RUN curl https://downloads.arduino.cc/arduino-1.8.13-linux64.tar.xz -o /usr/src/arduino.tar.xz
   RUN tar -xvf /usr/src/arduino.tar.xz -C /usr/src/
   RUN /usr/src/arduino-1.8.13/install.sh

   #RUN xhost local:root

   WORKDIR /usr/scr/wot

   RUN pip3 install .
   RUN npm install .

   CMD ["npm", "start"]