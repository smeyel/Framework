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

	/** Height of the image. */
	// TODO No "height" field in RAR protocol...
	int getHeight() {
		return root["height"].asInt();
	}
	void setHeight(int height) {
		root["height"] = height;
	}
	/** Width of the image. */
	// TODO No "width" field in RAR protocol...
	int getWidth() {
		return root["width"].asInt();
	}
	void setWidth(int width) {
		root["width"] = width;
	}
	/** Type of the Mat, as defined by OpenCV. */
	// TODO No "mattype" field in RAR protocol...
	int getMatType() {
		return root["mattype"].asInt();
	}
	void setMatType(int matType) {
		root["mattype"] = matType;
	}

	MatImageMessage();
	MatImageMessage(Json::Value root);

	virtual ~MatImageMessage();


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
