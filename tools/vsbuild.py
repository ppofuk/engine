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


def RunBuild(format, ninja):
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
        if ninja == "True":
            ninja_dir = os.path.abspath(os.path.join(script_dir, 'ninja'))
            ninja_path = os.path.abspath(os.path.join(ninja_dir,
                                                      'ninja.exe'))
            debug_dir = os.path.abspath(os.path.join(out_dir, format))
            args = [ninja_path, '-C', debug_dir]
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
        parser.add_argument("-n", "--ninja", type=str, default="True",
                            help="-n=False disables ninja and uses msvc")

        args = parser.parse_args()
        if RunBuild(args.format, args.ninja) == False:
            print('Build failed')
    else:
        print('This is only for win32.')
