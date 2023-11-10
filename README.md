# HACKWEEK 23

## Objective

Compare a network filtering function across 3 options:
- libnetfilter\_queue
- NF\_HOOKS
- eBPF eXpress Data Path

## Methodology

Build 3 simple programs with the same filtering function and compare their performance:
- Throughput
- Response times

## Benchmarks

- qperf for throughput and latency
- scp to test a more "usual" workload for throughput

