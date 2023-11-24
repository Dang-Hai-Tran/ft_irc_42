#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "irc.hpp"

class Server;
class Channel;

class	Client
{
	public:
		Client(void);
		~Client(void);

		Client&	operator=(const Client& objet);

		// nickname
		void		m_setNickName(std::string& nn);
		std::string	m_getNickName(void) const;

		// username
		void		m_setUserName(std::string un);
		std::string	m_getUserName(void) const;

		// realname
		void		m_setRealName(std::string rn);
		std::string	m_getRealName(void) const;

		// socket
		void		m_setSocket(int sk);
		int			m_getSocket(void) const;

		// input
		void		m_setInput(std::string input);
		std::string	m_getInput(void) const;

		// command + parameter
		void		m_setCmd(std::string cmd);
		std::string	m_getCmd(void) const;
		void		m_setParameter(std::string parameter);
		std::string	m_getParameter(void) const;

		// id
		void		m_setID(int id);
		int			m_getID(void) const;

		// admin server
		void		m_setAdminServer(bool status);
		bool		m_getAdminServer(void) const;

		// admin channel
		void		m_setAdminChannel(bool status);
		bool		m_getAdminChannel(void) const;

		// status on server
		void		m_setStatusS(bool status);
		bool		m_getStatusS(void) const;

		// status on channel
		void		m_setStatusC(bool status);
		bool		m_getStatusC(void) const;

		// modes
		void		m_setMode(bool mode);
		bool		m_getMode(void) const;

		// connected
		void		m_setConnected(bool status);
		bool		m_isConnected(void) const;
	private:
		int			m_socket;
		int			m_id;
		
		std::string	m_nickName;
		std::string	m_userName;
		std::string	m_realName;
		
		std::string	m_input;
		std::string	m_cmd;
		std::string	m_parameter;
		
		bool		m_connected;
		bool		m_statusS;
		bool		m_statusC;

		bool		m_invisible;
		bool		m_adminServer;
		bool		m_adminChannel;
};

#endif
