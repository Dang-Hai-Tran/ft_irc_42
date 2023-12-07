# Use an official C++ runtime as a parent image
FROM gcc:latest

# Set the working directory in the container to /app
WORKDIR /app

# Copy the current directory contents into the container at /app
COPY . /app

# Install any needed packages specified in requirements.txt
RUN apt-get update && \
    apt-get install -y make libcurl4-openssl-dev

# Make port 80 available to the world outside this container
EXPOSE 80

# Run the command to compile your code
RUN make

# Run your program when the container launches
CMD ["./ircserv"]

# docker build -t irc .
# docker run -it --rm -p 5555:80 irc
