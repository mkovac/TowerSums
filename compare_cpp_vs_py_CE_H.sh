#!/bin/bash

# Base paths
current_dir="./output/stage_1_tower_sums/SinglePhotonPU0V16/CE_H"
afs_dir="/afs/cern.ch/work/m/mkovac/CMS/HGCAL/CMSSW_13_3_0/src/TPGStage1/Emulator/output/Stage1_TowerSums"

# Loop over sectors (0 to 6)
for sector in {0..2}; do
    # Loop over boards (0 to 13)
    for board in {0..13}; do
        # Construct filenames
        file_current="${current_dir}/TowerSums_Sector_${sector}_Board_${board}_CE_H.txt"
        file_afs="${afs_dir}/TowerSums_Sector_${sector}_Board_${board}_CE_H.txt"
        
        # Check if both files exist
        if [[ -f "$file_current" && -f "$file_afs" ]]; then
            # Compare files
            if diff -q "$file_current" "$file_afs" > /dev/null; then
                echo "Files match: Sector ${sector}, Board ${board}"
            else
                echo "Files differ: Sector ${sector}, Board ${board}"
            fi
        else
            # Report missing files
            if [[ ! -f "$file_current" ]]; then
                echo "Missing file in current directory: $file_current"
            fi
            if [[ ! -f "$file_afs" ]]; then
                echo "Missing file in AFS directory: $file_afs"
            fi
        fi
    done
done
