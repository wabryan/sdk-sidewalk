#
# Copyright (c) 2022 Nordic Semiconductor ASA
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(sidewalk_test_ble_adapter)
set(SIDEWALK_BASE $ENV{ZEPHYR_BASE}/../sidewalk)

cmock_handle(${ZEPHYR_BASE}/include/zephyr/settings/settings.h)
# cmock_handle(${ZEPHYR_BASE}/include/include/zephyr/bluetooth/hci.h)
cmock_handle(${SIDEWALK_BASE}/subsys/sal/sid_pal/include/sid_ble_advert.h)
cmock_handle(${SIDEWALK_BASE}/subsys/sal/sid_pal/include/sid_ble_connection.h)
cmock_handle(${SIDEWALK_BASE}/subsys/sal/sid_pal/include/sid_ble_adapter_callbacks.h)
cmock_handle(${SIDEWALK_BASE}/subsys/sal/sid_pal/include/sid_ble_service.h)


# add test file
target_sources(app PRIVATE src/main.c)

# generate runner for the test
test_runner_generate(src/main.c)
