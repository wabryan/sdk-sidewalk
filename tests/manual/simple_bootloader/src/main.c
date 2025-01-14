/*
 * Copyright (c) 2023 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <zephyr/dfu/mcuboot.h>

#include <dk_buttons_and_leds.h>
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(main, LOG_LEVEL_DBG);

static void app_setup(void)
{
#if defined(CONFIG_BOOTLOADER_MCUBOOT)
	if (!boot_is_img_confirmed()) {
		int ret = boot_write_img_confirmed();

		if (ret) {
			LOG_ERR("Couldn't confirm image: %d", ret);
		} else {
			LOG_INF("Marked image as OK");
		}
	}
#endif

	dk_leds_init();
}

int main(void)
{
	app_setup();

	LOG_INF("Hello world!");
	dk_set_led_on(DK_LED2);

	return 0;
}
