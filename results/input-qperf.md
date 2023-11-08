# Throughput & Latency Tests with qperf

- Laptop unplugged => CPU throttled

## Nothing
- qperf 10.1.0.2 -t 30 tcp_bw

```
tcp_bw:
    bw  =  2.59 GB/sec
    bw  =  2.66 GB/sec
    bw  =  2.63 GB/sec
    bw  =  2.65 GB/sec
    bw  =  2.57 GB/sec
```

- qperf 10.1.0.2 -t 30 tcp_lat
```
tcp_lat:
    latency  =  29.2 us
    latency  =  29.1 us
    latency  =  29.5 us
    latency  =  28.6 us
    latency  =  24.8 us
```

## libnetfilter_queue - no str_str()
- qperf 10.1.0.2 -t 30 tcp_bw
- Note: requires a little more code to pull the packet from the queue => not as stripped down
- Strong variance <= user space => renice -20 -p PID

- rcvbufsiz = 8192 bytes
```
tcp_bw:
    bw  =  227 MB/sec   (renice)    203 MB/sec
    bw  =  176 MB/sec               222 MB/sec
    bw  =  124 MB/sec               283 MB/sec
    bw  =  165 MB/sec               184 MB/sec
    bw  =  138 MB/sec               223 MB/sec
```

- qperf 10.1.0.2 -t 30 tcp_lat
- renice -20 -p PID

```
tcp_lat:
    latency  =  37 us
    latency  =  41.2 us
    latency  =  39.3 us
    latency  =  41.3 us
    latency  =  39.2 us
```

## libnetfilter_queue - with str_str()
- qperf 10.1.0.2 -t 30 tcp_bw

- rcvbufsiz = 8192 bytes
```
tcp_bw:
    bw  =  112 MB/sec
    bw  =  127 MB/sec
    bw  =  115 MB/sec
    bw  =  112 MB/sec
    bw  =  118 MB/sec
```

- qperf 10.1.0.2 -t 30 tcp_lat

```
tcp_lat:
    latency  =  47.4 us
    latency  =  45 us
    latency  =  42.6 us
    latency  =  45.6 us
    latency  =  42.8 us
```

- rcvbufsiz = 1638400 bytes
```
tcp_bw:
    bw  =  313 MB/sec
    bw  =  314 MB/sec
    bw  =  317 MB/sec
    bw  =  265 MB/sec
    bw  =  267 MB/sec
```

```
tcp_lat:
    latency  =  38.5 us
    latency  =  38.6 us
    latency  =  40.6 us
    latency  =  39.4 us
    latency  =  38.9 us
```

- rcvbufsiz = 2^31 bytes (no ENOBUF during run)
    - shows no improvement...


## NF_HOOK - no str_str()
- qperf 10.1.0.2 -t 30 tcp_bw
```
tcp_bw:
    bw  =  2.65 GB/sec
    bw  =  2.61 GB/sec
    bw  =  2.66 GB/sec
    bw  =  2.66 GB/sec
    bw  =  2.68 GB/sec
```

- qperf 10.1.0.2 -t 30 tcp_lat
```
tcp_lat:
    latency  =  28.7 us
    latency  =  30.1 us
    latency  =  27.6 us
    latency  =  27.9 us
    latency  =  27.7 us
```

## NF_HOOK - with str_str()
- qperf 10.1.0.2 -t 30 tcp_bw
```
tcp_bw:
    bw  =  1.24 GB/sec
    bw  =  1.25 GB/sec
    bw  =  1.27 GB/sec
    bw  =  1.26 GB/sec
    bw  =  1.24 GB/sec
```

- qperf 10.1.0.2 -t 30 tcp_lat
```
tcp_lat:
    latency  =  27.3 us
    latency  =  27.9 us
    latency  =  27.2 us
    latency  =  27.0 us
    latency  =  27.4 us
```

## eBPF XDP - no str_str()
- qperf 10.1.0.2 -t 30 tcp_bw
```
tcp_bw:
    bw  =  298 MB/sec
    bw  =  302 MB/sec
    bw  =  306 MB/sec
    bw  =  315 MB/sec
    bw  =  311 MB/sec
```

- qperf 10.1.0.2 -t 30 tcp_lat
```
tcp_lat:
    latency  =  30.7 us
    latency  =  28.3 us
    latency  =  30.9 us
    latency  =  29.3 us
    latency  =  29.1 us
```

## eBPF XDP - with str_str()
- qperf 10.1.0.2 -t 30 tcp_bw
```
tcp_bw:
    bw  =  291 MB/sec
    bw  =  289 MB/sec
    bw  =  290 MB/sec
    bw  =  294 MB/sec
    bw  =  290 MB/sec
```

- qperf 10.1.0.2 -t 30 tcp_lat
```
tcp_lat:
    latency  =  29.5 us
    latency  =  28.7 us
    latency  =  27 us
    latency  =  28.5 us
    latency  =  28.2 us
```

## Nothing - titi
- qperf 192.168.1.16 -t 30 tcp_bw
```
tcp_bw:
    bw  =  43.2 MB/sec
    bw  =  43.2 MB/sec
    bw  =  43.2 MB/sec
    bw  =  43.2 MB/sec
    bw  =  43.1 MB/sec
```

- qperf 192.168.1.16 -t 30 tcp_lat
```
tcp_lat:
    latency  =  93.2 us
    latency  =  92.5 us
    latency  =  92.8 us
    latency  =  92.3 us
    latency  =  92.3 us
```

## libnetfilter_queue with str_str() - titi
- qperf 192.168.1.16 -t 30 tcp_bw
```
tcp_bw:
    bw  =  43.3 MB/sec
    bw  =  43.3 MB/sec
    bw  =  44.1 MB/sec
    bw  =  43.3 MB/sec
    bw  =  43.4 MB/sec
```

- qperf 192.168.1.16 -t 30 tcp_lat
```
tcp_lat:
    latency  =  103 us
    latency  =  98 us
    latency  =  103 us
    latency  =  104 us
    latency  =  101 us
```

## NF_HOOK with str_str() - titi
- qperf 192.168.1.16 -t 30 tcp_bw
```
tcp_bw:
    bw  =  43.1 MB/sec
    bw  =  43.1 MB/sec
    bw  =  43.1 MB/sec
    bw  =  43.1 MB/sec
    bw  =  43.1 MB/sec
```

- qperf 192.168.1.16 -t 30 tcp_lat
```
tcp_lat:
    latency  =  93.8 us
    latency  =  94.7 us
    latency  =  93.4 us
    latency  =  92.6 us
    latency  =  93.8 us
```
