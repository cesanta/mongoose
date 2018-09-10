project('mongoose', 'c',
    version: '0.6.12'
)

mongoose_inc = include_directories('.')
mongoose_lib = shared_library('mongoose',
    'mongoose.c',
    include_directories: mongoose_inc
)

mongoose_dep = declare_dependency(link_with: mongoose_lib,
    include_directories: mongoose_inc)
