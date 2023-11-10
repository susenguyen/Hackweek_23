# Throughput & Latency Tests with qperf

- Laptop power plugged => more deterministic!

## Nothing
```
qperf 10.1.0.2 -t 30 tcp_bw

tcp_bw:
    bw  =  3.92 GB/sec
    bw  =  3.69 GB/sec
    bw  =  3.58 GB/sec
    bw  =  3.62 GB/sec
    bw  =  3.52 GB/sec

avg = 3.67
std dev = 0.15/4.2%
```

```
qperf 10.1.0.2 -t 30 tcp_lat

tcp_lat:
    latency  =  18.2 us
    latency  =  18.3 us
    latency  =  18.7 us
    latency  =  21.6 us
    latency  =  18.5 us

avg = 19.1
std dev = 1.43/7.5%
```

## libnetfilter_queue - no str_str()
- Note: requires a little more code to pull the packet from the queue => not as stripped down
- Strong variance <= user space

```
qperf 10.1.0.2 -t 30 tcp_bw
(rcvbufsiz = 8192 bytes)
tcp_bw:
    bw  =  423 MB/sec
    bw  =  406 MB/sec
    bw  =  412 MB/sec
    bw  =  437 MB/sec
    bw  =  533 MB/sec

avg = 442
std dev = 52.1/11.8%
```

```
qperf 10.1.0.2 -t 30 tcp_bw
renice -20 -p [test pid]
(rcvbufsiz = 1638400 bytes)

tcp_bw:
    bw  =  602 MB/sec
    bw  =  572 MB/sec
    bw  =  539 MB/sec
    bw  =  523 MB/sec
    bw  =  543 MB/sec

avg = 556
std dev = 31.3/5.6%
```

```
qperf 10.1.0.2 -t 30 tcp_lat
renice -20 -p [test pid]
(rcvbufsiz = 1638400 bytes)

tcp_lat:
    latency  =  33.5 us
    latency  =  30 us
    latency  =  32.8 us
    latency  =  33 us
    latency  =  34.6 us

avg = 32.8
std dev = 1.70/5.2%
```

## libnetfilter_queue - with str_str()

```
qperf 10.1.0.2 -t 30 tcp_bw
renice -20 -p [test pid]
(rcvbufsiz = 1638400 bytes)

tcp_bw:
    bw  =  415 MB/sec
    bw  =  393 MB/sec
    bw  =  374 MB/sec
    bw  =  428 MB/sec
    bw  =  373 MB/sec

avg = 397
std dev = 24.5/6.2%
```

```
qperf 10.1.0.2 -t 30 tcp_lat
renice -20 -p [test pid]
(rcvbufsiz = 1638400 bytes)

tcp_lat:
    latency  =  31.6 us
    latency  =  32.8 us
    latency  =  32.2 us
    latency  =  32.8 us
    latency  =  33.9 us

avg = 32.7
std dev = 0.85/2.6%
```

```
rcvbufsiz maxed out at 2^31 bytes (no ENOBUF during run)
    => shows no improvement...
```


## NF_HOOK - no str_str()
```
qperf 10.1.0.2 -t 30 tcp_bw

tcp_bw:
    bw  =  3.68 GB/sec
    bw  =  3.64 GB/sec
    bw  =  3.71 GB/sec
    bw  =  3.5 GB/sec
    bw  =  3.53 GB/sec

avg = 3.61
std dev = 0.0925/2.6%
```

```
qperf 10.1.0.2 -t 30 tcp_lat

tcp_lat:
    latency  =  22.6 us
    latency  =  20.5 us
    latency  =  21.9 us
    latency  =  20.2 us
    latency  =  21.5 us

avg = 21.3
std dev = 0.99/4.6%
```

## NF_HOOK - with str_str()
```
qperf 10.1.0.2 -t 30 tcp_bw

tcp_bw:
    bw  =  1.81 GB/sec
    bw  =  1.75 GB/sec
    bw  =  1.84 GB/sec
    bw  =  1.74 GB/sec
    bw  =  1.81 GB/sec

avg = 1.79
std dev = 0.043/2.4%
```

```
qperf 10.1.0.2 -t 30 tcp_lat

tcp_lat:
    latency  =  17.9 us
    latency  =  22.7 us
    latency  =  17.7 us
    latency  =  19.8 us
    latency  =  19.3 us

avg = 19.5
std dev = 2.01/10.3%
```

## eBPF XDP - no str_str()
```
qperf 10.1.0.2 -t 30 tcp_bw

tcp_bw:
    bw  =  399 MB/sec
    bw  =  392 MB/sec
    bw  =  397 MB/sec
    bw  =  381 MB/sec
    bw  =  380 MB/sec

avg = 390
std dev = 8.87/2.3%
```

```
qperf 10.1.0.2 -t 30 tcp_lat

tcp_lat:
    latency  =  20.8 us
    latency  =  21.1 us
    latency  =  22.2 us
    latency  =  23.6 us
    latency  =  21.2 us

avg = 21.8
std dev = 1.14/5.3%
```

## eBPF XDP - with str_str()
```
qperf 10.1.0.2 -t 30 tcp_bw

tcp_bw:
    bw  =  487 MB/sec
    bw  =  451 MB/se
    bw  =  472 MB/sec
    bw  =  464 MB/sec
    bw  =  465 MB/sec

avg = 468
std dev = 13.1/2.8%
```

```
qperf 10.1.0.2 -t 30 tcp_lat

tcp_lat:
    latency  =  22.2 us
    latency  =  22.6 us
    latency  =  19.4 us
    latency  =  19.9 us
    latency  =  21.6 us

avg = 21.1
std dev = 1.42/6.7%
```
