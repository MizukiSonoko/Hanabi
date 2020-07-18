FROM gcc as build

WORKDIR /build
ADD . /build
RUN ./configure --host=x86_64-unknown-linux-gnu 
RUN make

FROM ubuntu 
RUN apt update
RUN apt install -y libncurses-dev
COPY --from=build /build/src/hanabi /bin

ENV TERM=xterm

CMD ["/bin/hanabi"]
