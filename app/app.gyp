{
  'includes': [
    '../common.gypi'
  ],
  'targets': [
    {
      'target_name': 'app',
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
              'main-msvc.cc'
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
              '-std=c++11'
            ],
            'ldflags': [
            ]
          }] # condition OS=linux
      ] # conditions
    }
  ]
}
