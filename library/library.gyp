{
  'includes': [
    '../common.gypi'
  ],

  'targets': [
    {
      'target_name': 'library',
      'type': '<(library)',
      'defines': [
        'LIBRARY_EXPORT'
      ],
      'include_dirs': [
        '.',
      ],
      'dependencies': [
        '../third-party/libpng.gyp:libpng',
        '../third-party/glew.gyp:glew'
      ],
      'sources': [
        'basic-types.h',
        'linked-list.h',
        'logger.cc',
        'logger.h',
        'singleton.h',
        'sprite-properties.cc',
        'sprite-properties.h',
        'sprite.h',
        'sprite.cc',
        'texture-abstract.h',
        'texture-png.h',
        'texture-png.cc',
        'reader-inl.h',
        # OpenGL stuff
        'gl/gl-texture.cc',
        'gl/gl-texture.h',
        'gl/gl-buffer.h',
        'gl/gl-program.h',
        'gl/gl-program.cc',
        'gl/gl-shader.h',
        'gl/gl-shader.cc',
        'gl/gl-texture.h',
        'gl/gl-texture.cc'
      ],
      'direct_dependent_settings': {
        'include_dirs': ['.'],
        'conditions': [
          ['OS=="win"', {
            'defines': [
              'OS_WINDOWS'
            ],
            'cflags': [
              '/Wall',
              '/O2'
            ],
            'ldflags': [
              '"opengl32.lib"',
              '"glu32.lib"',
              # so it builds
              '"kernel32.lib;"',
              '"user32.lib;"',
              '"gdi32.lib;"',
              '"winspool.lib;"',
              '"comdlg32.lib;"',
              '"advapi32.lib;"',
              '"shell32.lib;"',
              '"ole32.lib;"',
              '"oleaut32.lib;"',
              '"uuid.lib;"'
            ]
          }],
          ['OS=="linux"', {
            'cflags': [
              '-std=c++11' # optional?
            ],
            'ldflags': [
              '-lX11',
              '-lGL',
              '-lGLU',
              '-lpthread'
            ],
            'defines': [
              'OS_LINUX'
            ]
          }],

        ],
      },
      'export_dependent_settings': [
        # 'other_target_in_this_file',
      ],
      'conditions': [
        ['OS=="win"', {
          'defines': [
              'OS_WINDOWS'
          ],
           'sources': [
            # Widnows stuff
            'win32/rectangle.cc',
            'win32/rectangle.h',
            'win32/window-handles.cc',
            'win32/window-handles.h',
            'win32/window.cc',
            'win32/window.h'
          ],
          'cflags': [
            '/Wall',
            '/O2'
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
        }],
        ['OS=="linux"', {
          'cflags': [
            '-std=c++11' # optional?
          ],
          'ldflags': [
            '-lX11',
            '-lGL',
            '-lGLU',
            '-lpthread'
          ],
          'defines': [
              'OS_LINUX'
          ],
           'sources': [
              # Add sources here
              'window-xlib.cc',
              'window-xlib.h'
            ],
        }],
      ],
    },
  ],
}
