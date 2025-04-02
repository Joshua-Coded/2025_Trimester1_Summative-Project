from setuptools import setup, Extension

# Define the extension module
stat_module = Extension('stat_module',
                       sources=['stat_module.c'],
                       extra_compile_args=['-std=c99'],
                       libraries=['m'])  # Link with math library

# Setup configuration
setup(name='stat_module',
      version='1.0',
      description='Statistical functions for arrays',
      ext_modules=[stat_module])
