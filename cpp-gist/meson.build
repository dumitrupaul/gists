project('cpp-gist', 'cpp', default_options: ['cpp_std=c++26'],  license: 'BSD 3-Clause')
thread_dep = dependency('threads')
progs = [['bst', ['bst.cc']],
        ['demo', ['main.cc']]]

foreach p : progs
  exe = executable(p[0], p[1], dependencies: [thread_dep])
endforeach