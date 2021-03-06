/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#pragma once

#include "ioIGTL/config.hpp"

#include <fwData/Object.hpp>

#include <igtlNetwork/Client.hpp>
#include <igtlNetwork/Server.hpp>

#include <ioNetwork/INetworkSender.hpp>

#include <cstdint>
#include <future>
#include <string>

namespace ioIGTL
{

/**
 * @brief OpenIGTLink server that will send objects to the connected clients
 *
 * @section XML XML Configuration
 * @code{.xml}
 * <service uid="..." type="::ioIGTL::SServerSender" autoConnect="yes" >
 *      <port>...</port>
 *      <in group="objects">
 *           <key uid="..." deviceName="device01" />
 *           <key uid="..." deviceName="device02" />
 *      </in>
 * </service>
 * @endcode
 * @subsection Configuration Configuration:
 * - \b port : defines the port where the objects will be sent
 * @subsection Input Input:
 * - \b objects [::fwData::Object]: specified objects to send.
 * They must have an attribute 'deviceName' to know the device-name used for this specific data.
 **/

class IOIGTL_CLASS_API SServerSender : public ::ioNetwork::INetworkSender
{

public:

    fwCoreServiceClassDefinitionsMacro( (SServerSender)( ::ioNetwork::INetworkSender ) )

    /// Constructor
    IOIGTL_API SServerSender();

    /// Destructor
    IOIGTL_API virtual ~SServerSender();

protected:

    /// Configure port and device name
    IOIGTL_API virtual void configuring() override;

    /// Start the server.
    IOIGTL_API virtual void starting() override;

    /// Stop the server
    IOIGTL_API virtual void stopping() override;

private:

    /**
     * @brief method to send data.
     *
     * @param[in] obj obj to send
     * @param[in] index index of the object in the group
     */
    void sendObject(const ::fwData::Object::csptr& obj, const size_t index) override;

    /// Server instance
    ::igtlNetwork::Server::sptr m_server;

    /// Future used to wait for the server
    std::future<void> m_serverFuture;

    /// port preference key
    std::string m_portConfig;

    /// Vector of device name if the group configuration exists.
    std::vector< std::string > m_deviceNames;

};

} // namespace ioIGTL
