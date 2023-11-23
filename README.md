# IRC Server

This project is an IRC (Internet Relay Chat) server implementation written in C++. It allows users to connect to the server and communicate with each other in real-time.

## Installation

To install and run the IRC server, follow these steps:

1. Clone the repository: `git clone https://github.com/Dang-Hai-Tran/ft_irc_42.git ft_irc`
2. Navigate to the project directory: `cd ft_irc`
3. Build the project: `make`
4. Run the server: `./ircserv <port> <password>`

## Usage

Once the server is running, clients can connect to it using an IRC client application. The server supports various IRC commands, such as joining channels, sending messages, and managing user accounts.

Here are some examples of IRC commands:

-   Login server: `/PASS <password>`
-   Register new user or specify user if exits: `/USER <user> <mode> * :<realname>`
-   Join a channel or create a channel if not exits: `/JOIN <channel> <password*>`
-   Leave a channel with given message: `/PART <channel> <message>`
-   Send a message: `/PRIVMSG <username or channel> :<message>`
-   Guide to use: `/HELP`

For more information on IRC commands and usage, refer to the [RFC 2812 specification](https://www.rfc-editor.org/rfc/rfc2812.html).

## Contributing

Contributions to this project are welcome. If you find any issues or have suggestions for improvements, please open an issue or submit a pull request.

## License

This project is licensed under the MIT License. See the [LICENSE](./LICENSE) file for more information.

## Contact

For any questions or inquiries, please contact the project maintainer at [datran@student.42.fr](mailto:datran@student.42.fr).
