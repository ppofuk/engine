{
  'includes': [
    '../common.gypi',
  ],

  'variables' : {
    'component': 'static_library'
  },
  'conditions': [
    ['OS=="linux"', {
      'target_defaults': {
        'cflags': ['-fPIC', '-g', '-O3',],
      },
    },],
    ['OS=="win"', {
      'target_defaults': {
        'msvs_settings': {
          'VCLinkerTool': {'GenerateDebugInformation': 'true',},
          'VCCLCompilerTool': {'DebugInformationFormat': '3',},
        },
      },
    },],
  ],
  'targets': [
  {
    'target_name': 'libpng',
    'type': 'static_library',
    'dependencies': [
      'zlib.gyp:zlib',
    ],
    'include_dirs': [
      'libpng/',
      'zlib/',
    ],
    'defines': [
      'CHROME_PNG_WRITE_SUPPORT', # used in pngusr.h
      'PNG_USER_CONFIG',
    ],
    'msvs_guid': 'D32B393F-4D78-4B8D-860B-A7BAB146A4B8',
    'sources': [
      'libpng/png.c',
      'libpng/png.h',
      'libpng/pngconf.h',
      'libpng/pngerror.c',
      # 'libpng/pnggccrd.c',
      'libpng/pngget.c',
      'libpng/pngmem.c',
      'libpng/pngpread.c',
      'libpng/pngread.c',
      'libpng/pngrio.c',
      'libpng/pngrtran.c',
      'libpng/pngrutil.c',
      'libpng/pngset.c',
      'libpng/pngtrans.c',
      'libpng/pngusr.h',
      # 'libpng/pngvcrd.c',
      'libpng/pngwio.c',
      'libpng/pngwrite.c',
      'libpng/pngwtran.c',
      'libpng/pngwutil.c',
    ],
    'direct_dependent_settings': {
      'include_dirs': [
        'libpng'
      ],
      'defines': [
        'PNG_USER_CONFIG',
      ],
    },
    'export_dependent_settings': [
      'zlib.gyp:zlib',
    ],
    'conditions': [
      ['OS!="win"', {'product_name': 'png'}],
      ['OS=="win" and component=="shared_library"', {
        'defines': [
          'PNG_BUILD_DLL',
          'PNG_NO_MODULEDEF',
        ],
        'direct_dependent_settings': {
          'defines': [
            'PNG_USE_DLL',
          ],
        },
      }],
    ],
  },
  ]
}
