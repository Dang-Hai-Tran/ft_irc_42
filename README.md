# ft_irc - Internet Relay Chat Server

## Project Overview

ft_irc is an implementation of an Internet Relay Chat (IRC) server, developed as part of the curriculum at 42 Paris. This project challenges students to create a robust, real-time communication server that adheres to the IRC protocol standards while utilizing C++ programming concepts.

## Key Features

1. **Real-time Communication**: Users can connect to the server and communicate with each other instantaneously.
2. **Channel Management**: Support for creating, joining, and managing multiple chat channels.
3. **User Authentication**: Secure user registration and authentication system.
4. **Private Messaging**: Users can send private messages to each other.
5. **Command Support**: Implementation of various IRC commands, including but not limited to:
   - `/PASS`: Authenticate with the server
   - `/NICK`: Set or change nickname
   - `/USER`: Set username and real name
   - `/JOIN`: Join or create a channel
   - `/PART`: Leave a channel
   - `/PRIVMSG`: Send messages to users or channels
   - `/KICK`: Remove a user from a channel
   - `/INVITE`: Invite a user to a channel
   - `/TOPIC`: Set or view channel topics
   - `/MODE`: Change channel modes
6. **Server Administration**: Special commands for server operators to manage the IRC network.
7. **File Transfer**: Capability to send files between clients.
8. **AI-powered Bot**: Integration with OpenAI for an intelligent chatbot feature.

## Technical Specifications

- **Language**: C++98
- **Networking**: TCP/IP (IPv4 or IPv6)
- **Concurrency**: Non-blocking I/O operations using `poll()` (or equivalent)
- **Build System**: Makefile for easy compilation and management
- **Error Handling**: Robust error checking and graceful error handling
- **Memory Management**: Efficient memory usage and leak prevention

## Installation and Usage

1. Clone the repository:
   ```
   git clone https://github.com/Dang-Hai-Tran/ft_irc_42.git ft_irc
   ```

2. Navigate to the project directory:
   ```
   cd ft_irc
   ```

3. Compile the project:
   ```
   make
   ```

4. Run the server:
   ```
   ./ircserv <port> <password>
   ```
   Replace `<port>` with the desired port number and `<password>` with the server password.

5. Connect to the server using an IRC client (e.g., irssi, WeeChat, or a custom client).

## Project Structure

The project is organized into several key components:

- `src/`: Contains all source files
- `inc/`: Header files defining classes and functions
- `Makefile`: Build configuration
- `README.md`: Project documentation
- `Dockerfile`: Container configuration for easy deployment and testing

## Key Challenges and Learning Outcomes

- **Network Programming**: Deep understanding of socket programming and network protocols.
- **Concurrent Programming**: Managing multiple client connections simultaneously without blocking.
- **Protocol Implementation**: Accurately implementing the IRC protocol specifications.
- **Security Considerations**: Handling user authentication and data privacy.
- **Scalability**: Designing a server capable of handling multiple users and channels efficiently.

## Advanced Features

### File Transfer System
The project includes a file transfer feature, allowing users to send files directly through the IRC server. This is implemented in the `commandTransfer` function, which handles the file reading and transmission process.

### AI-powered Chatbot
An innovative addition to this IRC server is the integration of an AI-powered chatbot. Utilizing OpenAI's API, the bot can engage in intelligent conversations with users, enhancing the interactive experience of the chat server.

### Docker Support
The project includes a Dockerfile for easy containerization, allowing for consistent deployment and testing across different environments. This feature is particularly useful for debugging and ensuring consistent behavior across different systems.

## Testing and Debugging

The project includes comprehensive testing capabilities:

- Unit tests for individual components
- Integration tests for overall server functionality
- Valgrind support for memory leak detection and debugging

## Contribution Guidelines

Contributions to this project are welcome. If you'd like to contribute:

1. Fork the repository
2. Create a new branch for your feature
3. Commit your changes
4. Push to the branch
5. Create a new Pull Request

Please ensure your code adheres to the existing style conventions and includes appropriate tests.

## License

This project is licensed under the MIT License. See the [LICENSE](./LICENSE) file for details.

## Acknowledgments

- 42 Paris for providing the project subject and learning environment
- The IRC protocol developers and community
- OpenAI for the AI integration capabilities

## Contact

For any inquiries or issues, please contact the project maintainer at [datran@student.42.fr](mailto:datran@student.42.fr).

