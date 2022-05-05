/*
 * Copyright (c) 2022 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-4-Clause
 */

/** @file sid_ble_adapter.c
 *  @brief Bluetooth low energy adapter implementation.
 */

#include <sid_pal_ble_adapter_ifc.h>
#include <sid_ble_service.h>
#include <sid_ble_advert.h>
#include <sid_ble_connection.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/conn.h>
#include <bluetooth/gatt.h>
#include <bluetooth/uuid.h>
#include <settings/settings.h>
#include <logging/log.h>

LOG_MODULE_REGISTER(sid_ble, CONFIG_SIDEWALK_LOG_LEVEL);

static sid_error_t ble_adapter_init(const sid_ble_config_t *cfg);
static sid_error_t ble_adapter_start_service(void);
static sid_error_t ble_adapter_set_adv_data(uint8_t *data, uint8_t length);
static sid_error_t ble_adapter_start_advertisement(void);
static sid_error_t ble_adapter_stop_advertisement(void);
static sid_error_t ble_adapter_send_data(sid_ble_cfg_service_identifier_t id, uint8_t *data, uint16_t length);
static sid_error_t ble_adapter_set_callback(const sid_pal_ble_adapter_callbacks_t *cb);
static sid_error_t ble_adapter_disconnect(void);
static sid_error_t ble_adapter_deinit(void);

static struct sid_pal_ble_adapter_interface ble_ifc = {
	.init = ble_adapter_init,
	.start_service = ble_adapter_start_service,
	.set_adv_data = ble_adapter_set_adv_data,
	.start_adv = ble_adapter_start_advertisement,
	.stop_adv = ble_adapter_stop_advertisement,
	.send = ble_adapter_send_data,
	.set_callback = ble_adapter_set_callback,
	.disconnect = ble_adapter_disconnect,
	.deinit = ble_adapter_deinit,
};

typedef struct {
	const sid_ble_config_t *cfg;
} sid_pal_ble_adapter_ctx_t;

static sid_pal_ble_adapter_ctx_t ctx;


static sid_error_t ble_adapter_init(const sid_ble_config_t *cfg)
{
	LOG_DBG("Enable BT");

	if (!cfg) {
		return SID_ERROR_INVALID_ARGS;
	}

	ctx.cfg = cfg;

	int err_code;
	err_code = bt_enable(NULL);
	if (err_code) {
		LOG_ERR("BT init failed (err %d)", err_code);
		return SID_ERROR_GENERIC;
	}

	if (IS_ENABLED(CONFIG_SETTINGS)) {
		err_code = settings_load();
		if (err_code) {
			LOG_ERR("settings load failed (err %d)", err_code);
			return SID_ERROR_GENERIC;
		}
	}

	sid_ble_conn_init();

	return SID_ERROR_NONE;
}

static sid_error_t ble_adapter_start_service(void)
{
	return SID_ERROR_NONE;
}

static sid_error_t ble_adapter_set_adv_data(uint8_t *data, uint8_t length)
{
	if (!data || 0 == length) {
		return SID_ERROR_INVALID_ARGS;
	}

	int err = sid_ble_advert_update(data, length);

	if (err) {
		LOG_ERR("Advertising failed to update (err %d)", err);
		return SID_ERROR_GENERIC;
	}

	return SID_ERROR_NONE;
}

static sid_error_t ble_adapter_start_advertisement(void)
{
	int err = sid_ble_advert_start();

	if (err) {
		LOG_ERR("Advertising failed to start (err %d)", err);
		return SID_ERROR_GENERIC;
	}

	return SID_ERROR_NONE;
}

static sid_error_t ble_adapter_stop_advertisement(void)
{
	int err = sid_ble_advert_stop();

	if (err) {
		LOG_ERR("Advertising failed to stop (err %d)", err);
		return SID_ERROR_GENERIC;
	}

	return SID_ERROR_NONE;
}

static sid_error_t ble_adapter_send_data(sid_ble_cfg_service_identifier_t id, uint8_t *data, uint16_t length)
{
	const struct bt_gatt_service_static *srv = NULL;
	struct bt_uuid *uuid = NULL;

	switch (id) {
	case AMA_SERVICE:
	{
		uuid = AMA_SID_BT_CHARACTERISTIC_NOTIFY;
		srv = sid_ble_get_ama_service();
		break;
	}
	case VENDOR_SERVICE:
	case LOGGING_SERVICE:
	default:
		return SID_ERROR_NOSUPPORT;
	}

	const sid_ble_conn_params_t *params = sid_ble_conn_params_get();
	return sid_ble_send_data(params->conn, uuid, srv, data, length);
}

static sid_error_t ble_adapter_set_callback(const sid_pal_ble_adapter_callbacks_t *cb)
{
	sid_error_t erc;

	if (!cb) {
		return SID_ERROR_NULL_POINTER;
	}

	if (!cb->conn_callback   ||
	    !cb->mtu_callback    ||
	    !cb->adv_start_callback) {
		return SID_ERROR_INVALID_ARGS;
	}

	erc = sid_ble_set_notification_cb(cb->ind_callback);
	if (SID_ERROR_NONE != erc) {
		return erc;
	}

	erc = sid_ble_set_data_cb(cb->data_callback);
	if (SID_ERROR_NONE != erc) {
		return erc;
	}

	erc = sid_ble_set_notification_changed_cb(cb->notify_callback);
	if (SID_ERROR_NONE != erc) {
		return erc;
	}

	sid_ble_conn_cb_set(cb->conn_callback);

	return SID_ERROR_NONE;
}

static sid_error_t ble_adapter_disconnect(void)
{
	return SID_ERROR_NOSUPPORT;
}

static sid_error_t ble_adapter_deinit(void)
{
	memset(&ctx, 0x00, sizeof(ctx));
	sid_ble_conn_deinit();
	return SID_ERROR_NONE;
}

sid_error_t sid_pal_ble_adapter_create(sid_pal_ble_adapter_interface_t *handle)
{
	if (!handle) {
		return SID_ERROR_INVALID_ARGS;
	}

	*handle = &ble_ifc;

	return SID_ERROR_NONE;
}
