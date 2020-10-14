
FROM ubuntu

RUN apt update

RUN apt -y install \
    # npm \
	gcc \
	make \
    python3 \
    python3-pip \
    git \
    npm \
    libgtkextra-dev libgconf2-dev libnss3 libasound2 libxtst-dev libxss1

# ARG BUILD_ENV=development

WORKDIR /home/wot/app

COPY . .

RUN npm install --save-dev electron

RUN npm install \
    && pip3 install . 
    # && if [ "${BUILD_ENV}" = "production" ]; then node_modules/.bin/lerna exec "npm prune --production"; fi \
    # && npm run link

# EXPOSE 8080

#ENTRYPOINT [ "/bin/sh" ]
CMD [ "npm", "start" ]