/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2014-2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#pragma once

#include "echoEdSimu/config.hpp"

#include <arServices/ISimulator.hpp>

#include <fwCom/Signal.hpp>
#include <fwCom/Signals.hpp>

#include <fwData/TransformationMatrix3D.hpp>

namespace echoEdSimu
{

/**
 * @brief   Generate (i,j) indices from the projection of translation of a transformation matrix on a imaginary plane.
 *
 * The scale of the image is hardcoded and match the size of the physical plane used in the demo of echo101 during
 * the Hacking Health Camp 2015.
 * @deprecated This service will be removed in 19.0
 */
class ECHOEDSIMU_CLASS_API SProbePosition : public ::arServices::ISimulator
{

public:

    fwCoreServiceClassDefinitionsMacro( (SProbePosition)(::arServices::ISimulator) );

    ECHOEDSIMU_API static const ::fwCom::Signals::SignalKeyType s_POSITION_MODIFIED_SIG;
    typedef ::fwCom::Signal<void (int, int)> PositionModifiedSignalType;

    /// Constructor.
    ECHOEDSIMU_API SProbePosition() noexcept;

    /// Destructor. Does nothing
    ECHOEDSIMU_API virtual ~SProbePosition() noexcept;

protected:

    /**
     * @brief method description:
     * @code{.xml}
        <service uid="SProbePositionInstance" impl="::echoEdSimu::SProbePosition" type="::arServices::ISimulator" />
       @endcode
     */
    ECHOEDSIMU_API virtual void configuring() override;

    /// Do nothing.
    ECHOEDSIMU_API virtual void starting() override;

    /// Do nothing.
    ECHOEDSIMU_API virtual void stopping() override;

    /// Generate the indices (i,j) corresponding to the translation of the matrix.
    ECHOEDSIMU_API virtual void updating() override;

private:
    /// Key of the matrix in the composite
    std::string m_matrixKey;

    /// Matrix
    ::fwData::TransformationMatrix3D::sptr m_matrix;

    /// Signal position change
    PositionModifiedSignalType::sptr m_sigPositionChanged;
};

} // echoEdSimu
