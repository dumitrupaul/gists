project('advent-of-code-2024', 'cpp', default_options: ['cpp_std=c++26'],  license: 'BSD 3-Clause')
libf = dependency('fmt')
thread_dep = dependency('threads')
progs = [['day1', ['day1.cc']],
         ['day5', ['day5.cc']],
         ['day4', ['day4.cc']],
         ['day3', ['day3.cc']],
         ['day2', ['day2.cc']]]

foreach p : progs
  exe = executable(p[0], p[1], dependencies: [libf, thread_dep])
endforeach