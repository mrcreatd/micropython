/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2025 mrcreatd <mr.creat@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */


#include "esp_eap_client.h"
 
#include "py/mphal.h"
#include "py/obj.h"
#include "py/runtime.h"

// identity
static mp_obj_t espeapclient_set_identity(mp_obj_t identity_obj) {
    const char *s = mp_obj_str_get_str(identity_obj);
    check_esp_err(esp_eap_client_set_identity((const uint8_t *)s, strlen(s)));
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_1(espeapclient_set_identity_obj, espeapclient_set_identity);

// username
static mp_obj_t espeapclient_set_username(mp_obj_t username_obj) {
    const char *s = mp_obj_str_get_str(username_obj);
    check_esp_err(esp_eap_client_set_username((const uint8_t *)s, strlen(s)));
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_1(espeapclient_set_username_obj, espeapclient_set_username);

// password
static mp_obj_t espeapclient_set_password(mp_obj_t password_obj) {
    const char *s = mp_obj_str_get_str(password_obj);
    check_esp_err(esp_eap_client_set_password((const uint8_t *)s, strlen(s)));
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_1(espeapclient_set_password_obj, espeapclient_set_password);

// new_password
static mp_obj_t espeapclient_set_new_password(mp_obj_t password_obj) {
    const char *s = mp_obj_str_get_str(password_obj);
    check_esp_err(esp_eap_client_set_new_password((const uint8_t *)s, strlen(s)));
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_1(espeapclient_set_new_password_obj, espeapclient_set_new_password);

// ca_cert (bytes)
static mp_obj_t espeapclient_set_ca_cert(mp_obj_t ca_cert_obj) {
    mp_buffer_info_t bufinfo;
    mp_get_buffer_raise(ca_cert_obj, &bufinfo, MP_BUFFER_READ);
    check_esp_err(esp_eap_client_set_ca_cert((const uint8_t *)bufinfo.buf, bufinfo.len));
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_1(espeapclient_set_ca_cert_obj, espeapclient_set_ca_cert);

// cert_key(cert, key, [passwd])
static mp_obj_t espeapclient_set_cert_key(size_t n_args, const mp_obj_t *args) {
    mp_buffer_info_t cert, key, passwd = {0};
    mp_get_buffer_raise(args[0], &cert, MP_BUFFER_READ);
    mp_get_buffer_raise(args[1], &key, MP_BUFFER_READ);
    if (n_args == 3) {
        mp_get_buffer_raise(args[2], &passwd, MP_BUFFER_READ);
    }
    check_esp_err(esp_eap_client_set_certificate_and_key(cert.buf, cert.len, key.buf, key.len,
        passwd.buf, passwd.len));
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(espeapclient_set_cert_key_obj, 2, 3, espeapclient_set_cert_key);

// disable_time_check
static mp_obj_t espeapclient_set_disable_time_check(mp_obj_t disable_obj) {
    bool disable = mp_obj_is_true(disable_obj);
    check_esp_err(esp_eap_client_set_disable_time_check(disable));
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_1(espeapclient_set_disable_time_check_obj, espeapclient_set_disable_time_check);

// use_default_cert_bundle
static mp_obj_t espeapclient_use_default_cert_bundle(mp_obj_t enable_obj) {
    bool enable = mp_obj_is_true(enable_obj);
    check_esp_err(esp_eap_client_use_default_cert_bundle(enable));
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_1(espeapclient_use_default_cert_bundle_obj, espeapclient_use_default_cert_bundle);

// set_suiteb_192bit_certification
static mp_obj_t espeapclient_set_suiteb_192bit_certification(mp_obj_t enable_obj) {
    bool enable = mp_obj_is_true(enable_obj);
    check_esp_err(esp_eap_client_set_suiteb_192bit_certification(enable));
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_1(espeapclient_set_suiteb_192bit_certification_obj, espeapclient_set_suiteb_192bit_certification);

// ttls_phase2_method
static mp_obj_t espeapclient_set_ttls_phase2_method(mp_obj_t method_obj) {
    int method = mp_obj_get_int(method_obj);
    check_esp_err(esp_eap_client_set_ttls_phase2_method(method));
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_1(espeapclient_set_ttls_phase2_method_obj, espeapclient_set_ttls_phase2_method);

// enable
static mp_obj_t espeapclient_enable(void) {
    check_esp_err(esp_wifi_sta_enterprise_enable());
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_0(espeapclient_enable_obj, espeapclient_enable);

static mp_obj_t espeapclient_disable(void) {
    check_esp_err(esp_wifi_sta_enterprise_disable());
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_0(espeapclient_disable_obj, espeapclient_disable);

static const mp_rom_map_elem_t mp_module_espeapclient_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_espeapclient) },
    { MP_ROM_QSTR(MP_QSTR_set_identity), MP_ROM_PTR(&espeapclient_set_identity_obj) },
    { MP_ROM_QSTR(MP_QSTR_set_username), MP_ROM_PTR(&espeapclient_set_username_obj) },
    { MP_ROM_QSTR(MP_QSTR_set_password), MP_ROM_PTR(&espeapclient_set_password_obj) },
    { MP_ROM_QSTR(MP_QSTR_set_new_password), MP_ROM_PTR(&espeapclient_set_new_password_obj) },
    { MP_ROM_QSTR(MP_QSTR_set_ca_cert), MP_ROM_PTR(&espeapclient_set_ca_cert_obj) },
    { MP_ROM_QSTR(MP_QSTR_set_cert_key), MP_ROM_PTR(&espeapclient_set_cert_key_obj) },
    { MP_ROM_QSTR(MP_QSTR_set_disable_time_check), MP_ROM_PTR(&espeapclient_set_disable_time_check_obj) },
    { MP_ROM_QSTR(MP_QSTR_use_default_cert_bundle), MP_ROM_PTR(&espeapclient_use_default_cert_bundle_obj) },
    { MP_ROM_QSTR(MP_QSTR_set_suiteb_192bit_certification), MP_ROM_PTR(&espeapclient_set_suiteb_192bit_certification_obj) },
    { MP_ROM_QSTR(MP_QSTR_set_ttls_phase2_method), MP_ROM_PTR(&espeapclient_set_ttls_phase2_method_obj) },
    { MP_ROM_QSTR(MP_QSTR_enable), MP_ROM_PTR(&espeapclient_enable_obj) },
    { MP_ROM_QSTR(MP_QSTR_disable), MP_ROM_PTR(&espeapclient_disable_obj) },
    { MP_ROM_QSTR(MP_QSTR_enable), MP_ROM_PTR(&espeapclient_enable_obj) },
    { MP_ROM_QSTR(MP_QSTR_disable), MP_ROM_PTR(&espeapclient_disable_obj) },
};
static MP_DEFINE_CONST_DICT(mp_module_espeapclient_globals, mp_module_espeapclient_globals_table);

const mp_obj_module_t mp_module_espeapclient = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t *)&mp_module_espeapclient_globals,
};

#if MICROPY_PY_ESPEAPCLIENT
MP_REGISTER_MODULE(MP_QSTR_espeapclient, mp_module_espeapclient);
#endif
