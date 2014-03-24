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
all_path = os.path.abspath(os.path.join(root_dir, 'all.sln'))
app_dir = os.path.abspath(os.path.join(root_dir, 'app'))
app_path = os.path.abspath(os.path.join(app_dir, 'app.sln'))
out_dir = os.path.abspath(os.path.join(root_dir, 'out'))

def RunBuild(format):
    vsvars_dir = script_dir
    if sys.platform in ('cygwin', 'win32'):
        vsvars_path = os.path.abspath(os.path.join(vsvars_dir, 'vsvars32.bat'))

    #args = [vsvars_path]
    #if subprocess.call(args) != 0:
    #    print('vsvars32.bat call failed!')
    #    return False

    if format == "Clean":
        args = ['MSBuild.exe',
            all_path,
            '/target:Clean']
    else:
        args = ['MSBuild.exe',
                all_path,
                '/property:Configuration=' + format]

    print(args)

    return subprocess.call(args) == 0


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    if sys.platform in ('win32'):
        parser.add_argument("-f", "--format", type=str, default="Debug",
                            help="Configuration to be used with MSBuild or Clean")

    args = parser.parse_args()
    if RunBuild(args.format) == False:
        print('Build failed')

