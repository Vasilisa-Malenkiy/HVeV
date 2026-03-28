#!/bin/bash
# runBatch.sh
# Runs 10 batches of 50 events for a given thickness label
# Usage: ./runBatch.sh 10nm
# If a batch hangs, Ctrl+C and remove that seed from GOOD_SEEDS

THICKNESS=$1

if [ -z "$THICKNESS" ]; then
    echo "Usage: ./runBatch.sh [thickness_label]  e.g. ./runBatch.sh 10nm"
    exit 1
fi

# Hardcoded seeds — remove any that cause freezing
GOOD_SEEDS=(500 1500 2500 3500 5500 6500 7500 8500 9800 10000 11000 12500 13500 14500 15500 16500 17500 18500 19500 25000)

NRUNS=${#GOOD_SEEDS[@]}
SUCCESSFUL=0
TOTAL_START=$SECONDS
BATCH_TIMES=()

echo "========================================="
echo "Running $NRUNS batches for $THICKNESS"
echo "Using seeds: ${GOOD_SEEDS[@]}"
echo "========================================="
echo ""

for i in $(seq 0 $((NRUNS-1))); do
    SEED=${GOOD_SEEDS[$i]}
    OUTFILE="./${THICKNESS}_batch${i}.root"
    BATCH_START=$SECONDS

    # Progress bar
    DONE=$i
    REMAINING=$((NRUNS - i))
    BAR=""
    for b in $(seq 1 $DONE);     do BAR="${BAR}█"; done
    for b in $(seq 1 $REMAINING); do BAR="${BAR}░"; done

    echo "[$BAR] Batch $((i+1))/$NRUNS (seed=$SEED)"
    echo -n "  Running... "

    # Build temporary macro
    sed "s|/random/setSeeds.*|/random/setSeeds $SEED 0|" \
        ../macro/Be7_G4RadioDecay_0Vbias.mac > tmp_macro_${SEED}.mac
    sed -i "" "s|/hvev/HitsRootFile.*|/hvev/HitsRootFile ${OUTFILE}|" \
        tmp_macro_${SEED}.mac
    sed -i "" "s|/g4cmp/verbose.*|/g4cmp/verbose 0|" \
        tmp_macro_${SEED}.mac

    # Run
    ./g4cmpHVeV tmp_macro_${SEED}.mac > /dev/null 2>&1
    EXIT=$?

    BATCH_TIME=$((SECONDS - BATCH_START))
    BATCH_TIMES+=($BATCH_TIME)

    # Compute average time per batch so far for ETA
    TOTAL_ELAPSED=$((SECONDS - TOTAL_START))
    AVG_TIME=$((TOTAL_ELAPSED / (i+1)))
    BATCHES_LEFT=$((NRUNS - i - 1))
    ETA=$((AVG_TIME * BATCHES_LEFT))
    ETA_MIN=$((ETA / 60))
    ETA_SEC=$((ETA % 60))

    if [ $EXIT -eq 0 ] && [ -f "$OUTFILE" ]; then
        echo "OK ✓  (${BATCH_TIME}s)"
        SUCCESSFUL=$((SUCCESSFUL+1))
    else
        echo "FAILED ✗  (${BATCH_TIME}s) — skipping, remove seed $SEED if persistent"
        rm -f "$OUTFILE"
    fi

    echo "  ETA: ~${ETA_MIN}m ${ETA_SEC}s remaining"
    echo ""

    rm -f tmp_macro_${SEED}.mac
done

TOTAL_TIME=$((SECONDS - TOTAL_START))
TOTAL_MIN=$((TOTAL_TIME / 60))
TOTAL_SEC=$((TOTAL_TIME % 60))

echo "========================================="
echo "Completed: $SUCCESSFUL/$NRUNS batches OK"
echo "Total time: ${TOTAL_MIN}m ${TOTAL_SEC}s"
echo "========================================="

# Merge all successful batch files
if ls ./${THICKNESS}_batch*.root 1>/dev/null 2>&1; then
    echo "Merging batch files into ${THICKNESS}_merged.root..."
    hadd ${THICKNESS}_merged.root ./${THICKNESS}_batch*.root
    rm -f ./${THICKNESS}_batch*.root
    echo "Done. Output: ${THICKNESS}_merged.root"
else
    echo "ERROR: No batch files to merge."
fi
