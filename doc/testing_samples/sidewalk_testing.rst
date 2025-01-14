.. _sidewalk_testing:

Testing Sidewalk
################

With the sample application, you will be able to establish a connection with Sidewalk Gateways and to transfer and receive data over Sidewalk.
Additionally, you will be able to send a message to your Sidewalk Endpoint using AWS CLI and to verify if it was successfully delivered.

.. _sidewalk_testing_starting:

Starting Sidewalk
*****************

To start Sidewalk, do the following:

#. Connect your Nordic device to the PC through USB.
   Set the power switch on the device to **ON**.

#. Flash the sample application with the manufacturing data as described in the Building and running section of the respective sample.

   You should see the following logs:

   .. code-block:: console

       *** Booting Zephyr OS build v3.2.99-ncs2 ***
       ----------------------------------------------------------------
       sidewalk             v1.14.3-1-g1232aabb
       nrf                  v2.3.0
       zephyr               v3.2.99-ncs2
       ----------------------------------------------------------------
       sidewalk_fork_point = af5d608303eb03465f35e369ef22ad6c02564ac6
       build time          = 2023-03-14 15:00:00.000000+00:00
       ----------------------------------------------------------------

      [00:00:00.006,225] <inf> sid_template: Sidewalk example started!

#. Wait for the device to register, or perform registration manually as described in the :ref:`registering_sidewalk` documentation.

   You should see the following logs:

   .. code-block:: console

      [00:00:31.045,471] <inf> sid_thread: Device Is registered, Time Sync Fail, Link status Up

   When Sidewalk registration status changes, **LED 2** turns on.

#. Wait for the device to complete time sync with the Sidewalk network.

   You should see the following logs:

   .. code-block:: console

      [00:00:35.827,789] <inf> sid_thread: status changed: ready
      [00:00:35.827,850] <inf> sid_thread: Device Is registered, Time Sync Success, Link status Up

   When Sidewalk gets Time Sync, **LED 3** turns on.

#. Perform a button action.

   a. For a Bluetooth LE device, you need to send a connection request before sending a message.
      Press **Button 2** to set the request.

      .. code-block:: console

         [00:44:42.347,747] <inf> sid_thread: Set connection request

      Sidewalk automatically disconnects Bluetooth LE after some inactivity period.
      You have to repeat the button action.

   #. For a LoRa and FSK device, no action is needed.

#. Wait for status change.
   The following messages appear in the log:

   .. code-block:: console

      [00:45:31.597,564] <inf> sid_thread: status changed: init
      [00:45:31.597,564] <dbg> sid_thread: on_sidewalk_status_changed: Device Is registered, Time Sync Success, Link status Up

   When Sidewalk Link Status is Up, **LED 4** turns on.

.. _sidewalk_testing_send_message:

Sending message to AWS MQTT
***************************

You can use `AWS IoT MQTT client`_ to view the received and republished messages from the device.
Follow the outlined steps:

#. Enter ``#`` and click :guilabel:`Subscribe to topic`.
   You are now subscribed to the republished device messages.

