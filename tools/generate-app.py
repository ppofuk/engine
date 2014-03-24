#!/usr/bin/python

# Copyright (C) 2014 Petar Pofuk. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

import argparse
import subprocess
import pipes
import sys
import os

script_dir = os.path.dirname(os.path.realpath(__file__))
root_dir = os.path.abspath(os.path.join(script_dir, os.pardir))
all_gyp_path = os.path.abspath(os.path.join(root_dir, 'all.gyp'))
app_dir = os.path.abspath(os.path.join(root_dir, 'app'))
app_path = os.path.abspath(os.path.join(app_dir, 'app.gyp'))
out_dir = os.path.abspath(os.path.join(root_dir, 'out'))

def RunGyp(format):
    gyp_dir = os.path.abspath(os.path.join(script_dir, 'gyp'))
    if sys.platform in ('cygwin', 'win32'):
        gyp_path = os.path.abspath(os.path.join(gyp_dir, 'gyp.bat')) # untested
    elif sys.platform.startswith('linux'):
        gyp_path = os.path.abspath(os.path.join(gyp_dir, 'gyp'))

    args = [gyp_path,
            '--toplevel-dir=' + root_dir,
            '--format=' + format,
            '--depth=' + root_dir,
            all_gyp_path]
    return subprocess.call(args) == 0

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    if sys.platform in ('win32'):
        parser.add_argument("-f", "--format", type=str, default="msvs",
                            help="Specifiy build systems to generate.")
    elif sys.platform.startswith('linux'):
        parser.add_argument("-f", "--format", type=str, default="ninja",
                            help="Specifiy build systems to generate.")
    else:
        parser.add_argument("-f", "--format", type=str, default="make",
                            help="Specifiy build systems to generate.")

    args = parser.parse_args()
    if RunGyp(args.format) == False:
        print('Gyp call failed')

