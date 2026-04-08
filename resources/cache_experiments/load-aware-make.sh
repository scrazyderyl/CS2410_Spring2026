#!/usr/bin/env bash
#
# load-aware-make.sh
# Runs "make" only when the system's CPU usage (sampled over a
# short window) is below a configurable threshold.
#
# Usage:
#   ./load-aware-make.sh [OPTIONS] [-- MAKE_ARGS...]
#
# Options:
#   -t THRESHOLD   CPU usage % threshold (default: 75)
#   -s SAMPLE      Sample window in seconds (default: 10)
#   -i INTERVAL    Seconds between checks in wait mode (default: 10)
#   -w             Wait mode: keep polling until usage drops (default: exit immediately)
#
# Examples:
#   ./load-aware-make.sh                        # Run make if CPU < 75%
#   ./load-aware-make.sh -t 50 -s 5 -w         # Wait until CPU < 50% (5s sample)
#   ./load-aware-make.sh -t 60 -- -j4 all      # Pass "-j4 all" to make

set -euo pipefail

# --- Defaults ---
THRESHOLD=2
SAMPLE_WINDOW=10
INTERVAL=10
WAIT_MODE=false
MAKE_ARGS=()

# --- Parse arguments ---
while [[ $# -gt 0 ]]; do
    case "$1" in
        -t) THRESHOLD="$2"; shift 2 ;;
        -s) SAMPLE_WINDOW="$2"; shift 2 ;;
        -i) INTERVAL="$2"; shift 2 ;;
        -w) WAIT_MODE=true; shift ;;
        --) shift; MAKE_ARGS=("$@"); break ;;
        *)  MAKE_ARGS=("$@"); break ;;
    esac
done

# --- Helper: get CPU usage % averaged over SAMPLE_WINDOW seconds ---
get_cpu_usage() {
    # Linux: read two snapshots from /proc/stat and compute delta
    read_cpu_fields() {
        awk '/^cpu / {print $2, $3, $4, $5, $6, $7, $8}' /proc/stat
    }

    local fields1 fields2
    fields1=($(read_cpu_fields))
    sleep "$SAMPLE_WINDOW"
    fields2=($(read_cpu_fields))

    # fields: user nice system idle iowait irq softirq
    local idle1=${fields1[3]} idle2=${fields2[3]}
    local total1=0 total2=0

    for v in "${fields1[@]}"; do total1=$((total1 + v)); done
    for v in "${fields2[@]}"; do total2=$((total2 + v)); done

    local total_delta=$((total2 - total1))
    local idle_delta=$((idle2 - idle1))

    if [[ $total_delta -eq 0 ]]; then
        echo "0"
    else
        awk "BEGIN { printf \"%.1f\", 100 * (1 - ${idle_delta} / ${total_delta}) }"
    fi
}

# --- Helper: compare floats (returns 0 if $1 < $2) ---
is_below() {
    awk "BEGIN { exit !($1 < $2) }"
}

# --- Main ---
echo "=== load-aware-make ==="
echo "Threshold    : ${THRESHOLD}%"
echo "Sample window: ${SAMPLE_WINDOW}s"
echo "Wait mode    : ${WAIT_MODE}"
echo "Make args    : ${MAKE_ARGS[*]:-<none>}"
echo "========================"

while true; do
    echo "[$(date '+%H:%M:%S')] Sampling CPU usage over ${SAMPLE_WINDOW}s..."
    USAGE=$(get_cpu_usage)
    echo "[$(date '+%H:%M:%S')] Average CPU usage: ${USAGE}%"

    if is_below "$USAGE" "$THRESHOLD"; then
        echo "CPU (${USAGE}%) is below threshold (${THRESHOLD}%). Starting make..."
        make "${MAKE_ARGS[@]}"
        exit $?
    fi

    if [[ "$WAIT_MODE" == false ]]; then
        echo "CPU (${USAGE}%) is at or above threshold (${THRESHOLD}%). Exiting."
        exit 1
    fi

    echo "CPU too high. Retrying in ${INTERVAL}s..."
    sleep "$INTERVAL"
done
