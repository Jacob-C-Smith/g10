# syntax=docker/dockerfile:1

FROM fedora

RUN dnf -y install git cmake make gcc g++ hostname automake net-tools

WORKDIR /home
COPY ./ /home/
RUN (cd /home/; chmod +x server_fork.sh)
RUN (cd /home/; export CC=gcc;export CXX=g++;cmake .)
CMD (cd /home/; ./server_fork.sh)