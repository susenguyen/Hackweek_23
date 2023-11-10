# Throughput & Latency Tests with scp

firefox-tumbleweed.tgz 520704771 

## Nothing
```
- 181.8 MB/s
- 190.7 MB/s
- 179.2 MB/s
- 178.9 MB/s
- 181.6 MB/s

avg = 182.4 MB/s
std dev = 4.80/2.6%
```

## libnetfilter_queue
- buffer size = 1638400 bytes

```
- 140.1 MB/s
- 132.2 MB/s
- 147.8 MB/s
- 150.8 MB/s
- 139.6 MB/s

avg = 142.1
std dev = 7.35/5.2%
```

- default buffer size
```
- 140.1 MB/s
- 101.7 MB/s
- 149.2 MB/s
- 132.7 MB/s
- 145.0 MB/s

avg = 133.7
std dev = 18.9/14.2%
```

## NF_HOOK
```
- 152.1 MB/s
- 159.0 MB/s
- 177.8 MB/s
- 176.3 MB/s
- 161.4 MB/s

avg = 165.3
std dev = 11.25/6.8%
```

## eBPF XDP
```
- 163.0 MB/s
- 164.0 MB/s
- 168.8 MB/s
- 156.6 MB/s
- 161.0 MB/s

avg = 162.7
std dev = 4.45/2.7%
```