#. To see the data republished into the subscribed MQTT topic, press **Button 3** on your development kit.

   .. code-block:: console

      # Logs from DK after pressing "Button 3"
      [00:04:57.461,029] <inf> sid_template: Pressed button 3
      [00:04:57.461,120] <inf> sid_thread: sending counter update: 0
      [00:04:57.461,456] <inf> sid_thread: queued data message id:3


      # Logs from MQTT test client
	  {
      "MessageId": "4c5dadb3-2762-40fa-9763-8a432c023eb5",
      "WirelessDeviceId": "5153dd3a-c78f-4e9e-9d8c-3d84fabb8911",
      "PayloadData": "MDA=",
      "WirelessMetadata": {
         "Sidewalk": {
            "CmdExStatus": "COMMAND_EXEC_STATUS_UNSPECIFIED",
            "MessageType": "CUSTOM_COMMAND_ID_NOTIFY",
            "NackExStatus": [],
            "Seq": 2,
            "SidewalkId": "BFFFFFFFFF"
         }
      }

   Payload data is presented in base64 format.
   You can decode it using Python:

   .. code-block:: console

      python -c "import sys,base64;print(base64.b64decode(sys.argv[1].encode('utf-8')).decode('utf-8'))" MDA=
      00

   Data is republished into the subscribed MQTT topic.

   .. figure:: /images/Step7-MQTT-Subscribe.png

.. _sidewalk_testing_receive_message:

Receiving message from AWS MQTT
*******************************

#. To be able to use AWS CLI, ensure you completed steps in the `Installing or updating the latest version of the AWS CLI`_ documentation.

#. Run the following command to send a message to your Sidewalk Endpoint:

   .. code-block:: console

      aws iotwireless send-data-to-wireless-device --id=<wireless-device-id> --transmit-mode 0 --payload-data="<payload-data>" --wireless-metadata "Sidewalk={Seq=<sequence-number>}"


   * ``<wireless-device-id>`` is the Wireless Device ID of your Sidewalk Device.

      You can find it in the :file:`WirelessDevice.json` file, generated with the :file:`Nordic_MFG.hex` file during :ref:`setting_up_sidewalk_product`.

      You can also find your Wireless Device ID in the message sent form the device to AWS, it you have sent it before.

   * ``<payload-data>`` is base64 encoded.

      To prepare a message payload in the base64 format, run:

      .. code-block:: console

         python -c "import sys,base64;print(base64.b64encode(sys.argv[1].encode('utf-8')).decode('utf-8'))" "Hello   Sidewalk!"
         SGVsbG8gICBTaWRld2FsayE=

   * ``<sequence-number>`` is an integer and should be different for each subsequent request.

      .. note::
         Ensure to increase 'Seq' number on every message.
         The device will not receive a message with lower or equal sequence number.

   Once you have populated the command with data, it should look similar to the following:

   .. code-block:: console

      aws iotwireless send-data-to-wireless-device --id=5153dd3a-c78f-4e9e-9d8c-3d84fabb8911 --transmit-mode 0 --payload-data="SGVsbG8gICBTaWRld2FsayE=" --wireless-metadata "Sidewalk={Seq=1}"

   Successfully sent response should look as follows:

   .. code-block:: console

      {
          "MessageId": "eabea2c7-a818-4680-8421-7a5fa322460e"
      }

   In case you run into the following error, ensure your IAM user or role has permissions to send data to your wireless device:

   .. code-block:: console

      {
         "Message": "User: arn:aws:iam::[AWS Account ID]:user/console_user is not authorized to perform:
         iotwireless:SendDataToWirelessDevice on resource: arn:aws:iotwireless:us-east-1:[AWS Account ID]:
         WirelessDevice/[Wireless Device ID]"
      }

   Data will be received in Sidewalk logs:

   .. code-block:: console

       [00:06:56.338,134] <inf> sid_thread: Message data:
                                     48 65 6c 6c 6f 20 20 20  53 69 64 65 77 61 6c 6b |Hello    Sidewalk
                                     21                                               |!

.. _sidewalk_testing_dfu:

Testing Device Firmware Update (DFU)
************************************

#. To enter the DFU mode, long press **Button 4** on your development kit.
   This action restarts the device in the DFU mode, in which only the `Zephyr SMP Server`_ is running and Sidewalk is not operational.
   When the application is in the DFU mode, all LEDs flash every 500 ms to signal that the application is waiting for a new image.

#. To perform a firmware update, follow the Bluetooth testing steps from the `DevZone DFU guide`_.
   When the update starts, the LEDs change the pattern to indicate upload in progress.

#. To exit the DFU mode, reset your device.
   The device will restart in the Sidewalk mode.
   If the update completes successfully, the device will start a new image.
   After using the DFU mode, the first bootup might take up to 60 seconds. 
   During the image swap the application is silent, meaning that LEDs are not blinking and the console does not output any logs. 
   The application, however, is running, so do not reset the device.
   In case the update fails, the old image is started instead.

Testing Power Profiles
**********************

Power profiles are available for sub-GHz radio communication, such as LoRa or FSK.
For more information about Sidewalk Power Profiles, refer to the `Sidewalk Protocol Specification`_.

The following profiles are available in the template application:

+-------+-------------------+----------------------+--------------+-------------+
| Name  | Power consumption | Messages may be lost | LoRa profile | FSK profile |
+=======+===================+======================+==============+=============+
| Light | Lower             | Yes                  | A            | 1           |
+-------+-------------------+----------------------+--------------+-------------+
| Fast  | Higher            | No                   | B            | 2           |
+-------+-------------------+----------------------+--------------+-------------+

To test the power profiles, complete the following steps:

#. Build and flash the template application with the LoRa or FSK link mask.

   .. code-block:: console

       *** Booting Zephyr OS build v3.2.99-ncs2 ***
       ----------------------------------------------------------------
       sidewalk             v1.14.3-1-g1232aabb
       nrf                  v2.3.0
       zephyr               v3.2.99-ncs2
       ----------------------------------------------------------------
       sidewalk_fork_point = af5d608303eb03465f35e369ef22ad6c02564ac6
       build time          = 2023-03-14 15:00:00.000000+00:00
       ----------------------------------------------------------------
       [00:00:00.063,476] <inf> sid_thread: Initializing sidewalk, built-in LoRa link mask

#. Wait a few seconds until device establishes the connection with Gateway.
   You should see the following output:

   .. code-block:: console

       [00:00:15.017,425] <inf> sid_thread: Device Is registered, Time Sync Success, Link status Up
       [00:00:15.017,486] <inf> sid_thread: Link mode cloud, on lora

#. Get the current profile by short pressing **Button 2**.

   .. code-block:: console

       [00:00:35.433,441] <inf> button: button pressed 2 long
       [00:00:35.433,654] <inf> sid_thread: Profile id 0x81
       [00:00:35.433,654] <inf> sid_thread: Profile dl count 0
       [00:00:35.433,685] <inf> sid_thread: Profile dl interval 5000
       [00:00:35.433,685] <inf> sid_thread: Profile wakeup 0

#. Switch between the light and fast profile by long pressing **Button 2**.

   .. code-block:: console

       [00:00:29.375,732] <inf> button: button pressed 2 short
       [00:00:29.375,854] <inf> sid_thread: Profile set fast
       [00:00:29.375,976] <inf> sid_thread: Profile set success.

.. _sidewalk_testing_application_cli:

Application CLI
***************

The Sidewalk application can be build with the CLI support to help with testing and debugging.

Enabling and verifying Sidewalk command-line interface (CLI)
============================================================

#. To enable CLI, add the ``CONFIG_SIDEWALK_CLI=y`` option to one of the following places:

   * Menuconfig
   * Build command
   * :file:`prj.conf` file

#. To verify Sidewalk CLI, open the UART shell of the device on the default speed 115200.
#. Once you see a prompt ``uart:~$``, type the ``sidewalk help`` command to see the avaliable commands.

   Currently there are 6 commands avaliable:

   * ``sidewalk press_button {1,2,3,4}`` - Simulates button press.
   * ``sidewalk set_response_id <id>`` - Set ID of a next send message with type response.
     It can be used for remote development or for test automation.
   * ``sidewalk send <hex payload> <type [0-3]>`` - Sends message to AWS. Type id: 0 - get, 1 - set, 2 - notify, 3 - response.
     The payload has to be a hex string without any prefix and the number of characters has to be even.
   * ``sidewalk report [--oneline] get state of the application`` - Presents a report in JSON format with the internal state of the application.
   * ``sidewalk version [--oneline] print version of sidewalk and its components`` - Presents a report in JSON format with versions of components that build the Sidewalk application.
   * ``sidewalk factory_reset perform factory reset of Sidewalk application`` - Performs factory reset.

See the example report output:

.. code-block:: console

   uart:~sidewalk report
   "SIDEWALK_CLI": {
         "state": "invalid",
         "registered": 1,
         "time_synced": 1,
         "link_up": 0,
         "link_modes": {
                  "ble": 0,
                  "fsk": 0,
                  "lora": 0
         },
         "tx_successfull": 4,
         "tx_failed": 0,
         "rx_successfull": 0
   }

See the example version output:

.. code-block:: console

   uart:~sidewalk version
   "COMPONENTS_VERSION": {
        "sidewalk_fork_point": "ab13e49adea9edd4456fa7d8271c8840949fde70",
        "modules": {
                "sidewalk": "v1.12.1-57-gab13e49-dirty",
                "nrf": "v2.0.0-734-g3904875f6",
                "zephyr": "v3.0.99-ncs1-4913-gf7b0616202"
        }
   }

.. note::
    By default, only core components are printed.
    To show versions of all components, set ``CONFIG_SIDEWALK_GENERATE_VERSION_MINIMAL`` to ``n`` in :file:`prj.conf` file or in the menuconfig.

.. _AWS IoT MQTT client: https://docs.aws.amazon.com/iot/latest/developerguide/view-mqtt-messages.html
.. _Installing or updating the latest version of the AWS CLI: https://docs.aws.amazon.com/cli/latest/userguide/getting-started-install.html
.. _ID users change permissions: https://docs.aws.amazon.com/IAM/latest/UserGuide/id_users_change-permissions.html
.. _DevZone DFU guide: https://devzone.nordicsemi.com/guides/nrf-connect-sdk-guides/b/software/posts/ncs-dfu#ble_testing
.. _Zephyr SMP Server: https://developer.nordicsemi.com/nRF_Connect_SDK/doc/2.3.0/zephyr/services/device_mgmt/ota.html#smp-server
.. _Sidewalk Protocol Specification: https://docs.sidewalk.amazon/specifications/
.. _Sidewalk_Handler CloudWatch log group: https://console.aws.amazon.com/cloudwatch/home?region=us-east-1#logsV2:log-groups/log-group/$252Faws$252Flambda$252FSidewalk_Handler
.. _AWS IoT MQTT client: https://docs.aws.amazon.com/iot/latest/developerguide/view-mqtt-messages.html
.. _CloudShell: https://console.aws.amazon.com/cloudshell
.. _NCS testing applications: https://developer.nordicsemi.com/nRF_Connect_SDK/doc/2.3.0/nrf/gs_testing.html
