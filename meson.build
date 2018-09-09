project('mongoose', 'c',
    version: '0.6.12'
)

mongooseinc = include_directories('.')
mongooselib = shared_library('mongoose',
    'mongoose.c',
    include_directories: mongooseinc
)

