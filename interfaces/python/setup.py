# coding=utf-8
import glob

from setuptools import setup, find_packages
from setuptools.command.build_ext import build_ext
from distutils.core import setup, Extension
from codecs import open
from os import path
import sys
import platform

here = path.abspath(path.dirname(__file__))
base = here
src_dir = path.abspath(path.join(here, 'src'))
vendordir = path.abspath(path.join(here, 'vendor'))
builddir = path.abspath(path.join(here, '.build'))


# Avoid meaningless gcc warning - see https://stackoverflow.com/a/49041815
class BuildExt(build_ext):
    def build_extensions(self):
        self.compiler.compiler_so = [x for x in self.compiler.compiler_so if x != '-Wstrict-prototypes']
        super(BuildExt, self).build_extensions()


# Get the long description from the README file
with open(path.join(base, 'README.md'), encoding='utf-8') as f:
    long_description = f.read()


def main():
    debug_mode = '--debug' in sys.argv  # Haven't found yet a more robust alternative to this

    install_requires = [
        'setuptools',
        'tarski'
    ]

    module = Extension(
        'acado._acado',

        language="c++",

        sources=glob.glob('src/extension/*.cxx'),

        include_dirs=[
            src_dir,
            '/usr/local/include',
        ],

        library_dirs=[builddir],

        runtime_library_dirs=[builddir],

        extra_objects=[],  # Static library could go in here, as per https://stackoverflow.com/a/49139257

        extra_compile_args=['-std=c++14'],

        libraries=['acado_toolkit_s']
    )

    setup(
        name='pyacado',
        version='0.1.0',
        description='ACADO.py: Python bindings for the ACADO toolkit',
        long_description=long_description,
        url='https://github.com/miquelramirez/acado',
        author='Miquel Ramírez',
        author_email='miquel.ramirez@gmail.com',

        keywords='optimal-control non-linear-programming',
        classifiers=[
            'Development Status :: 3 - Alpha',

            'Intended Audience :: Science/Research',
            'Topic :: Scientific/Engineering :: Artificial Intelligence',

            'License :: OSI Approved :: MIT License',

            'Programming Language :: Python :: 3',
            'Programming Language :: Python :: 3.4',
            'Programming Language :: Python :: 3.5',
            'Programming Language :: Python :: 3.6',
        ],

        # You can just specify package directories manually here if your project is
        # simple. Or you can use find_packages().
        #
        # Alternatively, if you just want to distribute a single Python file, use
        # the `py_modules` argument instead as follows, which will expect a file
        # called `my_module.py` to exist:
        #
        #   py_modules=["my_module"],
        #
        packages=find_packages('src'),  # include all packages under src
        package_dir={'': 'src'},  # tell distutils packages are under src


        # This field lists other packages that your project depends on to run.
        # Any package you put here will be installed by pip when your project is
        # installed, so they must be valid existing projects.
        #
        # For an analysis of "install_requires" vs pip's requirements files see:
        # https://packaging.python.org/en/latest/requirements.html
        install_requires=install_requires,

        # List additional groups of dependencies here (e.g. development
        # dependencies). Users will be able to install these using the "extras"
        # syntax, for example:
        #
        #   $ pip install sampleproject[dev]
        #
        extras_require={
            'dev': ['pytest', 'tox'],
            'test': ['pytest', 'tox'],
        },


        # To provide executable scripts, use entry points in preference to the
        # "scripts" keyword. Entry points provide cross-platform support and allow
        # `pip` to create the appropriate form of executable for the target
        # platform.
        #
        # For example, the following would provide a command called `sample` which
        # executes the function `main` from this package when invoked:
        # entry_points={
        #     'console_scripts': [
        #         'sample=sample:main',
        #     ],
        # },

        # This will include non-code files specified in the manifest, see e.g.
        # http://python-packaging.readthedocs.io/en/latest/non-code-files.html
        # include_package_data=True,

        cmdclass={'build_ext': BuildExt},
        ext_modules=[module],
    )


if __name__ == '__main__':
    main()
