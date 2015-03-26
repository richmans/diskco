from distutils.core import setup, Extension
diskco_module=Extension('diskco',
    sources= ['diskco_python.cpp',
   '../Diskco/buffer.cpp',
   '../Diskco/buffer_processor.cpp',
   '../Diskco/byte_swapper.cpp',
   '../Diskco/diskco.cpp',
   '../Diskco/file_reader.cpp',
   '../Diskco/file_writer.cpp',
   '../Diskco/options.cpp',
   '../Diskco/progress.cpp',
   '../Diskco/utils.cpp'
  ],
)

setup (name = 'Diskco',
  description = 'Fast dd functionality',
  author = 'Richard',
  ext_modules=[diskco_module]

)