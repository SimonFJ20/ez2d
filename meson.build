project(
    'ez2d',
    'cpp',
    version: '0.1',
    default_options: [
        'warning_level=3',
        'cpp_std=c++20',
        'default_library=static',
    ],
)

fmt_dep = dependency('fmt')
argparse_dep = dependency('argparse')
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
    add_project_arguments(['-DWINDOWS'], language: 'cpp')
elif build_machine.system() == 'linux'
    add_project_arguments(['-DLINUX'], language: 'cpp')
else
    error('not setup for building on', build_machine.system(), '(yet)')
endif

common_inc = include_directories('common')

server_inc = include_directories('server')
server_sources = []
subdir('server')

client_inc = include_directories('client')
client_sources = []
subdir('client')

ez2d_server = executable(
    'ez2d-server',
    server_sources,
    override_options: [
        'werror=true',
    ],
    include_directories: [
        common_inc,
        server_inc,
    ],
    dependencies: [
        fmt_dep,
        argparse_dep,
        sdl2_dep,
    ],
)

ez2d_client = executable(
    'ez2d-client',
    client_sources,
    override_options: [
        'werror=true',
    ],
    include_directories: [
        common_inc,
        client_inc,
    ],
    dependencies: [],
)

