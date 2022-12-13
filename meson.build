project(
    'ez2d',
    'cpp',
    version: '0.1',
    default_options: [
        'warning_level=3',
        'cpp_std=c++latest',
        'default_library=static',
    ],
)

fmt_dep = dependency('fmt')
sdl2_dep = dependency('sdl2')

compiler = meson.get_compiler('cpp')

if compiler.get_id() == 'gcc' or compiler.get_id() == 'clang'
    add_project_arguments([
        '-Wpedantic',
        '-Wconversion',
        '-Wshadow',
    ], language: 'cpp')
elif compiler.get_id() == 'msvc' or compiler.get_id() == 'clang-cl'
    add_project_arguments([
        '/WX',
    ], language: 'cpp')
endif

if build_machine.system() == 'windows'
    add_project_arguments(['-DON_WINDOWS'], language: 'cpp')
else if build_machine.system() == 'linux'
    add_project_arguments(['-DON_LINUX'], language 'cpp')
else
    error('not setup for building on', build_machine.system(), '(yet)')
endif

source_inc = include_directories('src')
cpp_sources = []
subdir('src')

exe = executable(
    'ez2d',
    cpp_sources,
    install: true,
    override_options: [
        'werror=true',
    ],
    include_directories: [
        source_inc,
    ],
    dependencies: [
        fmt_dep,
        sdl2_dep,
    ],
)

