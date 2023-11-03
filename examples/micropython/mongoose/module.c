#include "py/runtime.h"

extern void mgmp_init(void);
extern void mgmp_poll(int);

STATIC mp_obj_t mongoose_init(void) {
    mgmp_init();
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mongoose_init_obj, mongoose_init);

STATIC mp_obj_t mongoose_poll(mp_obj_t t_obj) {
    mgmp_poll(mp_obj_get_int(t_obj));
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(mongoose_poll_obj, mongoose_poll);

STATIC const mp_rom_map_elem_t mongoose_module_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_mongoose) },
    { MP_ROM_QSTR(MP_QSTR_init), MP_ROM_PTR(&mongoose_init_obj) },
    { MP_ROM_QSTR(MP_QSTR_poll), MP_ROM_PTR(&mongoose_poll_obj) },
};
STATIC MP_DEFINE_CONST_DICT(mongoose_module_globals, mongoose_module_globals_table);

const mp_obj_module_t mongoose_module = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t *)&mongoose_module_globals,
};

MP_REGISTER_MODULE(MP_QSTR_mongoose, mongoose_module);
