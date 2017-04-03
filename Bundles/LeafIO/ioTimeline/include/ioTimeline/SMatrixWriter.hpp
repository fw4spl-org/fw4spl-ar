/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __IOTIMELINE_SMATRIXWRITER_HPP__
#define __IOTIMELINE_SMATRIXWRITER_HPP__

#include "ioTimeline/config.hpp"

#include <arData/MatrixTL.hpp>

#include <io/IWriter.hpp>

namespace ioTimeline
{

/**
 * @brief Thi service allows to save the timeline matrices in a csv file in a folder.
 *
 * @note The method 'updating' allows to save the timeline matrix with the current timestamp. If you want to save all
 * the
 *       matrices when they are pushed in the timeline, you must use the slots 'startRecord' and 'stopRecord'
 *
 * @section Slots Slots
 * - \b startRecord() : start recording
 * - \b stopRecord() : stop recording
 *
 * @section XML XML Configuration
 *
 * @code{.xml}
   <service type="::ioTimeline::SMatrixWriter">
       <in key="data" uid="..." autoConnect="yes" />
   </service>
   @endcode
 * @subsection Input Input
 * - \b data [::arData::MatrixTL]: timeline containing the matrices to save.
 */
class IOTIMELINE_CLASS_API SMatrixWriter : public ::io::IWriter
{

public:

    fwCoreServiceClassDefinitionsMacro( (SMatrixWriter)(::io::IWriter) );

    /// Constructor.
    IOTIMELINE_API SMatrixWriter() throw();

    /// Destructor. Does nothing
    IOTIMELINE_API virtual ~SMatrixWriter() throw();

    /// Defines auto connection for this service (saveFrame()) to the frame timeline (objectPushed)
    IOTIMELINE_API virtual ::fwServices::IService::KeyConnectionsMap getAutoConnections() const;

    /// Display a location dialog allowing to select the video file to save
    IOTIMELINE_API virtual void configureWithIHM();

    /// Return file type (::io::FOLDER)
    IOTIMELINE_API virtual ::io::IOPathType getIOPathType() const;

protected:

    /// Does nothing
    IOTIMELINE_API virtual void configuring()  throw ( ::fwTools::Failed );

    /// Does nothing
    IOTIMELINE_API virtual void starting()  throw ( ::fwTools::Failed );

    /// Does nothing
    IOTIMELINE_API virtual void stopping()  throw ( ::fwTools::Failed );

    /// Does nothing
    IOTIMELINE_API virtual void updating() throw ( ::fwTools::Failed );

private:

    /// SLOT: add the current frame in the video
    void saveMatrix(::fwCore::HiResClock::HiResClockType timestamp);

    /// SLOT: Start recording
    void startRecord();

    /// SLOT: Strop recording
    void stopRecord();

    bool m_isRecording; ///< flag if the service is recording.

    std::ofstream* m_filestream;
};

} // ioTimeline

#endif // __IOTIMELINE_SMATRIXWRITER_HPP__
