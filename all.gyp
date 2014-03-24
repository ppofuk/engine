# Copyright (C) 2014 Petar Pofuk. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.
{
  'includes': [
    'common.gypi'
  ],
  'targets': [
    {
      'target_name': 'all',
      'type': 'none',
      'dependencies': [
        'app/app.gyp:*'
      ]
    }
  ]
}
