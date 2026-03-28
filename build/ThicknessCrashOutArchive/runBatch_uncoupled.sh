#!/bin/bash
# runBatch_uncoupled.sh
# Runs 20 batches of 50 events for a given thickness label — UNCOUPLED scan
# Geometry fixed at 1nm, only KaplanQP filmThickness varies
# Usage: ./runBatch_uncoupled.sh 10nm
# If a batch hangs, Ctrl+C and remove that seed from GOOD_SEEDS

THICKNESS=$1

if [ -z "$THICKNESS" ]; then
    echo "Usage: ./runBatch_uncoupled.sh [thickness_label]  e.g. ./runBatch_uncoupled.sh 10nm"
    exit 1
fi

# Hardcoded seeds — remove any that cause freezing
GOOD_SEEDS=(1000 2500 3000 300000 5500 6500 30000 35000 40000 11000)

NRUNS=${#GOOD_SEEDS[@]}
SUCCESSFUL=0
TOTAL_START=$SECONDS

echo "========================================="
echo "UNCOUPLED SCAN: Running $NRUNS batches for $THICKNESS"
echo "Geometry fixed at 1nm, KaplanQP filmThickness = $THICKNESS"
echo "Using seeds: ${GOOD_SEEDS[@]}"
echo "========================================="
echo ""

for i in $(seq 0 $((NRUNS-1))); do
    SEED=${GOOD_SEEDS[$i]}
    OUTFILE="./${THICKNESS}_uncoupled_batch${i}.root"
    BATCH_START=$SECONDS

    DONE=$i
    REMAINING=$((NRUNS - i))
    BAR=""
    for b in $(seq 1 $DONE);     do BAR="${BAR}█"; done
    for b in $(seq 1 $REMAINING); do BAR="${BAR}░"; done

    echo "[$BAR] Batch $((i+1))/$NRUNS (seed=$SEED)"
    echo -n "  Running... "

    sed "s|/random/setSeeds.*|/random/setSeeds $SEED 0|" \
        ../macro/Be7_G4RadioDecay_0Vbias.mac > tmp_macro_uncoupled_${SEED}.mac
    sed -i "" "s|/hvev/HitsRootFile.*|/hvev/HitsRootFile ${OUTFILE}|" \
        tmp_macro_uncoupled_${SEED}.mac
    sed -i "" "s|/g4cmp/verbose.*|/g4cmp/verbose 0|" \
        tmp_macro_uncoupled_${SEED}.mac

    ./g4cmpHVeV tmp_macro_uncoupled_${SEED}.mac > /dev/null 2>&1
    EXIT=$?

    BATCH_TIME=$((SECONDS - BATCH_START))
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

    rm -f tmp_macro_uncoupled_${SEED}.mac
done

TOTAL_TIME=$((SECONDS - TOTAL_START))
TOTAL_MIN=$((TOTAL_TIME / 60))
TOTAL_SEC=$((TOTAL_TIME % 60))

echo "========================================="
echo "Completed: $SUCCESSFUL/$NRUNS batches OK"
echo "Total time: ${TOTAL_MIN}m ${TOTAL_SEC}s"
echo "========================================="

if ls ./${THICKNESS}_uncoupled_batch*.root 1>/dev/null 2>&1; then
    echo "Merging into ${THICKNESS}_uncoupled_merged.root..."
    hadd ${THICKNESS}_uncoupled_merged.root ./${THICKNESS}_uncoupled_batch*.root
    rm -f ./${THICKNESS}_uncoupled_batch*.root
    echo "Done. Output: ${THICKNESS}_uncoupled_merged.root"
else
    echo "ERROR: No batch files to merge."
fi
