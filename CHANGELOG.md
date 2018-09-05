# fw4spl-ar 17.2.0

## Refactor:

### cmake

*Remove racy backward compatibility.*

### registerService

*Replace deprecated methods to register a service.*

Replace the `::OSR::registerService(obj, srv)` by `srv->registerInOut(obj, key)`
and use `::fwServices::add(srv)` helper instead of calling directly `ServiceFactory

## Bug fixes:

### SGrabberProxy

*Include/exclude mode wasn't working as expected.*

- Improve include/exclude filtering. We can include/exclude a specific service or a specific configuration of a service or both.
- Grabbers are now always displayed in same order in selector dialog.
- Frame by Frame mode from `::videoOpenCV::SFrameGrabber`has been excluded from calibration

### glm

*Add missing GLM_ENABLE_EXPERIMENTAL define.*

Unused glm extensions have been removed

## New features:

### calDataGenerator

*Add an utility program to generate stereo pair of chessboard/charucoboard images.*

This can be useful to test calibration algorithms.

### handEyeActivity

*Handling a step value in SMatricesReader and SFrameGrabber.*

Add a step value in readNext()/readPrevious() slots in SMatricesReader and SFrameGrabber when configured on oneShot mode.

This step value can be changed calling a setStep slot, connected with an int SParameter withstep` key.

We also needed to add this setStep slot in the SGrabberProxy and IGrabber in order to call it properly when using a SGrabberProxy instead of a SFrameGrabber directly.




# fw4spl-ar 17.1.0

## Bug fixes:

### ARCalibration

*Remove warnings by using seriesDB key instead of series.*

Fix series keys to seriesDB used in various configurations because it will be removed in 18.0 version of FW4SPL.

## Refactor:

### getObject

*Remove deprecated getObject().*

Replace deprecated `getObject()` by `getInout()` in `::uiCalibration::SIntrinsicEdition`

Depreciate some bundles and services:
- bundles: ioZMQ, uiZMQ, uiNetwork
- services: SProbeMesh and SProbePosition from echoEdSimu

## New features:

### cvIO

*Add new conversion function between cv::Mat and f4s matrices.*

Convert from/to `::cv::Mat`to ::fwData::TransformationMatrix3D
Convert from/to `::cv::Mat`rvec & tvec convention to ::fwData::TransformationMatrix3D
Add new unit tests cases.
Refactor Calibration code to use new helpers.

merge(54-refactor-trasformationmatrix3d-from-to-opencv-mat): into 'dev'

### trackedCameraCalibration

*Merge activities.*

Fuse sense specific activity and rgb activity thanks to SGrabberProxy.

### video

*Import VLC, Orbbec and RealSense grabbers.*

VLC, Orbbec and RealSense grabbers code is now open and imported into fw4pl-ar, as well as the video filtering. The VLC grabber is convenient especially for RTSP streams. It may also be used as a fallback when the QtMultimedia grabber fails... The Orbbec grabber works for Astra camera and the RealSense brings support for cameras based on Intel sensors.

## Documentation:

### eigenTools

*Document helper namespace.*




# fw4spl-ar 17.0.0

## Bug fixes:

### SPoseFrom2d

*Trigger modified signal even if nothing is detected.*

To keep the processing pipeline updated, we need to keep to trigger the modified signal anytime, like in SArucoTracker.

### videoQt/editor

*Properly handle button actions on choose device.*

- Use accept() and reject QtDialog slots instead of our own onValidate() and generic close()
- In SCamera, check the result of exec dialog window to check if it's canceled and don't continue to configure the camera if so.

## New features:

### Calibration

*Add charuco calibration.*

Add ChArUco board calibration in ARCalibration:
* New Bundle with services related to Charuco calibration
* Brand new utility to generate charuco board.
* New activity in ARCalibration
* ARCalibration has been updated to version 0.5
* Both standard calibration and charuco calibration displays now reprojection error when calibration (intrinsic and extrinsic) is performed.
* videoCalibration Bundles were moved from video folder to calibration folder.

### SPoseFrom2D

*Add a points list data containing the corners of the marker model.*

SPoseFrom2D now provides an inout data that can be used to retrieve the
3D geometry of the marker model.

A mistake was also corrected in hand eye calibration, that called `SOpenCVIntrinsic` instead of `SSolvePnP`. In that case, the camera calibration was overwritten by the first service. Now, it just finds the
pose of a chessboard model in the camera only, without calling the camera calibration service (what we really want to).

### MedicalData

*Update fw4spl-ar data to V13AR.*

- This commit adds a new data version V13AR for AR data
- This new V13AR is require to manage new ModelSeries & ImageSeries with Dicom reference (fw4spl!259)

## Refactor:

### ioAtoms

*Find the correct version without an XML parameter.*

Override the current version of fwMDSemanticPatch to use the AR version.
Clean the useless IO configuration.

### Synchronization

*Improve the synchronization for augmented-reality.*

We reworked the way we synchronize the video frames and the extracted data in real-time.

So far, we have made an extensive use of timelines. First the video grabbers store the frames in timelines. Then we process some algorithms on them and we store all the extracted data (markers, transforms, etc...) in timelines as well. At the end, we rely on ``::videoTools::SFrameMatrixSynchronizer``to pick frames, matrices, etc... at the same timestamp and give these synchronized objects to the renderer, i.e. the generic scene.

However this does not work well. First this is very tedious to work with timelines, since we need to create a dedicated C++ class for each kind of data we want to manage. For some big data, like meshes, we never did it because this would consume too much memory. And some services are simply not well coded and work directly on the data instead of timelines, etc... Eventually, the renderer even screws up the synchronization since all updated objects request the rendering to be done.

So we propose a different approach here. First we restrict the usage of timelines to synchronize video grabbers together, for instance when you
use a camera with multiple sensors or simply several cameras. After that point, all algorithm process the data directly. A new data ``::arData::MarkerMap``is introduced to store a list of markers, since this was the only "data" that only existed in a "timeline" version. To synchronize the results of the algorithms, we propose a new service called `SSignalGate`. This service waits for several signals to be triggered before sending itself a signal, which indicates everyone before is done. This service is typically used to inform the renderer that it must send everything to the GPU right now. To achieve this, we introduced a new rendering mode in `::fwRenderVTK::SRender`.

You can try Ex04SimpleARCV which uses the new design, but for now everything is backward compatible. But we strongly encourage to have a look at this
very soon and try to port your application to benefit of this improvement.

### SExportWithSeriesDB

*Remove getObject.*


