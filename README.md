@mainpage

SMEyeL - Framework
==================

This is the Framework submodule of the SMEyeL system.
Project website: https://www.aut.bme.hu/Pages/ResearchEn/SMEyeL/Overview

libCommunication
----------------

The communication library covers all communication related functions between the SMEyeL nodes.

To use it, have a look at the class PhoneProxy. This handles connecting to a phone and sending/receiving messages which are derived from JsonMessage.

libLogConfigTime
----------------

This library has four main functions: logging, configuration handling, and execution time measurement.

Logging is performed via a singleton pattern via the LogConfigTime::Logger interface.

Configuration is usually handled by a local class. These use a LogConfigTime::ConfigReader to read from a source of configuration settings, and store the config settings in static variables to allow easy access.

Execution time measurement is handled by LogConfigTime::TimeMeasurement in a high-precision, but still platform independent way (thanks to OpenCV's abstraction). You can assign texts to integer measurement ID-s and then start and finish time measurement for these ID-s independently. Multiple measurements are averaged and exported at the end, together with the assigned texts for better readability.

libVideoInput
-------------

This library is the camera hardware abstraction used in SMEyeL.

All video sources are derived from VideoInput and instantiated by VideoInputFactory as requested.

As everything accepted by OpenCV's abstraction is covered by VideoInputGeneric, using that may be sufficient.

If you use the special drivers of Ps3Eye cameras under windows, you may need VideoInputPs3Eye for it.
