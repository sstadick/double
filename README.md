# double

Take a list of words, and create all combinations of them concatinated, and joined by one space.

EX:

```
#list
hello
world

helloworld
helo world
worldhello
world hello
```

Inspired by a random [post](https://www.reddit.com/r/golang/comments/bb4sgb/go_vs_perl_simple_task_speed_comparison/) on reddit. I really wanted to make a C version faster than go.

Current flags for compiling it fast:

```
clang -O2 -pipe -march=native -mtune=native -o double double.c
```

Pipe in or pass as argument. The current version is around .01 secons faster than the Go version. The secrets were fputs_unlocked to avoid locking and to not format the output string, and to use posix_fadvise, especially on the output stream.

## Very rough benchmarks

```
ducktape@marvin:~/Code/tmp/double$ time ./double /tmp/list > /tmp/doubled.txt

real	0m0.093s
user	0m0.073s
sys	0m0.020s
# go build godouble.go
ducktape@marvin:~/Code/tmp/double$ time ./godouble /tmp/list > /tmp/doubled.txt

real	0m0.105s
user	0m0.069s
sys	0m0.036s
ducktape@marvin:~/Code/tmp/double$ time python3 double.py /tmp/list > /tmp/doubled.txt

real	0m0.383s
user	0m0.343s
sys	0m0.040s
ducktape@marvin:~/Code/tmp/double$ time perl double.pl /tmp/list > /tmp/doubled.txt

real	0m0.361s
user	0m0.329s
sys	0m0.032s
```
