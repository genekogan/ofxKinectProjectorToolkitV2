### ofxKinectProjectorToolkitV2

This addon calibrates a projector to a [Microsoft KinectV2]() in order to align projections with non-static physical spaces, enabling applications like automated projection-mapping onto moving bodies. It is the successor to [ofxKinectProjectorToolkit]() which does the same thing for the [first-generation Kinect](), and is based on the method described by [Jan Hrdlička](http://blog.3dsense.org/programming/kinect-projector-calibration-human-mapping-2/). [See here]() for some example applications of the old library used for interactive installations and performances.

#### Usage notes

**Note 1**: This addon is currently OSX-only because it's built on top of []()'s excellent [ofxMultiKinectV2]() wrapping libfreenect to Mac. Maybe I will build a Windows version in the future (doubtful). 

**Note 2**: It's basically working but a bit messy, as it has many dependencies in order to get around Microsoft's platform restrictions.

**Note 3**: Personally, I haven't found much advantage to using a KinectV2 instead of a KinectV1 for most of the applications that the original ofxKinectProjectorToolkit was intended for. The depth and especially color images are much higher resolution but that advantage is lost if all you are extracting from them is the contours, which are as accurate (or even more so) than with the original toolkit. Skeleton tracking is much better in V2 but is Windows only -- you can send them over OSC to a Mac/Linux with [Andy McWilliams's addon]() but it's not integrated yet into this toolkit.

**Note 4**: The included bodyMapping example is buggy, as the mapped blobs seem to be off. I'm not sure yet whether it's a bug in that example itself or if the calibration is poor. I suspect the calibration is inaccurate because the registered depth and RGB images are different resolutions, possibly leading to sampling errors during calibration when matching chessboard points to the depth image -- to be investigated later.

#### Dependencies

The following addons are required.

 * [ofxMultiKinectV2]()
 * [ofxTurboJpeg]()
 * [ofxFastFboReader]()
 * [ofxLayerMask]() 
 * [ofxCv](https://github.com/kylemcdonald/ofxCv)
 * [ofxSecondWindow](https://github.com/genekogan/ofxSecondWindow) (this is deprecated and I will remove it once OF is officially released with mutli-window support)

For any project using the addon, it is necessary to link OpenCL framework and ofxturbojpeg.dylib to the project. Read the instructions [at ofxMultiKinectV2]() for how to do this.


#### Calibration

The included calibration example is used to derive the calibration between the projector and Kinect. It is basically the same as it was for the original [ofxKinectProjectorToolkit]().

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



