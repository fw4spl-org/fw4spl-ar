/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2014-2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __ARDATA_CAMERASERIES_HPP__
#define __ARDATA_CAMERASERIES_HPP__

#include "arData/Camera.hpp"
#include "arData/config.hpp"

#include <fwCom/Signal.hpp>
#include <fwCom/Signals.hpp>

#include <fwCore/macros.hpp>

#include <fwData/Object.hpp>
#include <fwData/TransformationMatrix3D.hpp>

#include <fwMedData/Series.hpp>

#include <map>
#include <vector>

fwCampAutoDeclareDataMacro((arData)(CameraSeries), ARDATA_API);

namespace arData
{
/**
 * @brief  This class contains the information about cameras (container of camera and extrinsec matrix).
 */

class ARDATA_CLASS_API CameraSeries : public ::fwMedData::Series
{
public:
    fwCoreClassDefinitionsWithFactoryMacro((CameraSeries)( ::fwData::Object), (()),
                                           ::fwData::factory::New< CameraSeries >);

    fwCampMakeFriendDataMacro((arData)(CameraSeries));

    typedef std::vector< ::arData::Camera::sptr > CameraContainerType;

    /**
     *@brief Constructor
     *@param key Private construction key
     */
    ARDATA_API CameraSeries(::fwData::Object::Key key);

    /// Destructor
    ARDATA_API virtual ~CameraSeries();

    /**
     * @brief Defines shallow copy
     * @throws ::fwData::Exception if an errors occurs during copy
     */
    ARDATA_API void shallowCopy( const ::fwData::Object::csptr& _source ) override;

    /**
     * @brief Defines deep copy
     * @throws ::fwData::Exception if an errors occurs during copy
     */
    ARDATA_API void cachedDeepCopy(const ::fwData::Object::csptr& _source, DeepCopyCacheType& cache) override;

    /**@name Signals API
     * @{
     */
    ARDATA_API static const ::fwCom::Signals::SignalKeyType s_ADDED_CAMERA_SIG;
    typedef ::fwCom::Signal<void (::arData::Camera::sptr)> AddedCameraSignalType;

    ARDATA_API static const ::fwCom::Signals::SignalKeyType s_REMOVED_CAMERA_SIG;
    typedef ::fwCom::Signal<void (::arData::Camera::sptr)> RemovedCameraSignalType;

    ARDATA_API static const ::fwCom::Signals::SignalKeyType s_EXTRINSIC_CALIBRATED_SIG;
    typedef ::fwCom::Signal<void ()> ExtrinsicCalibratedSignalType;
    /** @} */

    typedef std::vector< ::fwData::TransformationMatrix3D::sptr > MatricesContainer;

    /**
     * @brief Adds a camera in the cameraSeries.
     * @throws ::fwCore::Exception if the camera is already present in the `CameraSeries`
     */
    ARDATA_API void addCamera(const ::arData::Camera::sptr& camera);

    /**
     * @brief Returns the camera at the index.
     * @throws ::fwCore::Exception if the index is out of range
     */
    ARDATA_API ::arData::Camera::sptr getCamera(size_t index) const;

    /**
     * @brief Remove the given camera from the series
     * @throws ::fwCore::Exception if the camera is not found in the series
     */
    ARDATA_API void removeCamera(const ::arData::Camera::sptr& camera);

    /// Returns the number of cameras
    size_t getNumberOfCameras() const
    {
        return m_cameras.size();
    }

    /**
     * @brief Set the extrinsic matrix.
     * @param[in] index index of the camera associated to this extrinsic matrix. The matrix correspond to the
     *                  transformation from camera[0] to camera[index].
     * @param[in] matrix the extrinsic matrix
     * @note By default, the first matrix (index=0) is initialized to identity.
     * @throws ::fwCore::Exception if the index is out of range
     */
    ARDATA_API void setExtrinsicMatrix(size_t index, ::fwData::TransformationMatrix3D::sptr matrix);

    /**
     * @brief Gets the extrinsic matrix.
     * @param[in] index index of the camera associated to this extrinsic matrix. The matrix correspond to the
     *                  transformation from camera[0] to camera[index].
     * @return Returns the extrinsic transformation matrix, or null if not defined.
     * @note By default, the first matrix (index=0) is initialized to identity, the other are nullptr.
     * @throws ::fwCore::Exception if the index is out of range
     */
    ARDATA_API ::fwData::TransformationMatrix3D::sptr getExtrinsicMatrix(size_t index) const;

    /**
     * @brief Gets the extrinsic matrix corresponding to the transformation from camera[0] to camera[1].
     * @return Returns the extrinsic transformation matrix, or null if not defined.
     * @deprecated Use getExtrinsicMatrix(1) instead of this method
     */
    ::fwData::TransformationMatrix3D::sptr getExtrinsicMatrix() const
    {
        return this->getExtrinsicMatrix(1);
    }

protected:

    /// Contains camera
    CameraContainerType m_cameras;

    /// Signal when camera is added
    AddedCameraSignalType::sptr m_sigAddedCamera;

    /// Signal when camera is removed
    RemovedCameraSignalType::sptr m_sigRemovedCamera;

    /// Signal when cameras are calibrated (extrinsic matrix is computed)
    ExtrinsicCalibratedSignalType::sptr m_sigExtrinsicCalibrated;

    /// extrinsic matrices
    MatricesContainer m_extrinsicMatrices;

};

} //namespace arData

#endif  // __ARDATA_CAMERASERIES_HPP__
