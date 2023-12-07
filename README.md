# IRC Server

This project is an implementation of an IRC (Internet Relay Chat) server written in C++. IRC is a protocol for live interactive Internet text messaging (chat) or synchronous conferencing. It is mainly designed for group communication in discussion forums, called channels, but also allows one-to-one communication via private message.

## Features

- Real-time communication: Users can connect to the server and communicate with each other in real-time.
- Channel management: Users can create, join, and leave channels.
- User management: Users can register and manage their accounts.
- Message sending: Users can send private messages to each other or broadcast messages to a channel.
- Command support: The server supports various IRC commands, such as `/PASS`, `/USER`, `/JOIN`, `/PART`, `/PRIVMSG`, and `/HELP`.

## Installation

To install and run the IRC server, follow these steps:

1. Clone the repository: `git clone https://github.com/Dang-Hai-Tran/ft_irc_42.git ft_irc`
2. Navigate to the project directory: `cd ft_irc`
3. Build the project: `make`
4. Run the server: `./ircserv <port> <password>`

## Usage

Once the server is running, clients can connect to it using an IRC client application. Here are some examples of IRC commands:

- Login server: `/PASS <password>`
- Register new user or specify user if exits: `/USER <user> <mode> * :<realname>`
- Join a channel or create a channel if not exits: `/JOIN <channel> <password*>`
- Leave a channel with given message: `/PART <channel> <message>`
- Send a message: `/PRIVMSG <username or channel> :<message>`
- Guide to use: `/HELP`

## Contributing

Contributions to this project are welcome. If you find any issues or have suggestions for improvements, please open an issue or submit a pull request.

## License

This project is licensed under the MIT License. See the [LICENSE](./LICENSE) file for more information.

## Contact

For any questions or inquiries, please contact the project maintainer at [datran@student.42.fr](mailto:datran@student.42.fr).
