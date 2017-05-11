# third-party/cef-linux/
{
    'includes': [
        '../../common.gypi'
    ],

    'targets': [
        {
            'target_name': 'cef',
            'type': '<(obsidian_library)',
            'defines': [
                'USE_CEF'
            ],
            'include_dirs': [
                '.',
                '../../third-party/cef-linux/include/',
                '../../third-party/cef-linux/'
            ],
            'dependencies': [
                '../library.gyp:library',
                '../../third-party/cef.gyp:cef_wrapper'
            ],
            'sources': [
                'cef.h',
                'cef.cc'
            ],
            'direct_dependent_settings': {
                'include_dirs': [
                    '.',
                    '../../third-party/cef-linux/include/',
                    '../../third-party/cef-linux'
                ],
                'conditions': [
                    ['OS=="win"', {
                        'defines': [
                            'USE_CEF_WIN'
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
                        'libraries': [
                            '-lX11',
                            '-L../../third-party/cef-linux/Release',
                            '-lcef'
                        ],
                        'defines': [
                            'USE_CEF_LINUX'
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
                        'USE_CEF_WIN'
                    ],
                    'sources': [
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
                        '-L../../third-party/cef-linux/Release',
                        '-lcef'
                    ],
                    'defines': [
                        'USE_CEF_LINUX'
                    ],
                    'sources': [
# Add sources here
                        
                    ],
                }],
            ],
        },
    ],
}
