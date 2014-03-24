{

  'variables': {
    'library': 'static_library'
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
            '-ggdb'
          ]
        }]
      ],
    },
    'Release': {
		  'defines': [
		    'NDEBUG'
		  ]
    }
	 }
  }

}
