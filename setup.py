from distutils.core import setup, Extension
diskco_module=Extension('diskco',
    sources= ['diskco_python/diskco_python.cpp',
   'diskco/buffer.cpp',
   'diskco/buffer_pool.cpp',
   'diskco/buffer_processor.cpp',
   'diskco/byte_swapper.cpp',
   'diskco/diskco.cpp',
   'diskco/file_reader.cpp',
   'diskco/file_writer.cpp',
   'diskco/options.cpp',
   'diskco/progress.cpp',
   'diskco/utils.cpp',
   'diskco/searcher.cpp'
  ],
)

setup (name = 'Diskco',
  description = 'Fast dd functionality',
  author = 'Richard',
  ext_modules=[diskco_module]

)
