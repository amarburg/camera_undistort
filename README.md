camera_undistort
================

A quick-and-dirty program which uses OpenCV to undistort images given a camera
calibration file containing information about the camera's projection matrix
and distortion parameters.  Currently reads the CalibrationProject-style XML
files generated by (hmm ... where did these come from).

If you have OpenCV installed, then <code>make test</code> should build and then
undistort the sample image found in <code>sample/</code>.

Contains a copy of [RapidXML]("http://rapidxml.sourceforge.net/") for XML parsing.

Built and tested against [OpenCV]("http://www.opencv.org/") v2.4.9.

This code is cheap and nasty.  Don't take any programming style tips from me!

Distributed under the MIT license.
