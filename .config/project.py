# -*- encoding: utf-8 -*-

PROJECT_NAME = 'cof'

from tupcfg import Dependency
from tupcfg.lang import cxx
from tupcfg.lang import c
from tupcfg import tools, path, platform

class LibGit2(Dependency):
    def __init__(self, compiler, source_directory, shared = True):
        super().__init__("LibGit2", 'libgit2')
        self.compiler = compiler
        self.source_directory = source_directory
        ext = self.compiler.library_extension(shared)
        self.library_filename = 'libgit2.%s' % ext
        self.shared = shared

    @property
    def targets(self):
        from tupcfg.command import Shell as ShellCommand
        from tupcfg import Target
        configure_target = Target(
            'libgit2/Makefile',
            ShellCommand(
                "Configure libgit2",
                [
                    'cmake',
                    path.absolute(self.source_directory),
                    '-DCMAKE_C_COMPILER=%s' % self.compiler.binary,
                    '-DUSE_ICONV=ON',
                    '-DTHREADSAFE=OFF',
                ],
                working_directory = self.build_path(),
            )
        )
        build_target = Target(
            path.join('libgit2/', self.library_filename),
            ShellCommand(
                "Building libgit2",
                ['make'],
                working_directory = self.build_path(),
                dependencies = [configure_target]
            )
        )
        return [build_target]

    @property
    def libraries(self):
        return [
            cxx.Library(
                'git2',
                self.compiler,
                shared = self.shared,
                search_binary_files = False,
                include_directories = [
                    path.join(self.source_directory, 'include', abs = True)
                ],
                directories = [self.build_path(abs = True)],
                files = [self.build_path(self.library_filename, abs = True)],
                save_env_vars = False,
            )
        ]



def configure(project, build):

    ## Retreive BUILD_TYPE (defaults to DEBUG)
    build_type = project.env.get('BUILD_TYPE', 'DEBUG')
    project.env.build_set('BUILD_TYPE', build_type)

    ## Print a status message (Could have been verbose or debug)
    tools.status(
        "Configuring", project.env.PROJECT_NAME,
        'in', build.directory, '(%s)' % build_type
    )

    cxx_compiler = cxx.find_compiler(
        project,
        build,
        include_directories = ['src'],
        standard = 'c++11',
    )
    c_compiler = c.find_compiler(project, build)

    libgit2 = build.add_dependency(LibGit2(c_compiler, "deps/libgit2"))

    cxx_compiler.link_executable(
        'bin/cof',
        ['src/cof/main.cpp', 'src/cof/test.cpp'],
        libraries = libgit2.libraries,
    )
