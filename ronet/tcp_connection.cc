/* $Id$ */
#include "stdafx.h"

#include "ronet/tcp_connection.h"

#ifdef WIN32
#	define sprintErr(x) sprintf(x, "TCPConnection::RecvData(): Error: %d", WSAGetLastError())
#	define fprintErr(x) fprintf(x, "TCPConnection::RecvData(): Error: %d", WSAGetLastError())
#else
#	define sprintErr(x) sprintf(x, "TCPConnection::RecvData(): Error: %s", strerror(errno))
#	define fprintErr(x) fprintf(x, "TCPConnection::RecvData(): Error: %s", strerror(errno))
#endif

#ifndef MSG_DONTWAIT // This is for windows.
#	define MSG_DONTWAIT 0
#endif

#define RECBUFSIZE (1024*5)

ronet::TcpConnection::TcpConnection() {
	m_state = Ready;
	m_socket = -1;
	m_direction = Outgoing;
	m_protocol = TCP;
}

ronet::TcpConnection::~TcpConnection() {
}

bool ronet::TcpConnection::isConnected() const {
	return(m_state == Connected);
}


bool ronet::TcpConnection::Connect(const char* hostname, const unsigned int port) {
	if (m_socket != -1)
		return(false);
	unsigned long ip = inet_addr(hostname);
	return(Connect(ip, port));
}

bool ronet::TcpConnection::Connect(const unsigned long ip, const unsigned int port) {
	if (m_socket != -1)
		return(false);

	m_direction = Outgoing;

    struct sockaddr_in server_sin;
    m_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_socket < 0) {
    	m_state = Ready;
    	return(false);
    }

	server_sin.sin_family = AF_INET;
	server_sin.sin_addr.s_addr = ip;
	server_sin.sin_port = htons(port);

	int ret;
#ifdef WIN32
	ret = connect(m_socket, (PSOCKADDR) &server_sin, sizeof(server_sin));
#else
	ret = connect(m_socket, (struct sockaddr *) &server_sin, sizeof(server_sin));
#endif

	if (ret) {
		// Something went wrong.
#ifdef WIN32
		closesocket(m_socket);
#else
		close(m_socket);
#endif
		m_state = Ready;
		m_socket = 0;
		return(false);
	}

	// set nonblocking socket
#ifdef WIN32
	unsigned long nonblocking = 1;
	ioctlsocket(m_socket, FIONBIO, &nonblocking);
#else
	fcntl(m_socket, F_SETFL, O_NONBLOCK);
#endif

	m_ip = ip;
	m_port = port;
	m_state = Connected;

	return(true);
}

bool ronet::TcpConnection::Process() {
	if (!isConnected())
		return(false);
	bool ret;

	ret = SendData();
	if (ret == false)
		return(false);
	ret = RecvData();
	return(ret);
}

bool ronet::TcpConnection::SendData() {
	if (m_socket == -1) {
		std::cerr << "Trying to write data on a not connected socket!" << std::endl;
		return(false);
	}

	if (bufOutput.dataSize() == 0) {
		// Nothing to send.
		return(true);
	}

	int datawritten;
#ifdef WIN32
	datawritten = send(m_socket, (const char*) bufOutput.getBuffer(), bufOutput.dataSize(), 0);
#else
	datawritten = send(m_socket, bufOutput.getBuffer(), bufOutput.dataSize(), MSG_NOSIGNAL);
	if(errno==EPIPE) datawritten = -1; // TODO: Check this in Linux
#endif	

	if (datawritten < 0) {
		std::cerr << "Error writing data to socket" << std::endl;
		return (0);
	}
	if (datawritten != 0) {
		bufOutput.ignore(datawritten);
	}

	return(true);
}

