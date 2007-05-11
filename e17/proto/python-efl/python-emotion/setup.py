import sys
import os

from ez_setup import use_setuptools
use_setuptools('0.6c3')

import distutils.sysconfig
from setuptools import setup, find_packages, Extension
import commands


python_inc = distutils.sysconfig.get_python_inc()


def pkgconfig(*packages, **kw):
    flag_map = {'-I': 'include_dirs', '-L': 'library_dirs', '-l': 'libraries'}
    pkgs = ' '.join(packages)
    cmdline = 'pkg-config --libs --cflags %s' % pkgs

    status, output = commands.getstatusoutput(cmdline)
    if status != 0:
        raise ValueError("could not find pkg-config module: %s" % pkgs)

    for token in output.split():
        kw.setdefault(flag_map.get(token[:2]), []).append(token[2:])
    return kw


emotionmodule = Extension('emotion.c_emotion',
                          sources=['emotion/emotion.c_emotion.pyx',
                                   ],
                          depends=['emotion/emotion.c_emotion.pxd',
                                   ],
                          **pkgconfig('"emotion >= 0.0.1.005"'))


include_dirs = [os.path.join(python_inc, "python-evas")]

setup(name = 'python-emotion',
      version = '0.1.0',
      license = 'BSD',
      author = 'Gustavo Sverzut Barbieri',
      author_email = 'barbieri@gmail.com',
      url = 'http://www.enlightenment.org/',
      description = 'Python bindings for Emotion',
      packages = find_packages(),
      include_dirs = include_dirs,
      install_requires = ['python-evas>=0.1.0'],
      setup_requires = ['python-evas>=0.1.0'],
      ext_modules = [emotionmodule],
      zip_safe = False,
      )
