FROM gcc:12.2.0

# Install cpputest
WORKDIR /opt/cpputest

ARG GIT_USERNAME
ARG GIT_TOKEN
RUN git clone --depth 1 --branch v4.0 https://${GIT_USERNAME}:${GIT_TOKEN}@gitlab.com/dematic/pd-scs/external/cpputest.git .

RUN autoreconf . -i && \
    ./configure && \
    make tdd && \
    make install 

ENV CPPUTEST_HOME=/opt/cpputest

WORKDIR /home/src