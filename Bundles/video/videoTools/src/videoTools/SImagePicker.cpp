/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2018.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "videoTools/SImagePicker.hpp"

#include <fwCom/Signal.hxx>
#include <fwCom/Slots.hxx>

#include <fwServices/macros.hpp>

namespace videoTools
{

fwServicesRegisterMacro( ::fwServices::IController, ::videoTools::SImagePicker, ::fwData::PointList);

//-----------------------------------------------------------------------------

const ::fwCom::Slots::SlotKeyType SImagePicker::s_GET_INTERACTION_SLOT = "getInteraction";
const ::fwServices::IService::KeyType SImagePicker::s_POINTLIST_INOUT  = "pointList";

SImagePicker::SImagePicker() noexcept
{
    newSlot(s_GET_INTERACTION_SLOT, &SImagePicker::getInteraction, this);
}

//-----------------------------------------------------------------------------

SImagePicker::~SImagePicker() noexcept
{
}

//-----------------------------------------------------------------------------

void SImagePicker::starting()
{
}

//-----------------------------------------------------------------------------

void SImagePicker::stopping()
{
}

//-----------------------------------------------------------------------------

void SImagePicker::configuring()
{
}

//-----------------------------------------------------------------------------

void SImagePicker::updating()
{
}

//-----------------------------------------------------------------------------

void SImagePicker::getInteraction(::fwDataTools::PickingInfo info)
{
    if (info.m_modifierMask == ::fwDataTools::PickingInfo::CTRL)
    {
        const double x = info.m_worldPos[0];
        const double y = -(info.m_worldPos[1]);
        const double z = info.m_worldPos[2];

        const std::array<double, 3> position = {{x, y, z}};

        if (info.m_eventId == ::fwDataTools::PickingInfo::Event::MOUSE_LEFT_DOWN)
        {
            this->addPoint(position);
        }
        else if (info.m_eventId == ::fwDataTools::PickingInfo::Event::MOUSE_RIGHT_DOWN)
        {
            this->removeLastPoint();
        }

    }
}

//-----------------------------------------------------------------------------

void SImagePicker::addPoint(const std::array<double, 3>& currentPoint )
{
    // set z to 0 as it is an image.
    ::fwData::Point::sptr point         = ::fwData::Point::New(currentPoint[0], currentPoint[1], 0.);
    ::fwData::PointList::sptr pointList = this->getInOut< ::fwData::PointList >(s_POINTLIST_INOUT);

    pointList->getPoints().push_back(point);
    auto sig = pointList->signal< ::fwData::PointList::ModifiedSignalType >(
        ::fwData::PointList::s_MODIFIED_SIG);
    sig->asyncEmit();

    auto sig2 = pointList->signal< ::fwData::PointList::PointAddedSignalType >(
        ::fwData::PointList::s_POINT_ADDED_SIG);
    sig2->asyncEmit(point);
}

//-----------------------------------------------------------------------------

void SImagePicker::removeLastPoint()
{
    ::fwData::PointList::sptr pointList = this->getInOut< ::fwData::PointList >(s_POINTLIST_INOUT);
    ::fwData::Point::sptr point;

    if (!pointList->getPoints().empty())
    {
        point = pointList->getPoints().back();
        pointList->getPoints().pop_back();

        auto sig = pointList->signal< ::fwData::PointList::ModifiedSignalType >(
            ::fwData::PointList::s_MODIFIED_SIG);
        sig->asyncEmit();

        auto sig2 = pointList->signal< ::fwData::PointList::PointRemovedSignalType >(
            ::fwData::PointList::s_POINT_REMOVED_SIG);
        sig2->asyncEmit(point);
    }

}

//-----------------------------------------------------------------------------

} // namespace videoTools
