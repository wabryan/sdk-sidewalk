.. _sidewalk_requirements:

Requirements
############

This page outlines the requirements that you need to meet before you start working with the Amazon Sidewalk environment.

.. note::
   Amazon Sidewalk is available only in the United States of America.
   To an extent, any Sidewalk gateway functionality might be used outside of the U.S., however, it should be used only for the Amazon Sidewalk endpoint development purposes.
   In addition, we recommend that you consult with your local regulatory bodies, and check if the gateway is allowed to operate its radio in your locale, as U.S. license-free band devices, only for development.

Software requirements
*********************

To meet the software requirements, complete the following:

* Create an `Amazon Web Service account`_ to be able to connect AWS cloud applications with your Sidewalk Endpoints.

* Install `AWS CLI`_ with the Sidewalk command support to be able to send data to your Sidewalk Endpoints.

.. note::
   * Python3 and python3-pip are required to run the Sidewalk tools.
   * This solution was tested on Ubuntu Linux 20.04 LTS and 22.04 LTS.

Hardware requirements
*********************

To meet the hardware requirements, ensure you have the following:

* Hardware needed to run a specific sample:

   +---------------------------------------------------------------+-------------------------------------------------------------------+
   | Sample                                                        | Hardware required                                                 |
   +===============================================================+===================================================================+
   | Sensor Monitoring                                             | * `Nordic nRF52840 DK`_ - nRF52840 MCU development kit            |
   |                                                               | * `Semtech SX1262MB2CAS`_ (optional) - sub-GHz radio shield       |
   +---------------------------------------------------------------+-------------------------------------------------------------------+
   | Template sub-GHz                                              | * `Nordic nRF52840 DK`_ - nRF52840 MCU development kit            |
   |                                                               | * External flash MX25R6435F or similar for dual bank firmware     |
   |                                                               |   update (included in the DK)                                     |
   |                                                               | * `Semtech SX1262MB2CAS`_ - sub-GHz radio shield                  |
   +---------------------------------------------------------------+-------------------------------------------------------------------+
   | Template Bluetooth LE                                         | * `Nordic nRF52840 DK`_ - nRF52840 MCU development kit            |
   +-----------------------+---------------------------------------+-------------------------------------------------------------------+
   | Device Under Test (DUT)                                       | * `Nordic nRF52840 DK`_ - nRF52840 MCU development kit            |
   |                                                               | * `Semtech SX1262MB2CAS`_ (optional) - sub-GHz radio shield       |
   +---------------------------------------------------------------+-------------------------------------------------------------------+

* Bluetooth wireless interface on your machine or a USB Bluetooth adapter

   .. note::
      Bluetooth wireless adapter is used during the device registration to connect the machine with the development kit.
      For example, you can use a `USB Bluetooth adapter`_ recommended by Amazon.

* Sidewalk Gateway

   `Echo 4th Gen`_ is used as the Sidewalk Gateway.
   It is configured with the Alexa App on `iOS`_ or `Android`_ and requires an Amazon account for setup.

.. _requirements_memory:

RAM and flash memory requirements
*********************************

RAM and flash memory requirement values differ depending on the programmed sample.

The following table lists memory requirement values for the Sidewalk samples ran on the nRF52840 DK.

Values are provided in kilobytes (KB).

+-------------------------------------------------------------------+---------------+-------------------+---------------------+-------------+-------------+
| Sample                                                            |   MCUboot ROM |   Application ROM |   Sidewalk Settings |   Total ROM |   Total RAM |
+===================================================================+===============+===================+=====================+=============+=============+
| :ref:`Sensor monitoring <sensor_monitoring>` (Bluetooth LE Debug) |             0 |               386 |                  32 |         418 |          78 |
+-------------------------------------------------------------------+---------------+-------------------+---------------------+-------------+-------------+
| :ref:`Sensor monitoring <sensor_monitoring>` (FSK Debug)          |             0 |               522 |                  32 |         554 |          99 |
+-------------------------------------------------------------------+---------------+-------------------+---------------------+-------------+-------------+
| :ref:`Sensor monitoring <sensor_monitoring>` (LoRa Debug)         |             0 |               484 |                  32 |         516 |          98 |
+-------------------------------------------------------------------+---------------+-------------------+---------------------+-------------+-------------+
| :ref:`Template sub-GHz <template_subghz>` (FSK Debug)             |            36 |               534 |                  32 |         602 |         109 |
+-------------------------------------------------------------------+---------------+-------------------+---------------------+-------------+-------------+
| :ref:`Template sub-GHz <template_subghz>` (FSK Release)           |            36 |               470 |                  32 |         538 |         105 |
+-------------------------------------------------------------------+---------------+-------------------+---------------------+-------------+-------------+
| :ref:`Template sub-GHz <template_subghz>` (LoRa Debug)            |            36 |               497 |                  32 |         565 |         108 |
+-------------------------------------------------------------------+---------------+-------------------+---------------------+-------------+-------------+
| :ref:`Template sub-GHz <template_subghz>` (LoRa Release)          |            36 |               426 |                  32 |         494 |         104 |
+-------------------------------------------------------------------+---------------+-------------------+---------------------+-------------+-------------+
| :ref:`Template Bluetooth LE <template_ble>` (Debug)               |            36 |               399 |                  28 |         463 |          91 |
+-------------------------------------------------------------------+---------------+-------------------+---------------------+-------------+-------------+
| :ref:`Template Bluetooth LE <template_ble>` (Release)             |            36 |               332 |                  28 |         396 |          87 |
+-------------------------------------------------------------------+---------------+-------------------+---------------------+-------------+-------------+

.. _Amazon developer account: https://developer.amazon.com/dashboard
.. _Amazon Web Service account: https://console.aws.amazon.com/console/home
.. _AWS CLI: https://docs.aws.amazon.com/cli/latest/userguide/cli-chap-install.html
.. _Nordic nRF52840 DK: https://www.nordicsemi.com/Software-and-Tools/Development-Kits/nRF52840-DK
.. _Semtech SX1262MB2CAS: https://www.semtech.com/products/wireless-rf/lora-transceivers/sx1262mb2cas
.. _USB Bluetooth adapter: https://www.amazon.com/Kinivo-USB-Bluetooth-4-0-Compatible/dp/B007Q45EF4
.. _Echo 4th Gen: https://www.amazon.com/All-New-Echo-4th-Gen/dp/B07XKF5RM3
.. _iOS: https://apps.apple.com/us/app/amazon-alexa/id944011620
.. _Android: https://play.google.com/store/apps/details?id=com.amazon.dee.app
.. _nrf52840 DK: https://developer.nordicsemi.com/nRF_Connect_SDK/doc/2.1.0-rc2/nrf/app_boards.html#board-names
