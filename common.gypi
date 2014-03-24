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
		  'cflags': [
        '/ZI'
		  ],
      'ldflags': [
        '/debug'
      ]
    },
    'Release': {
		  'defines': [
		    'NDEBUG'
		  ],
      'cflags': [
        '/Zi'
      ]
    }
	 }
  }

}
