{
  'includes': [
    '../common.gypi'
  ],
  'targets': [
    {
      'target_name': 'shader_test',
      'type': 'executable',
      'dependencies': [
        '../third-party/libpng.gyp:libpng',
        '../library/library.gyp:library',
        '../library/cef/cef.gyp:cef'
      ],
      'include_dirs': [
        'library'
      ],
      'sources': [
        'test-gl-sprite-shader.cc'
      ],
      'conditions': [
          ['OS=="win"', {
            'defines': [
              'OS_WINDOWS'
            ],
            'cflags': [
              '/WX'
            ],
            'sources': [
              'main-msvc.cc',
              'test-shader.cc'
            ],
            'libraries': [
              '-lopengl32.lib',
              '-lglu32.lib',
              '-lkernel32.lib',
              '-luser32.lib',
              '-lgdi32.lib',
              '-lwinspool.lib',
              '-lcomdlg32.lib',
              '-ladvapi32.lib',
              '-lshell32.lib',
              '-lole32.lib',
              '-loleaut32.lib',
              '-luuid.lib'
            ]
          }], # condition OS=win
          ['OS=="linux"', {
            'sources': [
              'main-posix.cc',
              'test-shader.cc'
            ],
            'cflags': [
              '-Wall',
              '-std=c++14'
            ],
            'ldflags': [
              '-Wl,-rpath=\$$ORIGIN'
            ]
          }] # condition OS=linux
      ] # conditions
    }, # app target

    {
      'target_name': 'memory_test',
      'type': 'executable',
      'dependencies': [
        '../third-party/libpng.gyp:libpng',
        '../library/library.gyp:library'
      ],
      'include_dirs': [
        'library'
      ],
      'conditions': [
          ['OS=="win"', {
            'defines': [
              'OS_WINDOWS'
            ],
            'cflags': [
              '/WX'
            ],
            'sources': [
              'memory-msvc.cc'
            ],
            'libraries': [
              '-lopengl32.lib',
              '-lglu32.lib',
              '-lkernel32.lib',
              '-luser32.lib',
              '-lgdi32.lib',
              '-lwinspool.lib',
              '-lcomdlg32.lib',
              '-ladvapi32.lib',
              '-lshell32.lib',
              '-lole32.lib',
              '-loleaut32.lib',
              '-luuid.lib'
            ]
          }], # condition OS=win
          ['OS=="linux"', {
            'sources': [
              'memory-posix.cc'
            ],
            'cflags': [
              '-Wall',
              '-std=c++14'
            ],
            'ldflags': [
            ]
          }] # condition OS=linux
      ] # conditions
    } # memory test

  ]
}
