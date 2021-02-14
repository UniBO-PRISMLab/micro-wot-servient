   FROM node:10.21.0

   #RUN rm bdstart.sh
   RUN apt-get update

   # I think you need to install following 
   #RUN apt-get -y install libgtkextra-dev libgconf2-dev libnss3 libasound2 libxtst-dev libxss1
   RUN apt-get -y install git python3 python3-pip git libxtst6 curl libxss1 libgl1-mesa-glx libgl1-mesa-dri arduino python python-pip
   #RUN apt purge -y arduino
   # RUN xhost local:root
   # RUN useradd -ms /bin/bash -d /usr/src/wot user
   RUN usermod -a -G video node
   RUN usermod -a -G dialout node
   #RUN echo -e '#!/bin/bash\npython3' > /usr/bin/python && chmod +x /usr/bin/python
   ENV BINDIR=/usr/local/bin
   # i think you need to run these commands:
   RUN echo $PATH
   RUN curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh

   # RUN ls -la /usr/src/wot
   #RUN tar -xvf /usr/src/arduino.tar.xz -C /usr/src/
   #RUN /usr/src/arduino-1.8.13/install.sh

   COPY --chown=node . /home/node
   WORKDIR /home/node

   RUN chmod -R 777 .

   RUN pip install pyserial
   RUN pip3 install .

   USER node

   RUN arduino-cli config init
   RUN npm install

   USER root

   RUN chown root:root node_modules/electron/dist/chrome-sandbox
   RUN chmod 4755 node_modules/electron/dist/chrome-sandbox
   RUN chmod 777 /dev/ttyUSB0

   # USER user

   ENV LC_ALL=C.UTF-8
   ENV LANG=C.UTF-8

   CMD ["npm", "start"]