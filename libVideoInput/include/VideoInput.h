#ifndef __VIDEOINPUT_H
#define __VIDEOINPUT_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>	// for OPENCV_ASSERT

/** Interface for generic video inputs. You may want to use VideoInputFactory to create one and than use this generic interface.

	You can use captureFrame to capture the next frame from the source.
	Finally, use release to free the used resources.
	Further methods for adjusting camera parameters are provided. They may not be implemented in all derived classes.
*/
class VideoInput
{
	public:
		/** Used to capture a frame into a given OpenCV Mat.
			@param frame	An already initialized Mat.
			@return			Returns false if not successful or not implemented.

			@warning As the output Mat type is always CV_8UC3 BGR, this may involve a time-consuming conversion.
		*/
        virtual bool captureFrame(cv::Mat &frame)  = 0;

		/** Increments given camera parameter by 10
			@deprecated	Use dedicated methods like SetNormalizedGain, SetNormalizedExposure, and SetNormalizedWhiteBalance instead!
			@param parameter	The parameter to increment.
								Typical parameters: CLEYE_GAIN CLEYE_EXPOSURE CLEYE_ZOOM
			@returns			The new value of the parameter
		*/
        virtual int IncrementCameraParameter(int param) = 0;

		/** Decrements given camera parameter by 10
			@deprecated	Use dedicated methods like SetNormalizedGain, SetNormalizedExposure, and SetNormalizedWhiteBalance instead!
			@param parameter	The parameter to increment.
								Typical parameters: VIDEOINPUTPS3EYEPARAMETERS_GAIN VIDEOINPUTPS3EYEPARAMETERS_EXPOSURE
								(Found in VideoInputPs3EyeParameters.h)
			@returns			The new value of the parameter
		*/
        virtual int DecrementCameraParameter(int param) = 0;

		/** If implemented by current video input, sets gain.
			@param	value	Gain value 0-100, -1 means automatic gain.
			@return			0 for success, -1 for not implemented.
		*/
        virtual int SetNormalizedGain(int value) = 0;

		/** If implemented by current video input, sets exposure.
			@param	value	exposure value 0-100, -1 means automatic exposure.
			@return			0 for success, -1 for not implemented.
		*/
        virtual int SetNormalizedExposure(int value) = 0;

		/** If implemented by current video input, sets white balance.
			If any of r, g, and b are -1, automatic white balance is selected.
			@param	r	white balance for red channel, 0-100.
			@param	g	white balance for green channel, 0-100.
			@param	b	white balance for blue channel, 0-100.
			@return		0 for success, -1 for not implemented.
		*/
        virtual int SetNormalizedWhiteBalance(int r, int g, int b) = 0;

        /** If implemented by the current video input, sets the size of
         *  the frame in pixels
         *  @param width    desired width of the frame
         *  @param height   desired height of the frame
         *  @return     returns 0 on success, and -1 when this feature is not implemented.
         */
        virtual int SetFrameSize(unsigned width, unsigned height) = 0;

      virtual ~VideoInput() { }
//		{
//			// TODO: this does not seem to invoke the correct release() function, only the one of this base class...
//			this->release();
//		}
};
		
#endif
