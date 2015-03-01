from distutils.core import setup, Extension
setup(
        name='meta_ttt_ai',
        version='1.0',
        ext_modules=[
            Extension(
                'meta_ttt_ai',
                ['interface.cpp'],
                extra_compile_args=['-std=c++11', '-O3'],
                extra_link_args=['-std=c++11', '-O3'],
                )
            ]
        )

# , extra_compile_args=['-std=c++11', '-O0']
