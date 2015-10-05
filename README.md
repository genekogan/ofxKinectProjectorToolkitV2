### ofxKinectProjectorToolkitV2

This [openFrameworks](https://www.openframeworks.cc) addon calibrates a projector to a Microsoft KinectV2 in order to align projections with non-static physical spaces, enabling applications like automated projection-mapping onto moving bodies. It is similar to [ofxKinectProjectorToolkit](https://github.com/genekogan/ofxKinectProjectorToolkit) which does the same thing for the first-generation Kinect, and is based on the method described by [Jan Hrdlička](http://blog.3dsense.org/programming/kinect-projector-calibration-human-mapping-2/). See some [example applications here](http://genekogan.com/works/kinect-projector-toolkit.html) of the old library used for interactive installations and performances.

#### Usage notes

**1**: This addon is currently OSX-only because it's built on top of [ofxMultiKinectV2](https://github.com/hanasaan/ofxMultiKinectV2) wrapping libfreenect to Mac. Maybe I will build a Windows version in the future (doubtful). 

**2**: It's basically working but a bit messy, as it has many dependencies in order to get around Microsoft's platform restrictions.

**3**: Personally, I haven't yet found much advantage to using a KinectV2 instead of a KinectV1 for most of the applications that the original ofxKinectProjectorToolkit was intended for. The depth and especially color images are much higher resolution but that advantage is lost if all you are extracting from them is the contours, which are as accurate (or even more so) than with the original toolkit. Skeleton tracking is much better in V2 but is Windows only -- you can send them over OSC from a Windows machine to a Mac/Linux with [Andy McWilliams's addon](https://github.com/microcosm/KinectV2-OSC) but it's not integrated yet into this toolkit.

**4**: The included bodyMapping example is buggy, as the mapped blobs seem to be off horizontally. I'm not sure yet whether it's a bug in that example itself or if the calibration is poor. I suspect the calibration is inaccurate because the registered depth and RGB images are different resolutions, possibly leading to sampling errors during calibration when matching chessboard points to the depth image -- to be investigated later.

#### Dependencies

The following addons are required.

 * [ofxMultiKinectV2](https://github.com/hanasaan/ofxMultiKinectV2)
 * [ofxTurboJpeg](https://github.com/armadillu/ofxTurboJpeg)
 * [ofxFastFboReader](https://github.com/satoruhiga/ofxFastFboReader)
 * [ofxLayerMask](https://github.com/microcosm/ofxLayerMask) 
 * [ofxCv](https://github.com/kylemcdonald/ofxCv)
 * [ofxSecondWindow](https://github.com/genekogan/ofxSecondWindow) (this is deprecated and I will remove it once OF 0.9 is officially released with mutli-window support)

For any project using the addon (or any project using ofxMultiKinectV2 in general), it is necessary to link OpenCL framework and ofxturbojpeg.dylib to the project. Read the instructions [at ofxMultiKinectV2](https://github.com/hanasaan/ofxMultiKinectV2) for how to do this.


#### Calibration

The included calibration example is used to derive the calibration between the projector and Kinect. It is basically the same as it was for the original [ofxKinectProjectorToolkit](https://github.com/genekogan/ofxKinectProjectorToolkit).

Set your display settings to dual-screen so the projector has its own display, and launch the calibration example.

Make note of the screen resolution of the projector's display (e.g. 1280x800) and copy to `PROJECTOR_RESOLUTION_X` and `PROJECTOR_RESOLUTION_Y`.

Follow the instructions in the calibration app to collect a series of point pairs by moving the chessboard to various positions and recording point pairs, and save the calibration file.


#### Mapping

The post-calibration example_bodyMap demonstrates how to apply the calibration. They reproject solid colors onto the objects found and tracked from the kinect using ofxCv's contourFinder, which tracks blobs frame to frame, though persistent identification is not 100% reliable.

Make sure you load the calibration file you saved in the previous step in the line:

	kpt.loadCalibration(PATH_TO_YOUR_CALIBRTION_FILE);

The key function for mapping is `getProjectedPoint(ofVec3f worldPoint)`. This function takes any 3d world point from the Kinect and converts it to a normalized (0->1) pixel point. For example, the pixel point associated with the world point inside the depth image at (x, y) is found:

    ofVec3f worldPoint = kinect.getWorldCoordinateAt(x, y);
    ofVec2f projectedPoint = kpt.getProjectedPoint(worldPoint);

Multiplying that point by the resolution of the projector recovers the exact pixel to use for projection, so you can, for example, draw a projected circle there like so:

    ofCircle(projector.getWidth() * projectedPoint.x, projector.getHeight() * projectedPoint.y, 10);



