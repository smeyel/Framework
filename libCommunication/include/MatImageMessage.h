#ifndef __MATIMAGEMESSAGE_H
#define __MATIMAGEMESSAGE_H

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>

#include "JsonWithAuxMessage.h"

/** A message containing an image in OpenCV Mat format.
	The Mat is packed and unpacked by the methods Decode() and Encode(),
	which have to be called manually.
*/
class MatImageMessage : public JsonWithAuxMessage
{
private:
	/** Mat object. After receiving, the aux binary data
	is transformed into this Mat by the method Decode(). */
	cv::Mat *mat;
public:
	/** Timestamp of the image. */
	long long timestamp;

	/** Height of the image. */
	int height;
	/** Width of the image. */
	int width;
	/** Type of the Mat, as defined by OpenCV. */
	int mattype;

	MatImageMessage(char *json);
	MatImageMessage();

	bool parse(char *json);
	virtual void writeJson(char *buffer);

	/** After receiving a message, use this to create the Mat object
		and than access it via getMat().
	*/
	void Decode();

	/** Places the content of a Mat into the message.
	*/
	void Encode(cv::Mat *srcMat);

	/** Returns the Mat contained in the message.
		Before accessing it, use the Decode() method to transform
		the aux binary data into a Mat.
	*/
	cv::Mat *getMat()
	{
		// Decode() creates that...
		return mat;
	}

	virtual void log();
};

#endif