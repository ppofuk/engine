{

    'variables': {
        'obsidian_library': 'static_library'
    },

    'target_defaults': {
        'default_configuration': 'Debug',
        'configurations': {
            'Debug': {
	        'defines': [
		    'DEBUG', '_DEBUG'
	        ],
                'conditions': [
                    ['OS=="win"', {
                        'cflags': [
                            '/ZI'
                        ],
                        'ldflags': [
                            '/debug'
                        ]
                    }],
                    ['OS=="linux"', {
                        'cflags': [
                            '-Wall',
                            '-ggdb'
                        ]
                    }]
                ],
            },
            'Release': {
		'defines': [
		    'NDEBUG'
		],
                'conditions': [
                    ['OS=="win"', {
                        'cflags': [
                            '/ZI'
                        ],
                        'ldflags': [
                            '/debug'
                        ]
                    }],
                    ['OS=="linux"', {
                        'cflags': [
                            '-O3'
                        ]
                    }]
                ]
            }
	}
    }

}
