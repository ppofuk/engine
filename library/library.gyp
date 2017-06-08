{
  'includes': [
    '../common.gypi'
  ],

  'targets': [
    {
      'target_name': 'library',
      'type': '<(obsidian_library)',
      'defines': [
        'LIBRARY_EXPORT'
      ],
      'include_dirs': [
        '.',
        '../third-party/glm/'
      ],
      'dependencies': [
        '../third-party/glew.gyp:glew'
      ],
      'sources': [
        'basic-types.h',
        'linked-list.h',
        'logger.cc',
        'logger.h',
        'singleton.h',
        'reader-inl.h',
        'time-ticker.cc',
        'time-ticker.h',
        'static-compile-options.h',
        'memory-pool.h',
        'memory.h',
        'vector2.h',
        'vector3.h',
        'vector4.h',
        'matrix4.h',
        'matrix-utility.h',
        'matrix-utility.cc',
        'c-json.h',
        'c-json.cc',
        'properties.h',
        'properties.cc',
        # Render
        'render/texture-loader-abstract.h',
        'render/texture-loader.cc',
        'render/texture-loader.h',
        # Render: OpenGL stuff:
        'render/gl-buffer.h',
        'render/gl-buffer.cc',
        'render/gl-program.h',
        'render/gl-program.cc',
        'render/gl-shader-object.h',
        'render/gl-shader-object.cc',
        'render/gl-texture-2d.h',
        'render/gl-texture-2d.cc',
        'render/gl-uniform.h',
        'render/gl-uniform.cc',
        'render/gl-attribute.h',
        'render/gl-attribute.cc',
        'render/gl-shader.h',
        'render/gl-shader.cc',
        'render/gl-sprite-shader.h',
        'render/gl-sprite-shader.cc',
        'render/gl-pbo-texture-2d.h',
        'render/gl-pbo-texture-2d.cc'
      ],
      'direct_dependent_settings': {
        'include_dirs': ['.', '../third-party/glm/'],
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
              '-std=c++14' # optional?
            ],
            'ldflags': [
              '-lX11',
              #'-lGL',
              #'-lGLU',
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
            'win32/window.h',
            'win32/wgl-context.h',
            'win32/wgl-context.cc'
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