bool ronet::TcpConnection::RecvData() {
	if (m_socket == -1) {
		std::cerr << "Trying to read data on a not connected socket!" << std::endl;
		return(false);
	}
	int bufsize;
	unsigned char recbuf[RECBUFSIZE];

	bufsize = recv(m_socket, (char*)recbuf, RECBUFSIZE, MSG_DONTWAIT);
	if (bufsize > 0) {
		std::cout << "[DEBUG] received "<< bufsize <<" bytes of data" << std::endl;
		bufInput.write(recbuf, bufsize);
	}
	else if (bufsize == 0) {
		return(false);  
	}
	else if 
#ifdef WIN32
	(WSAGetLastError() != WSAEWOULDBLOCK)
#else
	(errno != EAGAIN)
#endif
	{
		printf("WSAGetLastError() != WSAEWOULDBLOCK\n");
		return false;
	}
	
	return (true);
}


bool ronet::TcpConnection::Close() {
	if (m_socket == -1)
		return(false);

	m_state = Disconnected;

#ifdef WIN32
	closesocket(m_socket);
#else
	close(m_socket);
#endif

	m_socket = -1;
	return(true);
}

/*

bool ronet::TcpConnection::Process() {
	char errbuf[1024];
	switch(m_state.get()) {
		case Ready:
			// we shouldn't be looping anything
			m_loop = false;
			break;
		case Connecting:
			// TODO: TCPConnection::Process() -- Connecting
			break;		
		case Connected:
			if (!RecvData(errbuf)) {
				_log(NET__ERROR, "RecvData(): %s", errbuf);
				recvbuf.clear();
				sendbuf.clear();
				Disconnect();
			}
			break;
		case Disconnecting:
		case Closing:
			if (sendbuf.size() == 0) {
				m_state = Disconnected;
#ifdef WIN32
				closesocket(m_socket);
#else
				close(m_socket);
#endif
			}
			break;
		case Disconnected:
			m_loop = false;
			break;
		case Error:
			// Socket error
			// TODO: TCPConnection::Process() -- Error
			fprintf(stderr, "[TODO] TCPConnection::Process() -- Error");
			m_loop = false;
			return(false);
			break;
		default:
			// This should never happen
			break;
	}
	
	if (m_state != Disconnected) {
		unsigned int datasize;
		if ((datasize = SendBufData())) {
			printf("[DEBUG] Sent %u bytes of data\n", datasize);
		}
	}
	if (recvbufm.trylock()) {
		if (recvbuf.size() > 0)
			ProcessReceivedData();
		recvbufm.unlock();
	}

	return(true);
}

bool ronet::TcpConnection::RecvData(char* errbuf) {
#define RECBUFSIZE (1024*5)
	if (errbuf)
		errbuf[0] = 0;

	if (!IsConnected()) {
		if (errbuf)
			sprintf(errbuf, "Trying to read data on a not connected socket!");
		printf("[DEBUG] Trying to read data on a not connected socket!\n");
		return(false);
	}
	int bufsize;
	unsigned char recbuf[RECBUFSIZE];

#ifndef MSG_DONTWAIT // This is for windows.
#	define MSG_DONTWAIT 0
#endif

	bufsize = recv(m_socket, (char*)recbuf, RECBUFSIZE, MSG_DONTWAIT);
	if (bufsize > 0) {
		printf("[DEBUG] received %d bytes of data\n", bufsize);
		_log(NET__TRACE, "<<");
		_hexlog(NET__TRACE, recbuf, bufsize);
		recvbufm.lock();
		recvbuf.write(recbuf, bufsize);
		recvbufm.unlock();
	}
	else if (bufsize == 0) {
		if (errbuf)
			sprintf(errbuf, "TCPConnection::RecvData(): Connection closed");

		return(false);  
	}
#ifdef WIN32
	else if (WSAGetLastError() != WSAEWOULDBLOCK) {
#else
	else if (errno != EAGAIN) {
#endif
		printf("WSAGetLastError() != WSAEWOULDBLOCK\n");
		if (errbuf)
			sprintErr(errbuf);

		return false;
	}
	
	return (true);
}

*/
