/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2014-2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "igtlNetwork/Client.hpp"

#include "igtlNetwork/Exception.hpp"

#include <fwTools/Stringizer.hpp>

#include <igtlProtocol/MessageFactory.hpp>

namespace igtlNetwork
{

//------------------------------------------------------------------------------

Client::Client()
{
    m_socket = ::igtl::ClientSocket::New();
}

//------------------------------------------------------------------------------

Client::Client (::igtl::ClientSocket::Pointer socket)
{
    m_socket = socket;
    SLM_ASSERT("socket is null", socket.IsNotNull());
    SLM_ASSERT("socket is not connected", socket->GetConnected());
}

//------------------------------------------------------------------------------

Client::~Client()
{
    if (m_socket->GetConnected())
    {
        this->disconnect();
    }
}

//------------------------------------------------------------------------------

bool Client::isConnected() const
{
    return (m_socket->GetConnected() == 1);
}

//------------------------------------------------------------------------------

void Client::throwExceptionIfFailed (const std::string& msg, bool result)
{
    if (result)
    {
        this->disconnect();
        throw Exception(msg);
    }
}

//------------------------------------------------------------------------------

void Client::connect (const std::string& addr, std::uint16_t port)
{
    ::igtl::ClientSocket* clietSocket = dynamic_cast< ::igtl::ClientSocket* >( m_socket.GetPointer() );
    const int result          = clietSocket->ConnectToServer(addr.c_str(), port);
    const std::string portStr = ::boost::lexical_cast<std::string> (port);

    this->throwExceptionIfFailed("Cannot connect to the server at " + addr + " : " + portStr, result == -1);
}

//------------------------------------------------------------------------------

void Client::disconnect()
{
    m_socket->CloseSocket();
}

//------------------------------------------------------------------------------

} // namespace igtlNetwork

