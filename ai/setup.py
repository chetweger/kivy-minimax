from distutils.core import setup, Extension
setup(
        name='helloworld',
        version='1.0',
        ext_modules=[
            Extension(
                'helloworld',
                ['interface.cpp'],
                extra_compile_args=['-std=c++11', '-O0'],
                extra_link_args=['-std=c++11', '-O0'],
                )
            ]
        )

# , extra_compile_args=['-std=c++11', '-O0']
