FROM debian:9
RUN apt-get update && apt-get upgrade -y
RUN mkdir /yoda-bin/
RUN mkdir /yoda-bin/data
ENV PATH="/yoda-bin/bin/:${PATH}"
ADD http://88.99.91.31:8080/yoda-binaries/master/yoda-5.0.0-x86_64-linux-gnu.tar.gz /tmp/
RUN tar -C /yoda-bin/ --strip 1 -xf /tmp/yoda-5.0.0-x86_64-linux-gnu.tar.gz
COPY ./yoda.conf /yoda-bin/
CMD ["yodad","-conf=/yoda-bin/yoda.conf","-datadir=/yoda-bin/data/","-debug","-gen"]
