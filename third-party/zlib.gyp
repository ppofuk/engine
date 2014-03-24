{
  'includes': [
    '../common.gypi',
  ],
  'targets': [
  {
    'target_name': 'zlib',
    'type': 'static_library',
    'include_dirs': [
      'zlib/'
    ],
    'msvs_guid': 'F5DC442E-3C67-4954-B35D-C6C21133F526',
    'sources': [
      'zlib/adler32.c',
      'zlib/compress.c',
      'zlib/crc32.c',
      'zlib/crc32.h',
      'zlib/deflate.c',
      'zlib/deflate.h',
      'zlib/gzlib.c',
      'zlib/gzclose.c',
      'zlib/gzread.c',
      'zlib/gzwrite.c',
      'zlib/infback.c',
      'zlib/inffast.c',
      'zlib/inffast.h',
      'zlib/inffixed.h',
      'zlib/inflate.c',
      'zlib/inflate.h',
      'zlib/inftrees.c',
      'zlib/inftrees.h',
#     'zlib/mozzconf.h',
      'zlib/trees.c',
      'zlib/trees.h',
      'zlib/uncompr.c',
      'zlib/zconf.h',
      'zlib/zlib.h',
      'zlib/zutil.c',
      'zlib/zutil.h',
    ],
    'direct_dependent_settings': {
      'include_dirs': [
        'zlib/'
      ],
    },
  },
  ] # targets
}
