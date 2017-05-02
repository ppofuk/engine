{
  'includes': [
    '../common.gypi',
  ],

  'targets': [
    {
      'target_name': 'glew',
      'type': 'static_library',
      'include_dirs': [
        'glew/include/'
      ],
      'defines': [
        'GLEW_STATIC'
      ],

      'all_dependent_settings': {
        'include_dirs': [
          'glew/include/'
        ],
        'defines': [
          'GLEW_STATIC'
        ],
      },
      'sources': [
        'glew/src/glew.c'
      ],

      'conditions': [
        ['OS=="win"',
          {
            'all_dependent_settings': {
              'libraries': [
                '-lOpenGL32.lib'
              ]
            }
          }
        ], # OS==win
        ['OS=="linux"',
          {
            'all_dependent_settings': {
              'defines': [
                'GL_GLEXT_PROTOTYPES'
              ],
              'libraries': [
                '-lGL',
                '-lGLU',
                '-lX11'
              ]
            }
          }
        ] # OS==linux
      ] # conditions
    }
  ]
}
