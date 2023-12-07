# Use an official C++ runtime as a parent image
FROM gcc:latest

# Set the working directory in the container to /app
WORKDIR /app

# Copy the current directory contents into the container at /app
COPY . /app

# Install any needed packages specified in requirements.txt
RUN apt-get update && \
    apt-get install -y make libcurl4-openssl-dev netcat-traditional irssi valgrind

# Make port 5555 available to the world outside this container
EXPOSE 5555

# Run the command to compile your code
RUN make

# Run your program when the container launches
CMD ["valgrind","--show-leak-kinds=all","--leak-check=full","--track-fds=yes","--track-origins=yes","./ircserv","5555","1234"]

# docker build -t irc .
# docker run -it --rm -p 5555:5555 irc
